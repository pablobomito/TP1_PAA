#include <stdio.h>
#include <string.h>
#include "./leitura/leitura.h"
#include "./backtracking/algoritmo.h"
#include "./gerador_mapa/geradorMapa.h"

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

    int opcao;

    printf("=== MENU PRINCIPAL ===\n");
    printf("1 - Ler labirinto de um arquivo\n");
    printf("2 - Criar um novo labirinto\n");
    printf("Escolha uma opcao: ");

    if (scanf("%d", &opcao) != 1) {
        printf("Opcao invalida.\n");
        return 1;
    }

    // Inicializa a estrutura do labirinto
    Labirinto labirinto = {0};

    switch (opcao) {
        case 1: {
            // Usa o arquivo informado ou o arquivo padrao.
            if (nomeArquivo == NULL) {
                nomeArquivo = "entrada.txt";
            }

            printf("\nLendo arquivo: %s\n\n", nomeArquivo);

            if (!lerLabirinto(nomeArquivo, &labirinto)) {
                printf("Erro: nao foi possivel ler o labirinto.\n");
                return 1;
            }

            break;
        }

        case 2: {
            printf("\nCriando um novo labirinto...\n\n");

            if (!gerarMapa(&labirinto)) {
                printf("Erro ao gerar o labirinto.\n");
                return 1;
            }

            break;
        }

        default:
            printf("Opcao invalida.\n");
            return 1;
    }

    // Imprime o Labirinto
    printf("\n");
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

// gcc main.c leitura/leitura.c backtracking/algoritmo.c gerador_mapa/geradorMapa.c -o p