#include "geradorMapa.h"

// Confirma se uma celula eh caminho ou nao
int ehCaminho(char celula) {
    return celula == '0' || celula == 'A' || celula == 'X';
}

// A funcao Principal de Gerar Labirinto
int gerarLabirinto(Labirinto *lab, Posicao inicio, Posicao fim, Posicao *caminhoViavel, int *qtdcaminhoViavel) {
    int linhas = lab->totalLinhas;
    int colunas = lab->totalColunas;

    int **visitado; // Matriz de inteiros, armazena quais quadrados foram visitados (1) e quais nao foram (0)
    Estado *pilha; // Pilha da atual tentativa de caminho do inicio ate o fim
    int topo = 0; // Topo da Pilha

    // O algoritmo de busca em profundidade anda de 2 em 2 para formar os corredores com paredes 1
    int dl[] = {-2, 2, 0, 0}; // Usado para a movimentacao vertical da busca em profundidade
    int dc[] = {0, 0, -2, 2}; // Usado para a movimentacao horizontal da busca em profundidade

    int limiteReto = LIMITE_RETO;
    int caminhoMinimo = (lab->totalLinhas * lab->totalColunas)/CAMINHO_MINIMO_FRACAO; 

    // Variaveis para caso o labirinto termine muito rapido, elas guardam a ultima posicao e movimento decidido, para ficar guardado um caminho possivel
        // Depois de guardado o movimento e sua posicao, o algoritmo segue outro caminho com o intuito de aumentar o tamanho do labirinto
    int melhorProfundidade = -1;
    int melhorOrigemL = -1, melhorOrigemC = -1;
    int melhorDirecao = -1;

    // Validacao
    if (!lab || !lab->matriz ||
        linhas < 3 || colunas < 3 ||
        inicio.linhaCaminho < 0 || inicio.linhaCaminho >= linhas ||
        inicio.colunaCaminho < 0 || inicio.colunaCaminho >= colunas ||
        fim.linhaCaminho < 0 || fim.linhaCaminho >= linhas ||
        fim.colunaCaminho < 0 || fim.colunaCaminho >= colunas ||
        (inicio.linhaCaminho == fim.linhaCaminho && inicio.colunaCaminho == fim.colunaCaminho)) {
        return 0;
    }

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
    if ((inicio.linhaCaminho % 2) != (fim.linhaCaminho % 2)){
        if (inicio.linhaCaminho % 2){
            inicioFalsoL--;
        }
        else if(inicio.linhaCaminho == 0){
            inicioFalsoL++;
        }
        else{
            inicioFalsoL--;
        }
    }

    if ((inicio.colunaCaminho % 2) != (fim.colunaCaminho % 2)){
        if (inicio.colunaCaminho % 2){
            inicioFalsoC--;
        }
        else if(inicio.colunaCaminho == 0){
            inicioFalsoC++;
        }
        else{
            inicioFalsoC--;
        }
    }

    // Cuindando em caso do inicioFalso ficar na diagonal do inicio real
    if (inicioFalsoL != inicio.linhaCaminho) {
        lab->matriz[inicioFalsoL][inicio.colunaCaminho].valor = '0';
        visitado[inicioFalsoL][inicio.colunaCaminho] = 1;
    }
    else if (inicioFalsoC != inicio.colunaCaminho) {
        lab->matriz[inicio.linhaCaminho][inicioFalsoC].valor = '0';
        visitado[inicio.linhaCaminho][inicioFalsoC] = 1;
    }

    // Marcar o inicio real como caminho
    lab->matriz[inicio.linhaCaminho][inicio.colunaCaminho].valor = '0';
    visitado[inicio.linhaCaminho][inicio.colunaCaminho] = 1;

    // Marcar a posicao inicial falso caminho
    lab->matriz[inicioFalsoL][inicioFalsoC].valor = '0';
    visitado[inicioFalsoL][inicioFalsoC] = 1;

    // O inicio falso que vai servir como o comeco do busca em profundide
    pilha[topo] = (Estado){inicioFalsoL, inicioFalsoC, -1, 0}; 
    topo++; // A pilha comeca apenas com o inicio falso

    while (topo > 0) { // Enquanto houver algo na pilha
        Estado atual = pilha[topo - 1];

        // Para caso a pilha chegue no fim
        if (atual.l == fim.linhaCaminho && atual.c == fim.colunaCaminho) {
            topo--;
            continue;
        }

        // Variaveis para decidir a direcao da busca em profundidade
        int candidatos[4];
        int qtdCandidatos = 0;
        int temOutraDirecao = 0;

        for (int d = 0; d < 4; d++) {
            int nl = atual.l + dl[d];
            int nc = atual.c + dc[d];

            if (nl < 0 || nl >= linhas || nc < 0 || nc >= colunas || visitado[nl][nc]) {
                continue;
            }

            // Caso o proximo passo da busca seja o ultimo, ele guarda esse passa e sua posicao, 
                /// e tenta seguir outro caminho (para criar labirintos maiores e mais interessantes)
            if (nl == fim.linhaCaminho && nc == fim.colunaCaminho) {
                if (topo < caminhoMinimo) {
                    if (topo > melhorProfundidade) {
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
            }

            candidatos[qtdCandidatos++] = d;

            if (d != atual.ultimaDirecao)
                temOutraDirecao = 1;
        }

        if (qtdCandidatos == 0) {
            topo--;
            continue;
        }

        // O algoritmo tambem tenta evitar criar labirintos com apenas corredores retos, usando como base um limiteReto para o tamanho
        if (atual.comprimentoReto >= limiteReto && temOutraDirecao) {
            int filtrados[4];
            int qtdFiltrados = 0;

            for (int i = 0; i < qtdCandidatos; i++) {
                int d = candidatos[i];
                if (d != atual.ultimaDirecao)
                    filtrados[qtdFiltrados++] = d;
            }

            for (int i = 0; i < qtdFiltrados; i++)
                candidatos[i] = filtrados[i];

            qtdCandidatos = qtdFiltrados;
        }

        // Aleatorizando os candidatos para caminho

        int indice = rand() % qtdCandidatos;
        int d = candidatos[indice];

        int nl = atual.l + dl[d];
        int nc = atual.c + dc[d];

        int pl = atual.l + dl[d] / 2;
        int pc = atual.c + dc[d] / 2;

        // Movendo 2 posicoes no labirinto
        lab->matriz[pl][pc].valor = '0';
        lab->matriz[nl][nc].valor = '0';
        visitado[nl][nc] = 1;

        // Atualizando o quanto ele andou em linha reta ate agora
        int novoComprimento;
        if (d == atual.ultimaDirecao)
            novoComprimento = atual.comprimentoReto + 1;
        else
            novoComprimento = 1;

        Estado novoEstado = {
            nl,
            nc,
            d,
            novoComprimento
        };

        pilha[topo++] = novoEstado;
    }

    // Se ele nao chegou no fim do labirinto mas existe um melhor passo guardado para ele usar e chegar no final
    if (!visitado[fim.linhaCaminho][fim.colunaCaminho] && melhorDirecao != -1) {
        int pl = melhorOrigemL + dl[melhorDirecao] / 2;
        int pc = melhorOrigemC + dc[melhorDirecao] / 2;

        lab->matriz[pl][pc].valor = '0';
        lab->matriz[fim.linhaCaminho][fim.colunaCaminho].valor = '0';
        visitado[fim.linhaCaminho][fim.colunaCaminho] = 1;
    }

    // O labirinto foi criado com sucesso
    int sucesso = visitado[fim.linhaCaminho][fim.colunaCaminho]; 

    // Se tudo deu certo, ele escreve o A e o X em suas respectivas posicoes
    if (sucesso) {
        lab->matriz[inicio.linhaCaminho][inicio.colunaCaminho].valor = 'A';
        lab->matriz[fim.linhaCaminho][fim.colunaCaminho].valor = 'X';
    }

    // Libera a matriz visitado
    for (int i = 0; i < linhas; i++) 
        free(visitado[i]);

    free(visitado);
    free(pilha);

    return sucesso;
}

// Funcao para abrir mais caminhos no labirinto, para deixar ele mais interessante
int abrirCaminhosExtras(Labirinto *lab) {

    int espessuraParedeMax = ESPESSURA_PAREDE_MAX;
    int quantidadeExtras = (lab->totalLinhas * lab->totalColunas)/QTD_CAMINHOS_EXTRAS_FRACAO;

    // Validacao
    if (lab == NULL || lab->matriz == NULL ||
        lab->totalLinhas <= 0 || lab->totalColunas <= 0 ||
        espessuraParedeMax <= 0 || quantidadeExtras <= 0) {
        return 0;
    }

    int linhas = lab->totalLinhas;
    int colunas = lab->totalColunas;
    int capacidade = 2 * linhas * colunas;

    ParedeCandidata *candidatas = malloc(capacidade * sizeof(ParedeCandidata));
    if (candidatas == NULL) return 0;

    int totalCandidatas = 0;
    int dl[] = {0, 1};
    int dc[] = {1, 0};

    // Passando por toda a matriz, se achar uma parede, connfere se seu comprimento <= espessura maxima, se sim, ele adiciona na lista de candidatas
    for (int d = 0; d < 2; d++) {
        for (int l = 0; l < linhas; l++) {
            for (int c = 0; c < colunas; c++) {

                if (!ehCaminho(lab->matriz[l][c].valor)) continue;

                int nl = l + dl[d];
                int nc = c + dc[d];
                int comprimento = 0;

                while (nl >= 0 && nl < linhas && nc >= 0 && nc < colunas && lab->matriz[nl][nc].valor == '1') {
                    comprimento++;
                    nl += dl[d];
                    nc += dc[d];
                }

                if (comprimento >= 1 &&
                    comprimento <= espessuraParedeMax &&
                    nl >= 0 && nl < linhas &&
                    nc >= 0 && nc < colunas &&
                    ehCaminho(lab->matriz[nl][nc].valor)) {

                    if (totalCandidatas < capacidade) {
                        candidatas[totalCandidatas++] = (ParedeCandidata) {l, c, dl[d], dc[d], comprimento};
                    }
                }
            }
        }
    }

    // Desordena a lista de candidatas
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

        int nl = p.l + p.dl;
        int nc = p.c + p.dc;
        int comprimento = 0;

        while (nl >= 0 && nl < linhas && nc >= 0 && nc < colunas && lab->matriz[nl][nc].valor == '1') {
            comprimento++;
            nl += p.dl;
            nc += p.dc;
        }

        if (comprimento != p.comprimento ||
            comprimento > espessuraParedeMax ||
            nl < 0 || nl >= linhas ||
            nc < 0 || nc >= colunas ||
            !ehCaminho(lab->matriz[nl][nc].valor)) {
            continue;
        }

        for (int k = 1; k <= comprimento; k++) {
            int pl = p.l + k * p.dl;
            int pc = p.c + k * p.dc;
            lab->matriz[pl][pc].valor = '0';
        }

        abertas++;
    }

    free(candidatas);
    return abertas;
}

// Adiciona as Chaves ao Longo do Caminho Viavel (O maior caminho que sai do A e chega ate o X)
int adicionarChaves(Labirinto *lab, Posicao *caminho, int qtdPosicoes, int nChaves){
    if (lab == NULL || lab->matriz == NULL || caminho == NULL || qtdPosicoes <= 0 || nChaves <= 0) {
        return 0;
    }

    Posicao *candidatas = malloc(qtdPosicoes * sizeof(Posicao));
    if (candidatas == NULL) return 0;

    // Todas as celulas do caminho sao candidatas para uma chave
    for (int i = 0; i < qtdPosicoes; i++)
        candidatas[i] = caminho[i];

    for (int i = qtdPosicoes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Posicao temp = candidatas[i];
        candidatas[i] = candidatas[j];
        candidatas[j] = temp;
    }

    int adicionadas = 0;

    // Contato que o espaco no caminho escolhido seja 0 e seja diferente de A, X e C, ele vai receber uma chave
    for (int i = 0; i < qtdPosicoes && adicionadas < nChaves; i++) {
        int l = candidatas[i].linhaCaminho;
        int c = candidatas[i].colunaCaminho;

        if (lab->matriz[l][c].valor == 'A' ||
            lab->matriz[l][c].valor == 'X' ||
            lab->matriz[l][c].valor == 'C') {
            continue;
        }

        if (lab->matriz[l][c].valor == '0') {
            lab->matriz[l][c].valor = 'C';
            adicionadas++;
        }
    }

    free(candidatas);
    return adicionadas;
}

// Funcao para aleatorizar a posicao do inicio e do fim, caso solicitado no menu
void posicaoAleatoria(Posicao *posicao, int limiteLinha, int limiteColuna) {
    if (posicao->linhaCaminho < 0) {
        posicao->linhaCaminho = rand() % limiteLinha;
    }

    if (posicao->colunaCaminho < 0) {
        posicao->colunaCaminho = rand() % limiteColuna;
    }
}

// Funcao que controla o menu interativo da criacao de labirintos
void menu(Labirinto *labirinto, Posicao *inicio, Posicao *fim, int *nChaves){
    
    printf("Geracao de Mapa\n");

    while (1) {
        printf("Digite o numero de linhas e colunas (ambos maiores que 3): ");
        
        if (scanf("%d %d", &labirinto->totalLinhas, &labirinto->totalColunas) != 2) {
            printf("Entrada invalida. Digite apenas numeros inteiros.\n");
            while (getchar() != '\n');
            continue;
        }

        if (labirinto->totalLinhas > 3 && labirinto->totalColunas > 3) {
            break; 
        }

        printf("Erro: Os valores devem ser maiores que 3.\n");
    }

    printf("Digite a linha e a coluna iniciais (negativos para aleatorio): ");

    if (scanf("%d %d", &inicio->linhaCaminho, &inicio->colunaCaminho) != 2) {
        printf("Entrada invalida.\n");
        return;
    }

    while (
        inicio->linhaCaminho >= labirinto->totalLinhas ||
        inicio->colunaCaminho >= labirinto->totalColunas
    ) {
        printf("Posicao fora do labirinto. Digite novamente: ");

        if (scanf("%d %d", &inicio->linhaCaminho, &inicio->colunaCaminho) != 2) {
            printf("Entrada invalida.\n");
            return;
        }
    }

    posicaoAleatoria(inicio, labirinto->totalLinhas, labirinto->totalColunas);

    printf("Digite a linha e a coluna do bau (negativos para aleatorio): ");

    if (scanf("%d %d", &fim->linhaCaminho, &fim->colunaCaminho) != 2) {
        printf("Entrada invalida.\n");
        return;
    }

    while (
        fim->linhaCaminho >= labirinto->totalLinhas ||
        fim->colunaCaminho >= labirinto->totalColunas ||
        (
            fim->linhaCaminho == inicio->linhaCaminho &&
            fim->colunaCaminho == inicio->colunaCaminho
        )
    ) {
        printf("Posicao invalida ou igual a entrada. Digite novamente: ");

        if (scanf("%d %d", &fim->linhaCaminho, &fim->colunaCaminho) != 2) {
            printf("Entrada invalida.\n");
            return;
        }
    }

    posicaoAleatoria(
        fim,
        labirinto->totalLinhas,
        labirinto->totalColunas
    );

    // Se alguma coordenada do bau foi sorteada,
    // verifica novamente se nao coincide com a entrada.
    while (
        fim->linhaCaminho == inicio->linhaCaminho &&
        fim->colunaCaminho == inicio->colunaCaminho
    ) {
        fim->linhaCaminho =
            rand() % labirinto->totalLinhas;

        fim->colunaCaminho =
            rand() % labirinto->totalColunas;
    }

    printf("Quantas chaves havera no labirinto? ");

    if (scanf("%d", nChaves) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    while (*nChaves > ((labirinto->totalLinhas * labirinto->totalColunas) - 2)){
        printf("Quantidade superior ao espaco do labirinto. Digite novamente: ");
        if (scanf("%d", nChaves) != 1) {
            printf("Entrada invalida.\n");
            return;
        }
    }

    if (*nChaves < 0) {
        *nChaves = 0;
    }
}

// Funcao Chamada pelo main
int gerarMapa(Labirinto *lab){
    // Inicializa a semente para numeros aleatorios
    srand(time(NULL));

    Posicao inicio;
    Posicao fim;

    menu(lab, &inicio, &fim, &lab->totalChaves);

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
    int sucesso = gerarLabirinto(lab, inicio, fim, caminhoViavel, &qtdCaminho);

    if (!sucesso) {
        printf("Falha ao gerar o labirinto!\n");
    } else {

        //randomizerDaBorda(lab, inicio, fim);

        //Adicionar caminhos extras (deixar o labirinto menos linear / com loops)
        abrirCaminhosExtras(lab); // Ex: paredes de até tamanho 3, abre 5 extras

        // Adicionar Chaves ('C') ao longo do caminho viavel
        int chavesAdicionadas = adicionarChaves(lab, caminhoViavel, qtdCaminho, lab->totalChaves);
    }

    free(caminhoViavel);
}