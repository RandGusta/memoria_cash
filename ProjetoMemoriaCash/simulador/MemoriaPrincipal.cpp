// implementação .cpp --> como faz
#include "MemoriaPrincipal.h"
#include "Memoria.h"
#include <iostream>

MemoriaPrincipal::MemoriaPrincipal(const std::string& nome, int latencia)
    : Memoria(nome, latencia) {                                            
}

int MemoriaPrincipal::ler(unsigned int endereco) {
    leitura_++;                                  // somando a cada chamada 
    if(dados_.count(endereco)){
        return dados_[endereco];

    } else {
        return 0; 
    }
}

void MemoriaPrincipal::escrever(unsigned int endereco, int dado) {
    escrita_++;
    dados_[endereco] = dado;
   
}

void MemoriaPrincipal::imprimirEstatistica() {
    std::cout << "MemoriaPrincipal" "("<<nome_<<"):\n";
    std::cout << "Leituras: "<< leitura_ << "\n";
    std::cout << "Escritas: "<< escrita_<< "\n";
}


