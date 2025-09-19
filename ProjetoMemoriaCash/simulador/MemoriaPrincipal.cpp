// implementação .cpp --> como faz
#include "MemoriaPrincipal.h"
#include "Memoria.h"
#include <iostream>

MemoriaPrincipal::MemoriaPrincipal(const std::string& nome, int latencia) // letencia --> ciclo de clocks que um acesso aquela memoria demora
    : Memoria(nome, latencia) {                                            // a latencia será somada toda vez que consultarmos um n nivel de hierarquia
}

int MemoriaPrincipal::ler(unsigned int endereco) {
    leituras_++;                                  // somando a cada chamada 
    if(dados_.count(endereco)){
        return dados_[endereco];

    } else {
        return 0; 
    }
}

void MemoriaPrincipal::escrever(unsigned int endereco, int dado) {
    escritas_++;
    dados_[endereco] = dado;
   
}

void MemoriaPrincipal::imprimirEstatisticas() {
    std::cout << "MemoriaPrincipal" "("<<nome_<<"):\n";
    std::cout << "Leituras: "<< leituras_ << "\n";
    std::cout << "Escritas: "<< escritas_<< "\n";
}


