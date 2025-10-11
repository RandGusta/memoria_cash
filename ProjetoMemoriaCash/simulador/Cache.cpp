#include "Cache.h"
#include <iostream>
#include <iomanip>

Cache::Cache(const std::string& nome, int latencia, int associatividade, int tamanhoLinha,
             int tamanhoConjuntoAssociativo, PoliticaDeEscrita politicaDeEscrita, Memoria* proximoNivel)
    : Memoria(nome, latencia),
      associatividade_(associatividade),
      tamanhoLinha_(tamanhoLinha),
      tamanhoConjuntoAssociativo_(tamanhoConjuntoAssociativo),
      politicaDeEscrita_(politicaDeEscrita),
      proximoNivel_(proximoNivel)
{

    cache_.resize(tamanhoConjuntoAssociativo_); // espaço para cada conjunto --> criando so a quantidade de conjuntos --> o vetor externo
    lista_LRU.resize(tamanhoConjuntoAssociativo_);  // lista para cada conjunto

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

// Metodos herdados ___________________________________
int Cache::ler(unsigned int endereco) {
    leitura_++;
    LinhaCache* linha = nullptr; // ponteiro que aponta para null
    // cada nivel soma sua própria latência
    int ciclos = latencia_; // a latencia do nivel sempre conta mesmo com hit

    if (buscarNaCache(endereco, linha)) { //hit
        acerto_++;
        return ciclos; // em hit retorna apenas a latencia do nivel que estava pois foi encontrado
    } else { //miss
        erro_++;
        int tempoProximoNivel = proximoNivel_->ler(endereco); // retornando a latencia do(s) proximo(s) nivel(is)
        inserirNaCache(endereco);
        return ciclos + tempoProximoNivel; // soma a latência do nivel atual e do proximo 
    }
}


void Cache::escrever(unsigned int endereco) {
    escrita_++;
    LinhaCache* linha = nullptr;

    if (buscarNaCache(endereco, linha)) { //hit
        acerto_++;
        if (politicaDeEscrita_ == WRITE_THROUGH) { // imediatamente no proximo nivel --> não deixa a linha suja
            proximoNivel_->escrever(endereco);
        } 
        else if (politicaDeEscrita_ == WRITE_BACK) { // marca a linha como suja
            linha->suja = true;
        }
    } 
    else { //miss
        erro_++;

        if (politicaDeEscrita_ == WRITE_THROUGH) {
            proximoNivel_->escrever(endereco);
        } 
        else if (politicaDeEscrita_ == WRITE_BACK) { 
            inserirNaCache(endereco);  //carrega bloco da cache
            unsigned int tag, conjunto, offset;
            pegarCampoEndereco(endereco, tag, conjunto, offset); // procura a linah e marca como suja
            for (auto& linhaC : cache_[conjunto]) {
                if (linhaC.valida && linhaC.tag == tag) {
                    linhaC.suja = true;
                    break;
                }
            }
        }
    }
}


// Metodos especificos ____________________________________________________

bool Cache::buscarNaCache(unsigned int endereco, LinhaCache*& linhaCache) {
    unsigned int tag, conjunto, offset;
    pegarCampoEndereco(endereco, tag, conjunto, offset);

    for (auto& linha : cache_[conjunto]) { // itera sobre todas as linhas do conjunto
        if (linha.valida && linha.tag == tag) { // verifica se tag bate
            linhaCache = &linha;
            atualizarLRU(conjunto, tag); // agora essa linha é a mais recente
            return true;
        }
    }
    return false;
}


void Cache::inserirNaCache(unsigned int endereco) {
    unsigned int tag, conjunto, offset;
    pegarCampoEndereco(endereco, tag, conjunto, offset);

    auto& linhas = cache_[conjunto];
    auto& lista = lista_LRU[conjunto];


    for (auto& linha : linhas) {
        if (!linha.valida) { // conjunto ainda não cheio 
            linha.valida = true;
            linha.suja = false;
            linha.tag = tag;
            atualizarLRU(conjunto, tag);
            return;
        }
    }

    unsigned int tagRemovida = lista.back(); //pega a tag menos usada a LRU 
    lista.pop_back(); // remove da lista 

    for (auto& linha : linhas) {
        if (linha.tag == tagRemovida) {
            // Se writeBack e estava suja --> escreve no próximo nível
            if (politicaDeEscrita_ == WRITE_BACK && linha.suja) {
                proximoNivel_->escrever(endereco); // --> deveria calcular o endereço exato correspondente no próximo nível
            }

            // Substitui linha
            linha.valida = true;
            linha.suja = false;
            linha.tag = tag;
            atualizarLRU(conjunto, tag);
            return;
        }
    }
    
}


// atualiza a lista LRU do conjunto: mantemos o elemento MAIS-RECENTE no front
void Cache::atualizarLRU(unsigned int conjunto, unsigned int tag) {
    auto& lista = lista_LRU[conjunto];

    // remove ocorrência anterior da tag (se existir)
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (*it == tag) { 
            lista.erase(it);
            break;
        }
    }

    // coloca a tag como MAIS-RECENTE (front)
    lista.push_front(tag);

    // segurança --> não deixar a lista maior que a associatividade
    while (lista.size() > static_cast<size_t>(associatividade_)) {
        lista.pop_back();
    }
}


void Cache::pegarCampoEndereco(unsigned int endereco, 
  unsigned int& tag, unsigned int& conjunto, unsigned int& offset) {
 
    unsigned int offsetBits = 0; // off set vai ser log(tamanhoLinha_) na base 2
    unsigned int tmp = static_cast<unsigned int>(tamanhoLinha_); // tmp --> tamnho da linha
    while (tmp > 1) { tmp >>= 1; ++offsetBits; } // dividindo tmp 2 --> movimentando os bits para a direita

    unsigned int conjuntoBits = 0; // tbm é log na base 2
    tmp = static_cast<unsigned int>(tamanhoConjuntoAssociativo_);
    while (tmp > 1) { tmp >>= 1; ++conjuntoBits; }
        //       resto               3 bits    4 bits
        // [ TAG ............... ][ CONJUNTO ][ OFFSET ]
        //    qual bloco           conj. linha  byte dentro da linha

    
    
        // mascara de bits --> extraindo o campo off set do endereco
    unsigned int offsetMask = (offsetBits == 0) ? 0u : ((1u << offsetBits) - 1u);
        // mascara tbm --> extraindo conjunto
    unsigned int conjuntoMask = (conjuntoBits == 0) ? 0u : ((1u << conjuntoBits) - 1u);

        // isolando o offset 
    offset = endereco & offsetMask;

        // isolando o offset --> retirando os bits menos significativos 
    conjunto = (offsetBits == 0) ? 0u : ((endereco >> offsetBits) & conjuntoMask);

        // sobrou a tag 
    tag = endereco >> (offsetBits + conjuntoBits);
}
