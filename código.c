#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#define INF 100 

void separador() {
    printf("------------------------------------------\n");
}

void separadoralternativa() {
    printf("==========================================\n");
}


void BFS(int inicio, int **matriz, int n) {
    int fila[100], visitado[100] = {0};
    int frente = 0, tras = 0;
    fila[tras++] = inicio;
    visitado[inicio] = 1;

    while (frente < tras) {
        int atual = fila[frente++];
        printf("%d ", atual);
        for (int i = 0; i < n; i++) {
            if (matriz[atual][i] != 0 && !visitado[i]) {
                fila[tras++] = i;
                visitado[i] = 1;
            }
        }
    }
}


void DFS(int v, int visitado[], int **matriz, int n) {
    visitado[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < n; i++) {
        if (matriz[v][i] != 0 && !visitado[i]) {
            DFS(i, visitado, matriz, n);
        }
    }
}

typedef struct No {
    int vertice;
    int peso;
    struct No* prox;
} No;


void adicionarArestaLista(No* lista[], int origem, int destino, int peso) {
    No* novo = malloc(sizeof(No));
    novo->vertice = destino;
    novo->peso = peso;
    novo->prox = lista[origem];
    lista[origem] = novo;
}


void imprimirListaAdjacencia(No* lista[], int n_vertices) {
    printf("Lista de Adjac�ncia:\n");
    for (int i = 0; i < n_vertices; i++) {
        printf("V�rtice %d:", i);
        No* atual = lista[i];
        while (atual != NULL) {
            printf(" -> %d(peso=%d)", atual->vertice, atual->peso);
            atual = atual->prox;
        }
        printf("\n");
    }
}

int menorVertice(int *chave, int *incluso, int n) {
    int min = INF, min_index;

    for (int v = 0; v < n; v++)
        if (!incluso[v] && chave[v] < min) {
            min = chave[v];
            min_index = v;
        }

    return min_index;
}


int main(){

    setlocale(LC_ALL, "Portuguese");
    
    FILE *arquivo;
    int num_vertices, num_conexoes;
    char tipo;
    int valorado;

    arquivo = fopen("grafo.txt", "r");

     separador();
    if (arquivo == NULL) {
        separador();
        perror("Erro ao abrir o arquivo");
        separador();
        return 1;
    }

    fscanf(arquivo, "%d %d %c %d", &num_vertices, &num_conexoes, &tipo, &valorado);


    separador();
    printf("===Dados do grafo lido no arquivo===\n");
    printf("- Vertices: %d\n", num_vertices);
    printf("- Arestas: %d\n", num_conexoes);
    printf("- Tipo: %c\n ", tipo);
    printf("- Valorado: %s\n", valorado ? "Sim" : "N�o");
    separador();

    printf("Arestas ou arcos lidos:\n");
    for (int i = 0; i < num_conexoes; i++) {
        int vi, vj, peso;

        if (valorado) {
            fscanf(arquivo, "%d %d %d", &vi, &vj, &peso);
            printf("%d %d [%d]\n", vi, vj, peso);
        } else {
            fscanf(arquivo, "%d %d", &vi, &vj);
            printf("%d %d [-]\n", vi, vj);
        }
    }

    fclose(arquivo);


    separadoralternativa();


    FILE *arquivo_usuario;
    int tipo_usuario, vertices_usuario, conexoes_usuario, valorado_usuario;
    int origem[100], destino[100], peso[100];

    printf("Digite o n�mero de v�tices\n");
    printf("Obs: O n�mero de v�rtices deve ser maior que 0\n");
    separador();
    scanf("%d", &vertices_usuario);
    separador();
    printf("Digite o n�mero de arestas ou arcos:\n");
    separador();
    scanf("%d", &conexoes_usuario);
    separador();
    printf("Digite o tipo de grafo\n1 - Direcionado\n2 - N�o direcionado\n");
    separador();
    scanf("%d", &tipo_usuario);
    separador();
    printf("O grafo � valorado?\n1 - Sim\n0 - N�o\n");
    separador();
    scanf("%d", &valorado_usuario);
    separador();

    for (int i = 0; i < conexoes_usuario; i++) {
        printf("Conex�o %d:\n", i + 1);
        printf("v�rtice de origem: ");
        scanf("%d", &origem[i]);
        printf("v�rtice de destino: ");
        scanf("%d", &destino[i]);
        printf("\n");
        if (valorado_usuario == 1) {
            printf("Peso da aresta ou arco: ");
            scanf("%d", &peso[i]);
            separador();
        } else {
            peso[i] = 0;
            separador();
        }
    }

    arquivo_usuario = fopen("arquivoUsuario.txt", "w");
    if (arquivo_usuario == NULL) {
        separador();
        perror("Erro ao abrir o arquivo");
        separador();
        return 1;
    }

    fprintf(arquivo_usuario, "%d %d %c %d\n", vertices_usuario, conexoes_usuario, tipo_usuario == 1 ? 'D': 'G', valorado_usuario);
    for (int i = 0; i < conexoes_usuario; i++) {
        if (valorado_usuario == 1) {
            fprintf(arquivo_usuario, "%d %d %d\n", origem[i], destino[i], peso[i]);
        } else {
            fprintf(arquivo_usuario, "%d %d\n", origem[i], destino[i]);
        }
    }

    separador();
    printf("Dados do grafo gravados no arquivo com sucesso!\n");
    fclose(arquivo_usuario);
    separadoralternativa();



    FILE *arquivo_matriz;
    arquivo_matriz = fopen("arquivoUsuario.txt", "r");
    if (arquivo_matriz == NULL) {
        separador();
        perror("Erro ao abrir o arquivo");
        separador();
        return 1;
    }

    int n_vertices, n_arestas, valorado_grafo;
    char tipo_grafo;
    fscanf(arquivo_matriz, "%d %d %c %d", &n_vertices, &n_arestas, &tipo_grafo, &valorado_grafo);

    int **matriz = (int **)malloc(n_vertices * sizeof(int *));
    for (int i = 0; i < n_vertices; i++) {
        matriz[i] = (int *)malloc(n_vertices * sizeof(int));
        for (int j = 0; j < n_vertices; j++) {
            matriz[i][j] = 0;
        }
    }

    for (int i = 0; i < n_arestas; i++) {
        int vi, vf, peso_aresta = 1;
        if (valorado_grafo) {
            fscanf(arquivo_matriz, "%d %d %d", &vi, &vf, &peso_aresta);
        } else {
            fscanf(arquivo_matriz, "%d %d", &vi, &vf);
        }
        matriz[vi][vf] = peso_aresta;
        if (tipo_grafo == 'G') {
            matriz[vf][vi] = peso_aresta;
        }
    }

    fclose(arquivo_matriz);

    FILE *matriz_final;
    matriz_final = fopen("matriz.txt", "w");
    if (matriz_final == NULL) {
        separador();
        perror("Erro ao abrir o arquivo");
        separador();
        return 1;
    }

    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            fprintf(matriz_final, "%d ", matriz[i][j]);
        }
        fprintf(matriz_final, "\n");
    }
    fclose(matriz_final);

    separador();
    printf("Matriz salva no arquivo matriz.txt com sucesso!\n");
    separadoralternativa();



    int vertices_grau, conexoes_grau, valorado_grau;
    char tipo_grau;
    int origem_grau[100], destino_grau[100], peso_grau[100];

    FILE *arquivo_grau = fopen("arquivoUsuario.txt", "r");
    if (arquivo_grau == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fscanf(arquivo_grau, "%d %d %c %d", &vertices_grau, &conexoes_grau, &tipo_grau, &valorado_grau);

    for (int i = 0; i < conexoes_grau; i++) {
        if (valorado_grau == 1) {
            fscanf(arquivo_usuario, "%d %d %d", &origem_grau[i], &destino_grau[i], &peso_grau[i]);
        } else {
            fscanf(arquivo_usuario, "%d %d", &origem_grau[i], &destino_grau[i]);
            peso_grau[i] = 0;
        }
    }

    fclose(arquivo_usuario); 

    separador();

    if (tipo_grau == 'D') {

        int grauEntrada[100] = {0};
        int grauSaida[100] = {0};

        for (int i = 0; i < conexoes_grau; i++) {
            grauSaida[origem_grau[i]]++;
            grauEntrada[destino_grau[i]]++;
        }

        printf("Graus dos v�rtices (grafo direcionado):\n");
        for (int i = 0; i < vertices_grau; i++) {
            printf("V�rtice %d: Grau = %d\n", i, grauSaida[i]);
        }

    } else {

        int grau[100] = {0};

        for (int i = 0; i < conexoes_grau; i++) {
            grau[origem_grau[i]]++;
            grau[destino_grau[i]]++;
        }

        printf("Graus dos v�rtices (grafo n�o direcionado):\n");
        for (int i = 0; i < vertices_grau; i++) {
            printf("V�rtice %d: Grau = %d\n", i, grau[i]);
        }
    }

    separadoralternativa();
    

    No* lista[n_vertices];
    for (int i = 0; i < n_vertices; i++) {
        lista[i] = NULL;
    }

    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            if (matriz[i][j] != 0) {
                adicionarArestaLista(lista, i, j, matriz[i][j]);
            }
        }
    }


    separador();
    printf("Lista de Adjac�ncia gerada a partir da matriz:\n");
    imprimirListaAdjacencia(lista, n_vertices);
    separador();

    for (int i = 0; i < n_vertices; i++) {
        No* atual = lista[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }

    for (int i = 0; i < n_vertices; i++) {
        free(matriz[i]);
    }


    int chave[100], pai[100];
    int incluido[100] = {0};
    for (int i = 0; i < n_vertices; i++) {
        chave[i] = INF;
        incluido[i] = 0;
    }
    chave[0] = 0;
    pai[0] = -1;
    for (int count = 0; count < n_vertices - 1; count++) {
        int u = menorVertice(chave, incluido, n_vertices);
        incluido[u] = 1;

        for (int v = 0; v < n_vertices; v++) {
            if (matriz[u][v] && !incluido[v] && matriz[u][v] < chave[v]) {
                pai[v] = u;
                chave[v] = matriz[u][v];
            }
        }
    }
    printf("Arestas da �rvore Geradora M�nima (Prim):\n");
    for (int i = 1; i < n_vertices; i++) {
        printf("%d - %d  peso: %d\n", pai[i], i, matriz[i][pai[i]]);
    }
    separadoralternativa();



        printf("C�lculo de caminho mais curto:\n");

    int origem_dijkstra;
    printf("\nDigite o v�rtice de origem para o algoritmo de Dijkstra (0 a %d): ", n_vertices - 1);
    scanf("%d", &origem_dijkstra);

    int dist[n_vertices], visitado[n_vertices], anterior[n_vertices];


    for (int i = 0; i < n_vertices; i++) {
        dist[i] = INF;
        visitado[i] = 0;
        anterior[i] = -1;
    }

    dist[origem_dijkstra] = 0;

    for (int count = 0; count < n_vertices - 1; count++) {
        int min = INF, u = -1;
        for (int v = 0; v < n_vertices; v++) {
            if (!visitado[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break;

        visitado[u] = 1;

        for (int v = 0; v < n_vertices; v++) {
            if (!visitado[v] && matriz[u][v] && dist[u] + matriz[u][v] < dist[v]) {
                dist[v] = dist[u] + matriz[u][v];
                anterior[v] = u;
            }
        }
    }

    separador();  
    printf("Caminhos m�nimos a partir do v�rtice %d:\n", origem_dijkstra);
    for (int i = 0; i < n_vertices; i++) {
        printf("Destino %d | Custo: %d | Caminho: ", i, dist[i]);

        if (dist[i] == INF) {
            printf("inacess�vel\n");
            continue;
        }

        int caminho[100], tam = 0, v = i;
        while (v != -1) {
            caminho[tam++] = v;
            v = anterior[v];
        }

        for (int j = tam - 1; j >= 0; j--) {
            printf("%d", caminho[j]);
            if (j != 0) printf(" -> ");
        }
        printf("\n");
    }
    separadoralternativa();

    separadoralternativa();
    printf("Busca em Profundidade (DFS):\n");
    int visitado_dfs[100] = {0};
    DFS(0, visitado_dfs, matriz, n_vertices);
    printf("\n");

    separador();
    separadoralternativa();
    printf("Busca em Largura (BFS):\n");
    BFS(0, matriz, n_vertices);
    printf("\n");


    return 0;
}
