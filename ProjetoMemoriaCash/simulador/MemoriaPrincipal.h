// interface .h --> oque faz
// ultimo e mais lento nivel de memória
// destino do que não foi para a cache



#ifndef MEMORIA_PRINCIPAL_H
#define MEMORIA_PRINCIPAL_H

#include <string>
#include "Memoria.h" // classe base
#include <map>

class MemoriaPrincipal : public Memoria {
public: 
    MemoriaPrincipal(const std::string& nome, int latencia);

    int ler(unsigned int endereco) override;
    void escrever(unsigned int endereco, int dado) override;
    void imprimirEstatistica() override;




//  esses são os atributos da classe
    private:
        std::map<unsigned int, int>dados_; // map --> "como se fosse um dicionário" armazena [chave]: valor --> representar os endereços e oque esta armazenado
        int leitura_ = 0;
        int escrita_ = 0;
};

#endif


