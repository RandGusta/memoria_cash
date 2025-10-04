// implementação .cpp --> como faz
#include "MemoriaPrincipal.h"
#include "Memoria.h"
#include <iostream>

MemoriaPrincipal::MemoriaPrincipal(const std::string& nome, int latencia)
    : Memoria(nome, latencia) {                                            
}

int MemoriaPrincipal::ler(unsigned int endereco) {
    leitura_++;                                  // somando a cada chamada 
    return latencia_;


}

void MemoriaPrincipal::escrever(unsigned int endereco) {
    escrita_++;
}

void MemoriaPrincipal::imprimirEstatistica() {
    std::cout << "MemoriaPrincipal" "("<<nome_<<"):\n";
    std::cout << "Leituras: "<< leitura_ << "\n";
    std::cout << "Escritas: "<< escrita_<< "\n";
}


