#include "./processos/posicao.h"
#include <stdio.h>

int qtdChavesEncontradas = 0;

bool verifica (Labirinto matriz[n][n], int lin, int col) {
    if (matriz[lin][col].valor == 1) {
        return false;
    }
    if (matriz[lin][col].temChave){
        qtdChavesEncontradas++;
    }
    if (matriz[lin][col].temBau) {
        if 
    }
}
void backtracking ();