#include "MemoriaPrincipal.h"
#include <iostream>
using namespace std;

int main() {
    MemoriaPrincipal ram("RAM", 100);

    ram.escrever(10, 42);
    ram.escrever(20, 99);

    cout << "Endereco 10 = " << ram.ler(10) << endl; 
    cout << "Endereco 20 = " << ram.ler(20) << endl;
    cout << "Endereco 30 (nao escrito) = " << ram.ler(30) << endl;

    ram.imprimirEstatistica();
    return 0;
}
