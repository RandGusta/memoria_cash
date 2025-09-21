
#ifndef CACHE_H
#define CACHE_H
#include "Memoria.h"
#include <string>
#include <vector>
#include <list>


struct LinhaCache {
    bool valida; 
    bool suja; 
    unsigned int tag; // os bytes mais significativos colocar aqui 
    std:: vector<char> dados;
};


enum TipoEscrita {
    WRITE_THROUGH,
    WIRTE_BACK
};

class Cache : public Memoria {
    public:
        Cache(const std::string& nome, int latencia, int associatividade,
        int tamanhoLinho, int tamanhoConjunto, TipoEscrita tipoEscrita, Memoria* proximoNivel);
        
        virtual ~Cache();

        int ler(unsigned int endereco) override;
        void escrever(unsigned int endereco, int dado) override;
        void imprimirEstatistica() override;


        private:
            int associatividade_;
            int tamanhoLinha_;
            int tamanhoConjunto_;
            TipoEscrita tipoEscrita_;
            Memoria* proximoNivel_;



            std::vector<std::vector<LinhaCache>> cache_;
            std:: vector<std::list<unsigned int>> lista_LRU; // LRU --> usada menos recente --> politica de substituição 
                                                            // oque foi usado a menos tempo sai para um novo dado entrar

            

            int leitura_ = 0;
            int escrita_ = 0;
            int acerto_ = 0; // hit
            int erro_ = 0; // miss


            void pegarCampoEndereco(unsigned int endereco, unsigned int& tag, unsigned int& conjunto, unsigned int& offset);
            bool buscarNaCache(unsigned int endereco, LinhaCache*& linhaCache);
            void atualizarLRU(unsigned int conjunto, unsigned int tag);
            void inserirNaCache(unsigned int endereco, int dado);
        
};

#endif
