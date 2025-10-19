#include <iostream>
#include "Cache.h"
#include "MemoriaPrincipal.h"
#include "Processador.h"

int main() {
    // cria memória principal (último nível)
    MemoriaPrincipal ram("RAM", 100);

                           // associatividade, tamanho da linha,  numero de conjuntos, politica de escrita e ponteiro 
    Cache cacheL2("L2", 10, 4                 , 32             , 8                   , WRITE_BACK,            &ram);


    Cache cacheL1("L1", 2, 2, 32, 4, WRITE_BACK, &cacheL2);

 
    {
        std::cout << "____Sequencial____\n";    //bufferSize       num de acesso   passo         probabilidade de escrita 
        Processador cpuSeq(&cacheL1, SEQUENCIAL, 1024           ,       1000,              4,               0.3);
        long long tempoSeq = cpuSeq.executar();
        std::cout << "Tempo total de leitura: " << tempoSeq << " ciclos\n\n";

        cacheL1.imprimirEstatistica();
        cacheL2.imprimirEstatistica();
        ram.imprimirEstatistica();
    }

    {
        std::cout << "\n____Aleatorio____\n";
        Cache cacheL2b("L2b", 10, 4, 32, 8, WRITE_BACK, &ram);
        Cache cacheL1b("L1b", 2, 2, 32, 4, WRITE_BACK, &cacheL2b);

        Processador cpuRand(&cacheL1b, ALEATORIO, 1024, 1000, 1, 0.3);
        long long tempoRand = cpuRand.executar();
        std::cout << "Tempo total de leitura: " << tempoRand << " ciclos\n\n";

        cacheL1b.imprimirEstatistica();
        cacheL2b.imprimirEstatistica();
        ram.imprimirEstatistica();
    }

    return 0;
}
