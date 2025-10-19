#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include <random>
#include <cstddef>
#include "Memoria.h"

enum TipoAcesso {
    SEQUENCIAL,
    ALEATORIO
};

class Processador {
public:
    Processador(Memoria* primeiroNivel,
                TipoAcesso tipoAcesso,
                size_t tamanhoBuffer,
                size_t quantidadeAcessos,
                size_t passo = 1,
                double probabilidadeEscrita = 0.5);

    long long executar();

private:
    Memoria* primeiroNivel_; 
    TipoAcesso tipoAcesso_;              
    size_t tamanhoBuffer_; // tamanho máximo de endereços
    size_t quantidadeAcessos_;           
    size_t passo_; // distância entre acessos no modo sequencial
    double probabilidadeEscrita_;// chance de ser um acesso de escrita

    std::mt19937 gerador_;   // nuemro aleatorio para o acesso aleatorio 
    std::uniform_real_distribution<double> distribuicaoProbabilidade_; // entre 0 e 1 probabilidade de escrita 
    std::uniform_int_distribution<unsigned int> distribuicaoEndereco_; 

    unsigned int posicaoSequencial_;// posição atual --> acessos sequenciais

    unsigned int proximoEndereco();
};

#endif
