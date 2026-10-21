#include <stdio.h>
#include <string.h>
#include "./leitura/leitura.h"
#include "./backtracking/algoritmo.h"

bool modoAnalise = false;

int main(int argc, char *argv[]) {
    // Define o arquivo de entrada
    const char *nomeArquivo; //= (argc > 1) ? argv[1] : "entrada.txt";

    for (int i = 1; i< argc; i++){
        if (strcmp(argv[i], "--analise") == 0){
            modoAnalise = true;
        } else {
            nomeArquivo = argv[i];
        }
    }

    if (nomeArquivo == NULL){
        nomeArquivo = "entrada.txt";
    }

    // Inicializa a estrutura do labirinto
    Labirinto labirinto = {0};

    // Tenta ler o arquivo
    printf("Lendo arquivo: %s\n\n", nomeArquivo);

    if (!lerLabirinto(nomeArquivo, &labirinto)){
        printf("Erro: nao foi possivel ler o labirinto.\n");
        return 1;
    }

    // Imprime o Labirinto
    imprimeLabirinto(&labirinto);
    
    //Algoritmo
    Posicao posicaoInicial;
    if (encontrarPontoInicial(&labirinto, &posicaoInicial)){
        iniciarJogo(&labirinto, posicaoInicial, modoAnalise);
    }

    // Libera a memoria utilizada
    liberarLabirinto(&labirinto);

    return 0;
}

//gcc main.c leitura/leitura.c -o programa