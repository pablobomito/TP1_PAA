#include "./processos/posicao.h"
#include <stdio.h>

int qtdChavesEncontradas = 0;
bool fim = false ;
int count = 0;

Posicao *caminho;

void initCaminho (int tamanho) {
    caminho = (Posicao*)malloc(tamanho*sizeof(Posicao));
}

void printarCaminho () {
    for (int i=0; i<count; i++){
        printf("[%d, %d]", caminho[i].linhaCaminho, caminho[i].colunaCaminho);
    }
}


bool verifica (Labirinto matriz[n][n], int lin, int col) {
    
}
void backtracking (Labirinto matriz[n][n],int lin_init,int col_init) {

    if (fim){
        return;
    }

    if (lin_init >=n || col_init>=n || lin_init < 0 || col_init < 0) return;

    if (matriz[lin_init][col_init]->celula.valor == '1') return;
    
    if (matriz[lin_init][col_init]->celula.valor == 'X') {
        if (matriz.totalcha ==qtdChavesEncontradas )
        {
            fim=true;
            printf("endgame\n");
            printarCaminho();
        }
        return;
    }

    if (matriz[lin_init][col_init]->celula.valor == 'C'){
        qtdChavesEncontradas++;
        matriz[lin_init][col_init]->celula.valor == '#';
        caminho[count].linhaCaminho = lin_init;
        caminho[count].colunaCaminho = col_init;
        count++;
    } else {
        matriz[lin_init][col_init].valor='1';
        caminho[count].linhaCaminho = lin_init;
        caminho[count].colunaCaminho = col_init;
        count++;
    }

        //Passo recursivo que verifica todos as direções possiveis
        if(!fim)
            {
                backtracking(matriz,lin_init+1,col_init);
                backtracking(matriz,lin_init-1,col_init);
                backtracking(matriz,lin_init,col_init+1);
                backtracking(matriz,lin_init,col_init-1);
            }

        //Volta recursiva
        //Retira visitado da casa e, caso a propria, possui uma chave, irá removera-la. Além de que o jogo verificará se é um caso impossivel
        if (!fim && matriz[lin_init][col_init].valor == '#'){
               
            fim = true;
            printf("\nNão há um caminho possível para a entrada de dados fornecida");
            return;
                    
            if (qtdChavesEncontradas>0)
                {
                    qtdChavesEncontradas--;
                    matriz[lin_init][col_init].valor = 'C';
                    count--;
                }

    
        } else if (!fim){

            matriz[lin_init][col_init].valor = '0' ;
            count--;
        }
    };