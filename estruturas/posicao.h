#ifndef POSICAO_H
#define POSICAO_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Posicao
{
    char valor;

    //Posição para armazenar o caminho do explorador
    int linhaCaminho;
    int colunaCaminho;

} Posicao;


#endif