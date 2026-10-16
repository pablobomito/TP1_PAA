#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "posicao.h"

typedef struct {
    int totalLinhas;
    int totalColunas;
    int totalChaves;
    Posicao **matriz;
} Labirinto;

#endif
