#ifndef LEITURA_H
#define LEITURA_H

#include <stdbool.h>

#include "../estruturas/labirinto.h"

bool lerLabirinto(const char *nomeArquivo, Labirinto *labirinto);
bool encontrarPontoInicial (Labirinto *labirinto, Posicao *pos_inicial);
void liberarLabirinto(Labirinto *labirinto);
void imprimeLabirinto(Labirinto *labirinto);

#endif
