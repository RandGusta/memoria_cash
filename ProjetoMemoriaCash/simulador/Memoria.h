// interface comum feita para todos os tipos de memória
//arquivo com extensão .h --> Pois so contêm a declaração de uma classse abstrata, dessa forma não precisa ser 
//separação de interfaces (oque a classe faz) e codigos de implementação (como ela faz)

#ifndef MEMORIA_H // uma vez por compilação para evitar erros
#define MEMORIA_H
#include <string> // biblioteca String

class Memoria{ 
    public: // construtor em C++
    Memoria(const std::string& nome, int latencia) : nome_(nome), latencia_(latencia){} // '&' a função acessa a string diretamente no local da memória
                                                                                        // não cria uma copia
                                                                                        // '_' seria tipo o 'this' em Java
    
    virtual  ~Memoria(){} // Destrutor virtual, importante para o polimorfismo e herança em C++


    virtual int ler(unsigned int endereco) = 0; 
    virtual void escrever(unsigned int endereco, int dado) = 0;
    virtual void imprimirEstatistica() = 0;

    protected:
        std::string nome_;
        int latencia_; 

};

#endif 