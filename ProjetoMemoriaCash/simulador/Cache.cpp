#include "Cache.h"
#include <iostream>

Cache::Cache(const std::string& nome, int latencia, int associatividade, int tamanhoLinha,
             int tamanhoConjuntoAssociativo, PoliticaDeEscrita politicaDeEscrita, Memoria* proximoNivel)
    : Memoria(nome, latencia),
      associatividade_(associatividade),
      tamanhoLinha_(tamanhoLinha),
      tamanhoConjuntoAssociativo_(tamanhoConjuntoAssociativo),
      politicaDeEscrita_(politicaDeEscrita),
      proximoNivel_(proximoNivel),
      leitura_(0), escrita_(0), acerto_(0), erro_(0) {



    cache_.resize(tamanhoConjuntoAssociativo_);// espaço para cada conjunto --> criando so a quantidade de conjuntos --> o vetor externo
    lista_LRU.resize(tamanhoConjuntoAssociativo_);// lista para cada conjunto


    for (int i = 0; i < tamanhoConjuntoAssociativo_; i++) {
        cache_[i].resize(associatividade_);// criando vetor interno --> linhas


        for (int j = 0; j < associatividade_; j++) {
            cache_[i][j].valida = false;
            cache_[i][j].suja = false;
            cache_[i][j].tag = 0;
            // linhaCache->dados removido --> depois de conversar com o professor

        }
    }
}

Cache::~Cache() {}

int Cache::ler(unsigned int endereco) {
    leitura_++;
    LinhaCache* linha = NULL; // ponteiro que aponta para null
     
    // cada nivel soma sua própria latência
    int ciclos = latencia_;// a latencia do nivel sempre conta mesmo com hit


    if (buscarNaCache(endereco, linha)) {
        acerto_++;
        return ciclos;// em hit retorna apenas a latencia do nivel que estava pois foi encontrado
    } else {
        erro_++;
        int tempoProximoNivel = proximoNivel_->ler(endereco);
        // retornando a latencia dos proximos niveís
        inserirNaCache(endereco);// buscado para escrever no nivel atual
        return ciclos + tempoProximoNivel;// soma a latência do nivel atual e do proximo
    }
}


void Cache::escrever(unsigned int endereco) {
    escrita_++;
    LinhaCache* linha = NULL;

    if (buscarNaCache(endereco, linha)) {
        acerto_++;
        if (politicaDeEscrita_ == WRITE_THROUGH) {
            // imediatamente no proximo nivel --> não deixa a linha suja
            proximoNivel_->escrever(endereco);
        } else if (politicaDeEscrita_ == WRITE_BACK) {  // marca a linha como suja dados so serão enviados quando a linha for susbstituida
            linha->suja = true;
        }
    } else {
        erro_++;
        if (politicaDeEscrita_ == WRITE_THROUGH) {
            proximoNivel_->escrever(endereco);
        } else if (politicaDeEscrita_ == WRITE_BACK) {
            inserirNaCache(endereco); //carrega bloco da cache para marcar como suja no futuro
            unsigned int tag, conjunto, offset;
            pegarCampoEndereco(endereco, tag, conjunto, offset);// procura a linha e marca como suja
            for (int i = 0; i < associatividade_; i++) {
                if (cache_[conjunto][i].valida && cache_[conjunto][i].tag == tag) {
                    cache_[conjunto][i].suja = true;
                    break;
                }
            }
        }
    }
}






bool Cache::buscarNaCache(unsigned int endereco, LinhaCache*& linhaCache) {
    unsigned int tag, conjunto, offset;
    pegarCampoEndereco(endereco, tag, conjunto, offset);

    for (int i = 0; i < associatividade_; i++) {
        LinhaCache& linha = cache_[conjunto][i];
        if (linha.valida && linha.tag == tag) {
            linhaCache = &linha;

            atualizarLRU(conjunto, tag);
            return true;
        }
    }
    return false;
}






void Cache::inserirNaCache(unsigned int endereco) {
    unsigned int tag, conjunto, offset;
    pegarCampoEndereco(endereco, tag, conjunto, offset);

    std::vector<LinhaCache>& linhas = cache_[conjunto];
    std::list<unsigned int>& lista = lista_LRU[conjunto];

    for (int i = 0; i < associatividade_; i++) {
        if (!linhas[i].valida) { // conjunto ainda não cheio --> diferente de valida
            linhas[i].valida = true;
            linhas[i].suja = false;
            linhas[i].tag = tag;
            atualizarLRU(conjunto, tag);
            return;
        }
    }

 // caso esteja cheia
    unsigned int tagRemovida = lista.back(); //pega a tag menos usada a LRU
    lista.pop_back();// remove da lista

    for (int i = 0; i < associatividade_; i++) {
        if (linhas[i].tag == tagRemovida) { 

             // Se writeBack e estava suja --> escreve no próximo nível ANTES DE SUBSTITUIRMOS
            if (politicaDeEscrita_ == WRITE_BACK && linhas[i].suja) {
                unsigned int enderecoAntigo = reconstruirEndereco(linhas[i].tag, conjunto);
                proximoNivel_->escrever(enderecoAntigo);
            }

            // substitui a linha
            linhas[i].valida = true;
            linhas[i].suja = false;
            linhas[i].tag = tag;
            atualizarLRU(conjunto, tag);
            return;
        }
    }
}


// atualiza a lista LRU do conjunto: mantemos o elemento MAIS-RECENTE na frente
void Cache::atualizarLRU(unsigned int conjunto, unsigned int tag) {
    auto& lista = lista_LRU[conjunto];


    // remove ocorrência anterior da tag (se existir) opara n ser duplicada
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (*it == tag) {
            lista.erase(it);
            break;
        }
    }

    // coloca a tag como MAIS RECENTE (front)
    lista.push_front(tag);

    // segurança --> não deixar a lista maior que a associatividade
    while (lista.size() > static_cast<size_t>(associatividade_)) {
        lista.pop_back();
    }
}





void Cache::pegarCampoEndereco(unsigned int endereco,
  unsigned int& tag, unsigned int& conjunto, unsigned int& offset) {
 
    // calculando o numero de bits do offsetmsa
    unsigned int offsetBits = 0; // off set vai ser log(tamanhoLinha_) na base 2
    unsigned int auxiliar = static_cast<unsigned int>(tamanhoLinha_); // auxiliar --> tamnho da linha
    while (auxiliar > 1) { auxiliar >>= 1; ++offsetBits; } // dividindo auxiliar 2 --> movimentando os bits para a direita


    // calculando o numero de bits do indice do conjunto
    unsigned int conjuntoBits = 0; // tbm é log na base 2
    auxiliar = static_cast<unsigned int>(tamanhoConjuntoAssociativo_);
    while (auxiliar > 1) { auxiliar >>= 1; ++conjuntoBits; }
        //       resto               3 bits    4 bits
        // [ TAG ............... ][ CONJUNTO ][ OFFSET ]
        //    qual bloco           conj. linha  byte dentro da linha
   
        // mascara de bits --> extraindo o campo off set do endereco
    unsigned int offsetMask = (offsetBits == 0) ? 0u : ((1u << offsetBits) - 1u);

        // mascara tbm --> extraindo conjunto
    unsigned int conjuntoMask = (conjuntoBits == 0) ? 0u : ((1u << conjuntoBits) - 1u);

        // isolando o offset
    offset = endereco & offsetMask;

        // isolando o conjunto --> retirando os bits menos significativos
    conjunto = (offsetBits == 0) ? 0u : ((endereco >> offsetBits) & conjuntoMask);


        // sobrou a tag
    tag = endereco >> (offsetBits + conjuntoBits);
}




// tentando buscar o endereço anterior para o metodo inserirNaCache
unsigned int Cache::reconstruirEndereco(unsigned int tag, unsigned int conjunto){
    unsigned int offsetBits = 0;
    unsigned int auxiliar = static_cast<unsigned int>(tamanhoLinha_);
    while (auxiliar > 1) { auxiliar >>= 1; ++offsetBits; }


    unsigned int conjuntoBits = 0;
    auxiliar = static_cast<unsigned int>(tamanhoConjuntoAssociativo_);
    while (auxiliar > 1) { auxiliar >>= 1; ++conjuntoBits; }


    // forrmula da reconstrução:
     // endereço = (TAG << (bitsConjunto + bitsOffset)) | (CONJUNTO << bitsOffset)
    unsigned int enderecoBase = (tag << (conjuntoBits + offsetBits)) | (conjunto << offsetBits);


    return enderecoBase;


}


void Cache::imprimirEstatistica() {
    std::cout << "[" << nome_ << "] Estatisticas:\n";
    std::cout << " Leituras: " << leitura_ << "\n";
    std::cout << " Escritas: " << escrita_ << "\n";
    std::cout << " Acertos (HITs): " << acerto_ << "\n";
    std::cout << " Erros (MISSes): " << erro_ << "\n";
    float taxaMiss = (leitura_ + escrita_ == 0) ? 0.0f : (erro_ * 100.0f / (leitura_ + escrita_));
    std::cout << " Taxa de miss: " << taxaMiss << "%\n\n";
}
