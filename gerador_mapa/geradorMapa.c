#include "geradorMapa.h"

// Macro auxiliar para facilitar a vizualizacao do codigo
#define MARCAR_CAMINHO(l, c) do { \
    lab->matriz[l][c].valor = '0'; \
    visitado[l][c] = 1; \
} while(0)

// A funcao Principal de Gerar Labirinto
int gerarLabirinto(Labirinto *lab, Posicao inicio, Posicao fim, Posicao *caminhoViavel, int *qtdcaminhoViavel, int limiteReto, int caminhoMinimoFracao) {
    int linhas = lab->totalLinhas;
    int colunas = lab->totalColunas;

    int **visitado; // Matriz de inteiros, armazena quais quadrados foram visitados (1) e quais nao foram (0)
    Estado *pilha; // Pilha da atual tentativa de caminho do inicio ate o fim
    int topo = 0; // Topo da Pilha

    // O algoritmo de busca em profundidade anda de 2 em 2 para formar os corredores com paredes 1
    int dl[] = {-2, 2, 0, 0}; // Usado para a movimentacao vertical da busca em profundidade
    int dc[] = {0, 0, -2, 2}; // Usado para a movimentacao horizontal da busca em profundidade

    int caminhoMinimo = (lab->totalLinhas * lab->totalColunas)/caminhoMinimoFracao; 

    // Variaveis para caso o labirinto termine muito rapido, elas guardam a ultima posicao e movimento decidido, para ficar guardado um caminho possivel
        // Depois de guardado o movimento e sua posicao, o algoritmo segue outro caminho com o intuito de aumentar o tamanho do labirinto
    int melhorProfundidade = -1;
    int melhorOrigemL = -1, melhorOrigemC = -1;
    int melhorDirecao = -1;


    // Criando e Verificando a pilha e o vetor visitado
    visitado = malloc(linhas * sizeof(int *)); 
    pilha = malloc(linhas * colunas * sizeof(Estado)); 

    if (!visitado || !pilha) {
        free(visitado);
        free(pilha);
        return 0;
    }

    // Transformando o vetor visitado em uma matriz de zeros
    for (int i = 0; i < linhas; i++) {
        visitado[i] = calloc(colunas, sizeof(int)); 

        if (!visitado[i]) { 
            for (int j = 0; j < i; j++)
                free(visitado[j]);

            free(visitado);
            free(pilha);
            return 0;
        }

        // O labirinto comeca com apenas paredes
        for (int j = 0; j < colunas; j++) {
            lab->matriz[i][j].valor = '1';
            lab->matriz[i][j].linhaCaminho = i;
            lab->matriz[i][j].colunaCaminho = j;
        }
    }

    // Como o algoritmo anda de 2 em 2 celulas, caso o inicio e o fim estejam em grides diferentes, ele cria um inicio falso na mesma gride do fim
    int inicioFalsoL = inicio.linhaCaminho;
    int inicioFalsoC = inicio.colunaCaminho;

    // ifs conferindo se ha a necessidade de o inicioFalso se diferir do inicio real, e se sim, onde ele ficara
    // LINHA
    if ((inicio.linhaCaminho % 2) != (fim.linhaCaminho % 2)) {
        if (inicio.linhaCaminho == 0) inicioFalsoL++;
        else inicioFalsoL--;
    }

    // COLUNA
    if ((inicio.colunaCaminho % 2) != (fim.colunaCaminho % 2)) {
        if (inicio.colunaCaminho == 0) inicioFalsoC++;
        else inicioFalsoC--;
    }

    // Cuidando do caso em que o inicioFalso fica na diagonal do inicio real
    if (inicioFalsoL != inicio.linhaCaminho) {
        MARCAR_CAMINHO(inicioFalsoL, inicio.colunaCaminho); // Macro no topo desse arquivo do codigo, aqui simplesmente para facilitar a leitura
                                                            // Marca essa celula como caminho (0 na matriz) e como visitado (1 em visitado)
    } 
    else if (inicioFalsoC != inicio.colunaCaminho) {
        MARCAR_CAMINHO(inicio.linhaCaminho, inicioFalsoC);
    }

    // Marcar o início real e o início falso como caminho
    MARCAR_CAMINHO(inicio.linhaCaminho, inicio.colunaCaminho);
    MARCAR_CAMINHO(inicioFalsoL, inicioFalsoC);


// O inicio falso que vai servir como o comeco da busca em profundidade
    pilha[topo++] = (Estado){inicioFalsoL, inicioFalsoC, -1, 0}; 

    while (topo > 0) { // Enquanto houver algo na pilha
        Estado atual = pilha[topo - 1];

        // Para caso a pilha chegue no fim
        if (atual.l == fim.linhaCaminho && atual.c == fim.colunaCaminho) {
            topo--;
            continue;
        }

        int candidatos[4];
        int qtdCandidatos = 0;
        int temOutraDirecao = 0;

        // Coleta direcoes validas
        for (int d = 0; d < 4; d++) {

            // nova linha e nova coluna
            int nl = atual.l + dl[d]; 
            int nc = atual.c + dc[d];

            if (nl < 0 || nl >= linhas || nc < 0 || nc >= colunas || visitado[nl][nc]) {
                continue;
            }

            // Se o proximo passo for o final
            if (nl == fim.linhaCaminho && nc == fim.colunaCaminho) {

                // O algoritmo guarda o passo mais longe do inicio que
                    //chega no fim, e continua a busca
                if (topo < caminhoMinimo && topo > melhorProfundidade) {
                    melhorProfundidade = topo;
                    melhorOrigemL = atual.l;
                    melhorOrigemC = atual.c;
                    melhorDirecao = d;

                    *qtdcaminhoViavel = topo;
                    for (int i = 0; i < topo; i++){
                        caminhoViavel[i] = lab->matriz[pilha[i].l][pilha[i].c];
                    }
                }
                continue;
            }

            candidatos[qtdCandidatos++] = d;
            if (d != atual.ultimaDirecao) {
                temOutraDirecao = 1;
            }
        }

        if (qtdCandidatos == 0) {
            topo--;
            continue;
        }

        // Filtro de corredores retos
        if (atual.comprimentoReto >= limiteReto && temOutraDirecao) {
            int qtdFiltrados = 0;
            for (int i = 0; i < qtdCandidatos; i++) {
                if (candidatos[i] != atual.ultimaDirecao) {
                    candidatos[qtdFiltrados++] = candidatos[i];
                }
            }
            qtdCandidatos = qtdFiltrados;
        }

        // Se apos o filtro sobrou 0 candidatos, recua
        if (qtdCandidatos == 0) {
            topo--;
            continue;
        }

        // Aleatoriza e avança no labirinto
        int d = candidatos[rand() % qtdCandidatos];

        int nl = atual.l + dl[d];
        int nc = atual.c + dc[d];
        int pl = atual.l + dl[d] / 2;
        int pc = atual.c + dc[d] / 2;

        // Movendo 2 posicoes no labirinto
        lab->matriz[pl][pc].valor = '0';
        lab->matriz[nl][nc].valor = '0';
        visitado[nl][nc] = 1; // Adicionando a nova posicao visitada

        int novoComprimento = (d == atual.ultimaDirecao) ? (atual.comprimentoReto + 1) : 1;

        pilha[topo++] = (Estado){nl, nc, d, novoComprimento};
    }

    // Apos o While, se não chegou no fim, usa o melhor passo alternativo
    if (!visitado[fim.linhaCaminho][fim.colunaCaminho] && melhorDirecao != -1) {
        int pl = melhorOrigemL + dl[melhorDirecao] / 2;
        int pc = melhorOrigemC + dc[melhorDirecao] / 2;

        lab->matriz[pl][pc].valor = '0';
        MARCAR_CAMINHO(fim.linhaCaminho, fim.colunaCaminho);
    }

    // O labirinto foi criado com sucesso
    int sucesso = visitado[fim.linhaCaminho][fim.colunaCaminho]; 

    if (sucesso) {
        lab->matriz[inicio.linhaCaminho][inicio.colunaCaminho].valor = 'A';
        lab->matriz[fim.linhaCaminho][fim.colunaCaminho].valor = 'X';
    }

    // Libera a memoria alocada
    for (int i = 0; i < linhas; i++) 
        free(visitado[i]);

    free(visitado);
    free(pilha);

    return sucesso;
}

// Confirma se uma celula eh caminho ou nao
int ehCaminho(char celula) {
    return celula == '0' || celula == 'A' || celula == 'X';
}

// Função para abrir mais caminhos no labirinto, para deixar ele mais interessante
int abrirCaminhosExtras(Labirinto *lab, int espessuraParedeMax, int quantidadeExtrasFracao) {
    int linhas = lab->totalLinhas;
    int colunas = lab->totalColunas;

    int quantidadeExtras = (linhas * colunas) / quantidadeExtrasFracao;

    // Validação inicial antecipada
    if (lab == NULL || lab->matriz == NULL ||
        espessuraParedeMax <= 0 || quantidadeExtras <= 0) {
        return 0;
    }

    int capacidade = 2 * linhas * colunas;
    ParedeCandidata *candidatas = malloc(capacidade * sizeof(ParedeCandidata));
    if (candidatas == NULL) return 0;

    int totalCandidatas = 0;
    int dl[] = {0, 1};
    int dc[] = {1, 0};

    // Varredura para encontrar as paredes candidatas
    for (int d = 0; d < 2; d++) {
        int dir_l = dl[d];
        int dir_c = dc[d];

        for (int l = 0; l < linhas; l++) {
            for (int c = 0; c < colunas; c++) {
                if (!ehCaminho(lab->matriz[l][c].valor)) continue;

                int pl = l + dir_l; // Proxima Linha (linha atual + direcao linha)
                int pc = c + dir_c; // Proxima Coluna (Coluna atual + direcao Coluna)
                int comprimento = 0;

                // Saindo de uma celula caminho, quao espesso sao as paredes do seu lado
                while (pl >= 0 && pl < linhas && pc >= 0 && pc < colunas && 
                    lab->matriz[pl][pc].valor == '1' && comprimento <= espessuraParedeMax) {
                    comprimento++;
                    pl += dir_l;
                    pc += dir_c;
                }

                // Valida se o comprimento está dentro do limite e se termina em um caminho válido
                if (comprimento >= 1 && comprimento <= espessuraParedeMax &&
                    pl >= 0 && pl < linhas && pc >= 0 && pc < colunas &&
                    ehCaminho(lab->matriz[pl][pc].valor)) {

                    if (totalCandidatas < capacidade) {
                        candidatas[totalCandidatas++] = (ParedeCandidata){l, c, dir_l, dir_c, comprimento};
                    }
                }
            }
        }
    }

    // Embaralha a lista de candidatas (Fisher-Yates Shuffle)
    for (int i = totalCandidatas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        ParedeCandidata temp = candidatas[i];
        candidatas[i] = candidatas[j];
        candidatas[j] = temp;
    }

    int abertas = 0;

    // Abre caminho nas paredes candidatas
    for (int i = 0; i < totalCandidatas && abertas < quantidadeExtras; i++) {
        ParedeCandidata p = candidatas[i];

        for (int k = 1; k <= p.comprimento; k++) {
            lab->matriz[p.l + k * p.dl][p.c + k * p.dc].valor = '0';
        }
        abertas++;
    }

    free(candidatas);
    return abertas;
}

// Adiciona as Chaves pelo Caminho Viavel (O maior caminho de A ate X)
int adicionarChaves(Labirinto *lab, Posicao *caminho, int qtdPosicoes, int nChaves){
    if (lab == NULL || lab->matriz == NULL || caminho == NULL || qtdPosicoes <= 0 || nChaves <= 0) {
        return 0;
    }

    // Embaralha o vetor caminho diretamente (Fisher-Yates Shuffle)
    // Esse vetor nao vai ser mais usado depois
    for (int i = qtdPosicoes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Posicao temp = caminho[i];
        caminho[i] = caminho[j];
        caminho[j] = temp;
    }

    int adicionadas = 0;

    // Tenta colocar as chaves nas posições embaralhadas
    for (int i = 0; i < qtdPosicoes && adicionadas < nChaves; i++) {
        int l = caminho[i].linhaCaminho;
        int c = caminho[i].colunaCaminho;

        // Se for um espaço vazio '0', coloca a chave 'C'
        if (lab->matriz[l][c].valor == '0') {
            lab->matriz[l][c].valor = 'C';
            adicionadas++;
        }
    }
    return adicionadas;
}

// Funcao para aleatorizar a posicao do inicio e do fim, caso solicitado no menu
void posicaoAleatoria(Posicao *posicao, int limiteLinha, int limiteColuna) {
    if (posicao->linhaCaminho < 0)
        posicao->linhaCaminho = rand() % limiteLinha;

    if (posicao->colunaCaminho < 0)
        posicao->colunaCaminho = rand() % limiteColuna;
}

// Funcao que controla o menu interativo da criacao de labirintos
void menu(Labirinto *labirinto, Posicao *inicio, Posicao *fim, int *nChaves, int *limiteReto, int *espessuraParedeMax, int *quantidadeExtrasFracao, int *caminhoMinimoFracao) {
          
    printf("========================================\n");
    printf("         GERADOR DE LABIRINTO           \n");
    printf("========================================\n\n");

    // DIMENSOES DO LABIRINTO
    while (1) {
        printf("Digite o numero de linhas e colunas (minimo 4): ");
        if (scanf("%d %d", &labirinto->totalLinhas, &labirinto->totalColunas) != 2) {
            printf("[Erro] Digite apenas numeros inteiros.\n");
            while (getchar() != '\n');
            continue;
        }

        if (labirinto->totalLinhas > 3 && labirinto->totalColunas > 3) {
            break;
        }
        printf("[Erro] Os valores devem ser maiores que 3.\n");
    }

    // POSICAO INICIAL (A)
    printf("\n--- Posicao de Inicio (A) ---\n");
    printf("Digite a linha e a coluna (ou valores negativos para sortear): ");
    if (scanf("%d %d", &inicio->linhaCaminho, &inicio->colunaCaminho) != 2) {
        inicio->linhaCaminho = -1; // Força aleatório em caso de erro de leitura
        inicio->colunaCaminho = -1;
        while (getchar() != '\n');
    }
    
    if (inicio->linhaCaminho < 0 || inicio->colunaCaminho < 0 || 
        inicio->linhaCaminho >= labirinto->totalLinhas || inicio->colunaCaminho >= labirinto->totalColunas) {
        posicaoAleatoria(inicio, labirinto->totalLinhas, labirinto->totalColunas);
        printf("-> Inicio sorteado automaticamente: (%d, %d)\n", inicio->linhaCaminho, inicio->colunaCaminho);
    }

    // POSIÇÃO FINAL (X)
    printf("\n--- Posicao do Bau (X) ---\n");
    printf("Digite a linha e a coluna (ou valores negativos para sortear): ");
    if (scanf("%d %d", &fim->linhaCaminho, &fim->colunaCaminho) != 2) {
        fim->linhaCaminho = -1;
        fim->colunaCaminho = -1;
        while (getchar() != '\n');
    }

    if (fim->linhaCaminho < 0 || fim->colunaCaminho < 0 || 
        fim->linhaCaminho >= labirinto->totalLinhas || fim->colunaCaminho >= labirinto->totalColunas ||
        (fim->linhaCaminho == inicio->linhaCaminho && fim->colunaCaminho == inicio->colunaCaminho)) {
        
        posicaoAleatoria(fim, labirinto->totalLinhas, labirinto->totalColunas);
        while (fim->linhaCaminho == inicio->linhaCaminho && fim->colunaCaminho == inicio->colunaCaminho) {
            posicaoAleatoria(fim, labirinto->totalLinhas, labirinto->totalColunas);
        }
        printf("-> Bau sorteado automaticamente: (%d, %d)\n", fim->linhaCaminho, fim->colunaCaminho);
    }

    // CONFIGURACOES EXTRAS
    printf("\nDeseja configurar os parametros avancados do labirinto? (1- Sim / 0- Nao [Usar Padroes]): ");
    int opcaoAvancada = 0;
    if (scanf("%d", &opcaoAvancada) != 1) {
        opcaoAvancada = 0;
        while (getchar() != '\n');
    }

    // Atribui valores padrao iniciais
    *limiteReto = LIMITE_RETO;
    *espessuraParedeMax = ESPESSURA_PAREDE_MAX;
    *quantidadeExtrasFracao = QTD_CAMINHOS_EXTRAS_FRACAO;
    *caminhoMinimoFracao = CAMINHO_MINIMO_FRACAO;
    int maxChaves = (labirinto->totalLinhas * labirinto->totalColunas) - 2;
    *nChaves = NUMERO_CHAVE_PADRAO; // Valor padrão inicial de chaves

    if (opcaoAvancada == 1) {
        printf("\n--- Parametros Avancados ---\n");
        
        printf("Limite de movimentos retos [Padrao: 2]: ");
        scanf("%d", limiteReto);

        printf("Espessura maxima de parede para criar corredores [Padrao: 3]: ");
        scanf("%d", espessuraParedeMax);

        printf("Fracao de caminhos extras (Tamanho/X) [Padrao: 20]: ");
        scanf("%d", quantidadeExtrasFracao);

        printf("Fracao do caminho minimo A-X (Tamanho/X) [Padrao: 5]: ");
        scanf("%d", caminhoMinimoFracao);

        printf("Quantidade de chaves (Max: %d) [Padrao: %d]: ", maxChaves, *nChaves);
        scanf("%d", nChaves);
    }

    // Validacao final de segurança para o numero de chaves
    if (*nChaves > maxChaves) *nChaves = maxChaves;
    if (*nChaves < 0) *nChaves = 0;
}

// Funcao Chamada pelo main
int gerarMapa(Labirinto *lab){
    // Inicializa a semente para numeros aleatorios
    srand(time(NULL));

    Posicao inicio;
    Posicao fim;

    int limiteReto;
    int caminhoMinimoFracao;
    int espessuraParedeMax;
    int quantidadeExtraFracao;

    menu(lab, &inicio, &fim, &lab->totalChaves, &limiteReto, &espessuraParedeMax, &quantidadeExtraFracao, &caminhoMinimoFracao);

    // Alocar os espacos para a matriz
    lab->matriz = malloc(lab->totalLinhas * sizeof(Posicao *));
    for (int i = 0; i < lab->totalLinhas; i++) {
        lab->matriz[i] = malloc(lab->totalColunas * sizeof(Posicao));
    }

    // Alocar vetor para capturar o caminho viavel retornado pelo gerador
    int maxPosicoes = lab->totalLinhas * lab->totalColunas;
    Posicao *caminhoViavel = malloc(maxPosicoes * sizeof(Posicao));
    int qtdCaminho = 0;


    printf("Gerando labirinto...\n");
    int sucesso = gerarLabirinto(lab, inicio, fim, caminhoViavel, &qtdCaminho, limiteReto, caminhoMinimoFracao);

    if (!sucesso) {
        printf("Falha ao gerar o labirinto!\n");
    } else {

        //Adicionar caminhos extras (deixar o labirinto menos linear / com loops)
        abrirCaminhosExtras(lab, espessuraParedeMax, quantidadeExtraFracao); // Ex: paredes de até tamanho 3, abre 5 extras

        // Adicionar Chaves ('C') ao longo do caminho viavel
        int chavesAdicionadas = adicionarChaves(lab, caminhoViavel, qtdCaminho, lab->totalChaves);
    }

    free(caminhoViavel);
}