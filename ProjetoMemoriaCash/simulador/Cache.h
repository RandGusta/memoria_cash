
#ifndef CACHE_H
#define CACHE_H
#include "Memoria.h"
#include <string>
#include <vector>
#include <list>


struct LinhaCache {
    bool valida; 
    bool suja; 
    unsigned int tag; // os bytes mais significativos 
    std:: vector<char> dados;
}


class Cache : public Memoria {
    private:
        int tamanhoLinha = 0;
        
}



#endif
