#include "Processador.h"
#include <chrono>

Processador::Processador(Memoria* primeiroNivel,
                         TipoAcesso tipoAcesso,
                         size_t tamanhoBuffer, // size_ t--> tamanho de contagem de elementos de memoria 
                         size_t quantidadeAcessos,
                         size_t passo,
                         double probabilidadeEscrita)
    : primeiroNivel_(primeiroNivel),
      tipoAcesso_(tipoAcesso),
      tamanhoBuffer_(tamanhoBuffer),
      quantidadeAcessos_(quantidadeAcessos),
      passo_(passo == 0 ? 1 : passo),
      probabilidadeEscrita_(probabilidadeEscrita),
      gerador_(static_cast<unsigned int>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count())),
      distribuicaoProbabilidade_(0.0, 1.0),
      distribuicaoEndereco_(0, tamanhoBuffer_ > 0 ? static_cast<unsigned int>(tamanhoBuffer_ - 1) : 0),
      posicaoSequencial_(0)
{
}

unsigned int Processador::proximoEndereco() {
    if (tamanhoBuffer_ == 0) return 0;

    if (tipoAcesso_ == SEQUENCIAL) {
        unsigned int endereco = posicaoSequencial_;
        posicaoSequencial_ = static_cast<unsigned int>((posicaoSequencial_ + passo_) % tamanhoBuffer_); 
        return endereco;                                                              // dar a volta --> volta para o inicio
    } else {
        return distribuicaoEndereco_(gerador_);
    }
}

long long Processador::executar() {
    long long totalCiclos = 0;

    for (size_t i = 0; i < quantidadeAcessos_; ++i) {
        unsigned int endereco = proximoEndereco();
        double p = distribuicaoProbabilidade_(gerador_);
    
        bool ehEscrita = (p < probabilidadeEscrita_); // verifica se é escrita ou leitura 
        if (ehEscrita) {
            primeiroNivel_->escrever(endereco);
        } else {
            int ciclos = primeiroNivel_->ler(endereco);
            totalCiclos += ciclos;
        }
    }

    return totalCiclos; // ottal de ciclo 
}
