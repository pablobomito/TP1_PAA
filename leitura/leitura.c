#include "leitura.h"
#include <stdio.h>
#include <stdlib.h>


bool lerLabirinto(const char *nomeArquivo, Labirinto *labirinto){
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL){
        return false;
    }

    // Le as dimensoes
    if (fscanf(arquivo, "%d %d", &labirinto->totalLinhas, &labirinto->totalColunas) != 2 ||
        fscanf(arquivo, "%d", &labirinto->totalChaves) != 1 ||
        labirinto->totalLinhas <= 0 ||
        labirinto->totalColunas <= 0 ||
        labirinto->totalChaves < 0){

        printf("Erro na formatacao do arquivo de entrada!");

        fclose(arquivo);
        return false;
    }

    // Aloca as linhas da matriz
    labirinto->matriz = malloc(labirinto->totalLinhas * sizeof(Posicao *));

    // Erro ao criar a matriz
    if (labirinto->matriz == NULL){
        fclose(arquivo);
        return false;
    }

    // Aloca as colunas em cada linha e le os caracteres
    for (int i = 0; i < labirinto->totalLinhas; i++){
        labirinto->matriz[i] = malloc(labirinto->totalColunas * sizeof(Posicao));

        // Caso hara falha na alocacao das colunas
        if (labirinto->matriz[i] == NULL){
            // Libera as linhas ja alocadas
            for (int k = 0; k < i; k++){
                free(labirinto->matriz[k]);
            }

            free(labirinto->matriz);
            fclose(arquivo);
            return false;
        }

        // Prepara para ler os caracteres
        for (int j = 0; j < labirinto->totalColunas; j++){

            // Tenta ler os caracteres da matriz (Caso diferente de 1 eh erro)
            if (fscanf(arquivo, " %c", &labirinto->matriz[i][j].valor) != 1){

                printf("Erro na leitura da matriz");

                for (int k = 0; k <= i; k++){
                    free(labirinto->matriz[k]);
                }

                free(labirinto->matriz);
                fclose(arquivo);
                return false;
            }
        }
    }

    fclose(arquivo);
    return true;
}

void liberarLabirinto(Labirinto *labirinto){
    for (int i = 0; i < labirinto->totalLinhas; i++){
        free(labirinto->matriz[i]);
    }

    free(labirinto->matriz);
    labirinto->matriz = NULL;
}

void imprimeLabirinto(Labirinto *labirinto){
    // Exibe os dados lidos
    printf("Linhas: %d\n", labirinto->totalLinhas);
    printf("Colunas: %d\n", labirinto->totalColunas);
    printf("Total de chaves: %d\n", labirinto->totalChaves);

    printf("\n");

    // Exibe a matriz
    for (int i = 0; i < labirinto->totalLinhas; i++){
        for (int j = 0; j < labirinto->totalColunas; j++){
            printf("%c ", labirinto->matriz[i][j].valor);
        }
        printf("\n");
    }
}
