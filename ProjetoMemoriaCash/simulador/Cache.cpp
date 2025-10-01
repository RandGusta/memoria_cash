#include "Cache.h"
#include <iostream>
#include <iomanip>


Cache::Cache(const std::string& nome, int latencia, int associatividade,
             int tamanhoConjuntoAssociativo, PoliticaDeEscrita politicaDeEscrita, Memoria* proximoNivel)
    : Memoria(nome, latencia),
      associatividade_(associatividade),
      tamanhoLinha_(32), // valor fixo (32 bytes) para facilitar nossa vida o calculo da tag, conjunto, etc... por ser potência de 2 --> documentação do trabalho Perguntar para o eduardo
      tamanhoConjuntoAssociativo_(tamanhoConjuntoAssociativo),
      politicaDeEscrita_(politicaDeEscrita),
      proximoNivel_(proximoNivel) 

{

  cache_.resize(tamanhoConjuntoAssociativo_); // espaço para cada conjunto --> criando so a quantidade de conjuntos --> o vetor externo
  lista_LRU.resize(tamanhoConjuntoAssociativo_); // lista para cada conjunto

  for(int i=0; i<tamanhoConjuntoAssociativo_; i++){
    cache_[i].resize(associatividade_); // criando vetor interno --> linhas
    for(int j=0; i<associatividade_; j++){
      cache_[i][j].valida = false;
      cache_[i][j].suja = false;
      cache_[i][j].tag = 0;
      cache_[i][j].dados.resize(tamanhoLinha_, 0); // bytes da linha --> espaço reservado, nesse caso (32, 0) --> começa no zero

    }
  }
}

Cache::~Cache(){}

int Cache:: ler(unsigned int endereco){
  leitura_++;
  LinhaCache* linha = nullptr; // cria um ponteiro que aponta nulo

  if(buscarNaCache(endereco, linha)){  // hit --> dado já esta na cache 
    acerto_++;
    return linha->dados[0]; // acessa o campo em dados apontado '->' por linha //CALCULAR OFFSET

  } else{  // miss

    erro_++;
    int dado = proximoNivel_->ler(endereco);
    inserirNaCache(endereco, dado);
    return dado;

  }
}


void Cache::escrever(unsigned int endereco, int dado) {
    escrita_++;
    LinhaCache* linha = nullptr; // ponteiro null

    if (buscarNaCache(endereco, linha)) { // hit 
        acerto_++;
        linha->dados[0] = static_cast<char>(dado); // casting --> int para char

        if (politicaDeEscrita_ == WRITE_THROUGH) { // analisa a politica de escrita 
            proximoNivel_->escrever(endereco, dado); 

        } else if (politicaDeEscrita_ == WRITE_BACK) {
            linha->suja = true;
        }
    } else { // miss 
        erro_++;
        if (politicaDeEscrita_ == WRITE_THROUGH) {
            proximoNivel_->escrever(endereco, dado);

        } else if (politicaDeEscrita_ == WRITE_BACK) {
            inserirNaCache(endereco, dado);
        }
    }

// metodo buscarNaCache( int endereco, LinhaCache*& linhaCache) 
//        se encontrar vai apontar para a linha correspondente 
bool Cache::buscarNaCache(unsigned int endereco, LinhaCache*& linhaCache) {
  unsigned int tag, conjunto, offset;
  pegarCampoEndereco(endereco, tag, conjunto, offset);

  for(auto& linha : cache_[conjunto]) {
    if(linha.valida && linha.tag == tag) {
      linhaCache = &linha;
      atualizarLRU(conjunto,tag);
      return true;
    }
  }
  return false;
}

// metodo inserirNaCache(int endereco, int dado)
//        