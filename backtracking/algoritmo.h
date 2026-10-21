#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "../estruturas/labirinto.h"

void initCaminho(int tamanho);
void printarCaminho();
void backtracking (Labirinto *labirinto, Posicao **matriz,int lin_init,int col_init);
void iniciarJogo (Labirinto *labirinto, Posicao posInicial, bool modo_analise);

#endif