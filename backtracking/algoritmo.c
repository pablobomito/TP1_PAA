#include "./processos/posicao.h"
#include <stdio.h>

int qtdChavesEncontradas = 0;
bool fim = false ;
int count = 0;
bool verifica (Labirinto matriz[n][n], int lin, int col) {
    if (lin >=n || col>=n)
    {
        return false;
    }
    if (matriz[lin][col].valor == 1) {
        return false;
    }
    if (matriz[lin][col].temChave){
        qtdChavesEncontradas++;
    }
    if (matriz[lin][col].temBau) {
        if (matriz.totalcha ==qtdChavesEncontradas )
            {
                fim=true;
                printf("endgame");
            }
    }
}
void backtracking (Labirinto matriz[n][n],int lin_init,int col_init)
    {
        if(fim==false && verifica(matriz,lin_init+1,col_init))
            {
                matriz[lin_init+1][col_init].valor=1;
                
                backtracking(matriz,lin_init+1,col_init);

            }
        if(fim==false && verifica(matriz,lin_init-1,col_init))
            {
                matriz[lin_init-1][col_init].valor=1;
                backtracking(matriz,lin_init-1,col_init);
            }
        if(fim==false && verifica(matriz,lin_init,col_init+1))
            {
                matriz[lin_init][col_init+1].valor=1;
                backtracking(matriz,lin_init,col_init+1);
            }
        
        if(fim==false && verifica(matriz,lin_init,col_init-1))
            {
                matriz[lin_init][col_init-1].valor=1;
                backtracking(matriz,lin_init,col_init-1);
            }
        if (matriz[lin_init][col_init].temChave){
            if( matriz[lin_init][col_init-1].valor==1&&
                matriz[lin_init][col_init+1].valor==1&&
                matriz[lin_init-1][col_init].valor==1&&
                matriz[lin_init+1][col_init].valor==1)
                    {
                        fim = true;
                        printf("Não há um caminho possível para a entrada de dados fornecida");
                        return;
                    }
            if (qtdChavesEncontradas>0)
                {
                    qtdChavesEncontradas--;
                }

    
        }
        if(fim==false)
            {
                matriz[lin_init][col_init].valor = 0 ;
            }
    };