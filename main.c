#include <stdio.h>
#include "./leitura/leitura.h"
#include "./backtracking/algoritmo.h"


int main(int argc, char *argv[]) {
    // Define o arquivo de entrada
    const char *nomeArquivo = (argc > 1) ? argv[1] : "entrada.txt";

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
    Posicao posicaoInicial = encontrarPontoInicial(&labirinto);
    iniciarJogo(&labirinto, posicaoInicial);

    // Libera a memoria utilizada
    liberarLabirinto(&labirinto);

    return 0;
}

//gcc main.c leitura/leitura.c -o programa