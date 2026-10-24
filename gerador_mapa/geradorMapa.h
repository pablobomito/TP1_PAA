#ifndef GERADORMAPA_H
#define GERADORMAPA_H

#include "../estruturas/labirinto.h"

#define LIMITE_RETO 2 // Quantos movimentos (2 quadrado) pode se fazer em linha reta no algoritimo de busca em profundidade
#define CAMINHO_MINIMO_FRACAO 5 // O tamanho minimo do caminho do A ate o X (Tamanho da matriz/CAMINHO_MINIMO_FRACAO)

#define ESPESSURA_PAREDE_MAX 3 // O quao espessa pode ser a parede para se considerar um caminho por ela
#define QTD_CAMINHOS_EXTRAS_FRACAO 20 // Quantos caminhos extras um labirinto deve ter (Tamanho da matriz/QTD_CAMINHOS_EXTRAS_FRACAO)

#define NUMERO_CHAVE_PADRAO 1


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct { // Usado pela pilha do busca em profundidade
    int l, c;
    int ultimaDirecao;
    int comprimentoReto;
} Estado;

typedef struct { // Parede candidata ha virar um caminho
    int l, c;
    int dl, dc;
    int comprimento;
} ParedeCandidata;

// A funcao Principal de Gerar Labirinto
int gerarLabirinto(Labirinto *lab, Posicao inicio, Posicao fim, Posicao *caminhoViavel, int *qtdcaminhoViavel, int limiteReto, int caminhoMinimoFracao);

// Confirma se uma celula eh caminho ou nao
int ehCaminho(char celula);

// Funcao para abrir mais caminhos no labirinto, para deixar ele mais interessante
int abrirCaminhosExtras(Labirinto *lab, int espessuraParedeMax, int quantidadeExtras);

// Adiciona as Chaves ao Longo do Caminho Viavel (O maior caminho que sai do A e chega ate o X)
int adicionarChaves(Labirinto *lab, Posicao *caminho, int qtdPosicoes, int N);

// Funcao para aleatorizar a posicao do inicio e do fim, caso solicitado no menu
void posicaoAleatoria(Posicao *posicao, int limiteLinha, int limiteColuna);

// Funcao que controla o menu interativo da criacao de labirintos
void menu(Labirinto *labirinto, Posicao *inicio, Posicao *fim, int *nChaves, int *limiteReto, int *espessuraParedeMax, int *quantidadeExtrasFracao, int *caminhoMinimoFracao);

// Funcao Chamada pelo main
int gerarMapa(Labirinto *lab);

#endif