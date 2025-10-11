#include "MemoriaPrincipal.h"
#include "Cache.h"
#include <iostream>
using namespace std;

int main() {
    MemoriaPrincipal memoriaPrincipal("RAM", 50);

    Cache cacheL1("L1", 
                  2,                 // latência
                  2,                 // associatividade
                  32,                // tamanho da linha (fixo)
                  4,                 // número de conjuntos
                  WRITE_BACK,        // política de escrita
                  &memoriaPrincipal  // ponteiro para o próximo nível
    );

    std::cout << "===== TESTE DE CACHE =====" << std::endl;

    std::cout << "\nLendo endereço 100 (esperado: MISS)" << std::endl;
    int tempo1 = cacheL1.ler(100);
    std::cout << "Tempo total: " << tempo1 << " ciclos\n";

    std::cout << "\nLendo endereço 100 novamente (esperado: HIT)" << std::endl;
    int tempo2 = cacheL1.ler(100);
    std::cout << "Tempo total: " << tempo2 << " ciclos\n";

    std::cout << "\nEscrevendo no endereço 100 (esperado: HIT e suja=true)" << std::endl;
    cacheL1.escrever(100);

    std::cout << "\nEscrevendo no endereço 200 (pode gerar substituição dependendo do conjunto)" << std::endl;
    cacheL1.escrever(200);

    std::cout << "\n===== ESTATÍSTICAS =====" << std::endl;
    cacheL1.imprimirEstatistica();

    return 0;
}