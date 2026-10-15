#ifndef POSICAO_H
#define POSICAO_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Posicao
{
    union
    {
        struct
        {
            //Linha e coluna para salvar as posiçoes do caminho do explorador
            int linha;
            int coluna;

        } Mapeamento;
        struct
        {
           int valor;
           bool temChave;
           bool temBau;

        } celula;
        
        /* data */
    } dados;

} Posicao;


#endif