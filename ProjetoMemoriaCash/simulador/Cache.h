#ifndef CACHE_H
#define CACHE_H

#include "Memoria.h"
#include <string>
#include <vector>
#include <list>

struct LinhaCache {
    bool valida;
    bool suja;
    unsigned int tag;
    //std:: vector<char> dados;
};


enum PoliticaDeEscrita {
    WRITE_THROUGH,
    WRITE_BACK
};

class Cache : public Memoria {
public:
    Cache(const std::string& nome, int latencia, int associatividade, int tamanhoLinha,
          int tamanhoConjuntoAssociativo, PoliticaDeEscrita politicaDeEscrita, Memoria* proximoNivel);

    virtual ~Cache();

    int ler(unsigned int endereco) override;
    void escrever(unsigned int endereco) override;
    void imprimirEstatistica() override;

private:
    int associatividade_; // quantidade de linhas por conjunto 
    int tamanhoLinha_; // tamanho de byte das linhas 
    int tamanhoConjuntoAssociativo_; // quantidade de conjuntos na cache
    PoliticaDeEscrita politicaDeEscrita_;
    Memoria* proximoNivel_; // para guardar um endereço de um objeto do tipo memória ou que herde de memória


    std::vector<std::vector<LinhaCache>> cache_;// vetor de vetores --> primeiro indice = conjunto da cache segundo indice = linha
                                                        // conjunto 0 --> [linha0, linha1, ...]



    std::vector<std::list<unsigned int>> lista_LRU;; // LRU --> usada menos recente --> politica de substituição
                                                            // oque foi usado a MAIS tempo sai para um novo dado entrar
                                                            // conjunto 0 --> [0, 1...]
                                                            // primeiro numero na lista foi o ultimo usado
                                                            // quando acessar uma linha mandamos ela para o final da lista


    int leitura_;
    int escrita_;
    int acerto_; // hit --> esta na cache
    int erro_;// miss --> não esta na cache --> buscar o proximo nível da memória

    bool buscarNaCache(unsigned int endereco, LinhaCache*& linhaCache);
    void inserirNaCache(unsigned int endereco);
    void atualizarLRU(unsigned int conjunto, unsigned int tag);
    void pegarCampoEndereco(unsigned int endereco, unsigned int& tag, unsigned int& conjunto, unsigned int& offset);
    unsigned int reconstruirEndereco(unsigned int tag, unsigned int conjunto);
};

#endif
