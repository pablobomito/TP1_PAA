#include "algoritmo.h"
#include <stdio.h>
#include <stdlib.h>

int qtdChavesEncontradas = 0;
bool fim = false ;
bool barreira = false;
int count = 0;

Posicao *caminho;

void initCaminho (int tamanho) {
    caminho = (Posicao*)malloc(tamanho*sizeof(Posicao));
}

void printarCaminho () {
    printf("\nPercurso: ");
    for (int i=0; i<count; i++){
        printf("[%d, %d]", caminho[i].linhaCaminho, caminho[i].colunaCaminho);
    }
}


void backtracking (Labirinto *labirinto, Posicao **matriz,int lin_init,int col_init) {

    if (fim){
        return;
    }

    if (lin_init >= labirinto->totalLinhas || col_init>= labirinto->totalColunas || lin_init < 0 || col_init < 0) {
        barreira = true;
        return;
    } 

    if (matriz[lin_init][col_init].valor == '1'){
        barreira = true;
        return;
    }
    
    if (matriz[lin_init][col_init].valor == 'X') {
        if (labirinto->totalChaves ==qtdChavesEncontradas )
        {
            fim=true;
            printf("endgame\n");
            caminho[count].linhaCaminho = lin_init;
            caminho[count].colunaCaminho = col_init;
            count++;

            printarCaminho();
        }
        return;
    }

    if (matriz[lin_init][col_init].valor == '#'){
        barreira = false;
        return;
    }

    if (matriz[lin_init][col_init].valor == 'C'){
        qtdChavesEncontradas++;
        matriz[lin_init][col_init].valor = '#';
        caminho[count].linhaCaminho = lin_init;
        caminho[count].colunaCaminho = col_init;
        count++;
        barreira = false;
    } else {
        matriz[lin_init][col_init].valor='1';
        caminho[count].linhaCaminho = lin_init;
        caminho[count].colunaCaminho = col_init;
        count++;
        barreira = false;
    }

        //Passo recursivo que verifica todos as direções possiveis
        
        backtracking(labirinto,matriz,lin_init,col_init-1);
        backtracking(labirinto,matriz,lin_init-1,col_init);
        backtracking(labirinto,matriz,lin_init,col_init+1);
        backtracking(labirinto,matriz,lin_init+1,col_init);



        //Volta recursiva
        //Retira *visitado da casa e, caso a propria, possui uma chave, irá remove-la. Além de que o jogo verificará se é um caso impossivel
        
        if (!fim && matriz[lin_init][col_init].valor == '#'){
               
            if (barreira){
                fim = true;
                printf("\nNão há um caminho possível para a entrada de dados fornecida");
                return;
            }
                    
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

void imprimirCaminhoCriativo(const Labirinto *labirinto, const char *mapaOriginal) {

    if (count == 0)
        return;

    Posicao ultima = caminho[count - 1];

    if (mapaOriginal[ultima.linhaCaminho * labirinto->totalColunas + ultima.colunaCaminho] != 'X')
        return;

    printf("\n\n=== CAMINHO NO LABIRINTO ===\n");
    printf("A = inicio | C = chave | X = bau | * = percurso | # = parede | . = livre\n\n");

    for (int i = 0; i < labirinto->totalLinhas; i++) {
        for (int j = 0; j < labirinto->totalColunas; j++) {

            char original = mapaOriginal[i * labirinto->totalColunas + j];
            bool pertenceAoCaminho = false;

            for (int k = 0; k < count; k++) {
                if (i == caminho[k].linhaCaminho &&
                    j == caminho[k].colunaCaminho) {

                    pertenceAoCaminho = true;
                    break;
                }
            }

            if (original == 'A' || original == 'C' || original == 'X')
                printf("%c ", original);
            else if (original == '1')
                printf("# ");
            else if (pertenceAoCaminho)
                printf("* ");
            else
                printf(". ");
        }

        printf("\n");
    }
}

    void iniciarJogo(Labirinto *labirinto, Posicao posInicial) {

    int total = labirinto->totalLinhas * labirinto->totalColunas;

    char *mapaOriginal = malloc((size_t)total * sizeof(char));

    if (mapaOriginal == NULL) {
        printf("Erro: nao foi possivel copiar o mapa.\n");
        return;
    }

    for (int i = 0; i < labirinto->totalLinhas; i++) {
        for (int j = 0; j < labirinto->totalColunas; j++) {

            mapaOriginal[i * labirinto->totalColunas + j] =
                labirinto->matriz[i][j].valor;
        }
    }

    initCaminho(total);

    if (caminho == NULL) {
        printf("Erro: nao foi possivel alocar o caminho.\n");
        free(mapaOriginal);
        return;
    }

    backtracking(labirinto, labirinto->matriz,
                 posInicial.linhaCaminho,
                 posInicial.colunaCaminho);

    imprimirCaminhoCriativo(labirinto, mapaOriginal);

    free(mapaOriginal);
    free(caminho);
    caminho = NULL;
}
