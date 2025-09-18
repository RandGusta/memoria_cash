#include <string> // biblioteca String
class Memoria{
    public: // construtor em C++
    Memoria(const std::string& nome, int latencia) : nome_(nome), latencia_(latencia){} // '&' a função acessa a string diretamente no local da memória
                                                                                        // não cria uma copia
                                                                                        // '_' seria tipo o 'this' em Java
    
    virtual  ~Memoria(){} // Destrutor virtual, importante para o polimorfismo e herança em C++


    virtual int ler(unsigned int endereco) = 0;
    virtual void escrever(unsigned int endereco, int dado) = 0;
    virtual void imprimirEstatisticas() = 0;

    protected:
        std::string nome_;
        int latencia_; 

};