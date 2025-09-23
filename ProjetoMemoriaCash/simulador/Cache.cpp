#include "Cache.h"
#include <iostream>
#include <iomanip>


Cache::Cache(const std::string& nome, int latencia, int associatividade,
             int tamanhoConjuntoAssociativo, PoliticaDeEscrita politicaDeEscrita, Memoria* proximoNivel)
    : Memoria(nome, latencia),
      associatividade_(associatividade),
      tamanhoLinha_(32), // valor fixo (32 bytes) simplifica o calculo da tag, conjunto, etc... por ser potência de 2 --> documentação do trabalho Perguntar para o eduardo
      tamanhoConjuntoAssociativo_(tamanhoConjuntoAssociativo),
      politicaDeEscrita_(politicaDeEscrita),
      proximoNivel_(proximoNivel) 

{}