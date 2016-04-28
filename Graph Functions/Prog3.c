/*
* Antonio Ricardo Alexandre Brasil
* Matematica Discreta 2014-2
* Prof: Jefferson Andrade
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/*
* Estruturas das matrizes.
* Preferi por nao criar a biblioteca.
* Dado que a estrutura da lista foi necessaria pois foi preciso criar uma lista IN que contem os nos percorridos, no caso.
*
*/
struct matriz {
	int linhas;
	int colunas;
	int* vetor;
}; typedef struct matriz Matriz;

struct lista {
    int valor;
    struct lista* prox;
}; typedef struct lista Lista;

// Apenas por elegancia
#define nos int

#ifndef max
    #define max(a,b) (((a) (b)) ? (a) : (b))
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
Lista* cria_lista(void){
    /*
    * Cria uma lista.
    */
    return NULL;
}
Lista* insere_lista(Lista* l, int v) {
    /*
    * Insere um valor na lista simplesmente encadeada.
    */
    Lista* novo = (Lista*) malloc(sizeof(Lista));
    novo->prox = l;
    novo->valor=v;
    return novo;
}
void imprime_lista(Lista* l) {
    /*
    * Imprime na lista.
    */
    Lista* aux = l;
    while(aux != NULL) {
        printf("%d ", aux->valor);
        aux = aux->prox;
    }
}
void libera_lista(Lista* l) {
    /*
    * Libera a lista.
    */
    Lista* aux = l;
    while(aux != NULL) {
        Lista* ant = aux->prox;
        free(aux);
        aux = ant;
    }
}
int busca_lista(Lista* l, int v) {
    /*
    * Percorre a busca na lista.
    */
    Lista* aux = l;
    for(aux = l; aux != NULL; aux = aux->prox ){
        if(aux->valor == v) {
            return 1;
        }
    }
    return 0;
}
// ------------------------------------
Matriz* cria(int m, int n) {
    /*
    * Aloca a matriz na memoria de tamanho mXn
    */
    Matriz* aux = (Matriz*) malloc(sizeof(Matriz)); // Aloca a memoria
    if(aux == NULL){ // Verifica se tem espa?o
        printf("Erro ao alocar memoria");
        exit(1);
    }
    aux->linhas = m;
    aux->colunas = n;
    aux->vetor = (int*) malloc(m*n*sizeof(int)); // matriz representada por um vetor simples

    return aux;
}
/*
* Retorna as linhas e colunas da matriz
*/
int linhas(Matriz* mat) {
    return (mat->linhas);
}
int colunas(Matriz* mat) {
    return (mat->colunas);
}

int acessa(Matriz* mat, int i, int j)
{
    /*
    * Acessa um valor na l e coluna da matriz.
    */
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j;
    return mat->vetor[k];
 }

void atribui(Matriz* mat, int i, int j, int v)
{
    /*
    * Atribui um valor a matriz.
    */
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j; // Indice
    mat->vetor[k] = v;
}

void libera (Matriz* mat)
{
    /*
    * Libera a matriz na memoria.
    */
    free(mat->vetor);
    free(mat);
}
void imprime(Matriz* mat, int tamanho) {
    /*
    * Imprime uma matriz com formatacao
    */
    int i = 0;
    int j = 0;
    printf("  ");
    for(i=0;i<tamanho;i++){
        printf(" %d",i+1);
    }
    printf("\n");
    for(i=0;i<tamanho;i++){
        printf("%d| ",i+1);
        for(j=0;j<tamanho;j++){
            printf("%d ", acessa(mat, i, j));
        }
        printf("|\n");
    }
}

Matriz* criaMatrizTransicoes(Matriz* matrizPesos, char arquivo[], int tamanho) {
    /*
    * Le o arquivo contendo os nos e as transicoes para criar a matriz de transicao
    */
    FILE *fp;
    fp = fopen(arquivo,"r");   /* Arquivo ASCII, para leitura */
    if(!fp) {
        printf( "Erro na abertura do arquivo");
        exit(0);
    }
    Matriz* matriz = cria(tamanho,tamanho);
    int i,j, peso;
    fscanf(fp,"%d",&i);
    for(i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++) {
            atribui(matriz, i, j, 0);
            atribui(matrizPesos, i, j, 999999);
        }
    }
    while(1){
        fscanf(fp,"%d %d %d",&i,&j, &peso);
        atribui(matrizPesos, i-1, j-1, peso);
        atribui(matrizPesos, j-1, i-1, peso);
        if(acessa(matriz, i-1, j-1) == 0) {
            atribui(matriz, i-1, j-1, 1);
            atribui(matriz, j-1, i-1, 1);
        } else {
            atribui(matriz, i-1, j-1, 2);
            atribui(matriz, j-1, i-1, 2);
        }
        if (feof(fp))
            break;
    }
    fclose(fp);
    return matriz;
}

int retornaTamanho(char arquivo[]){
    /*
    *   Retorna o tamanho da matriz, dado que a primeira linha eh o tamanho.
    */
    int tamanhoMatriz;
    FILE *fp;
    fp = fopen(arquivo,"r");   /* Arquivo ASCII, para leitura */
    if(!fp) {
        printf( "Erro na abertura do arquivo");
        exit(0);
    }

    fscanf(fp, "%d", &tamanhoMatriz);
    return tamanhoMatriz;
}
void caminhoMinimo(Matriz* matrizTransicao, Matriz* matrizPesos, int tamanho, nos x, nos y) {
    /*
    *   Função do caminho minimo dijkstra.
    *   Parametros: matrizTransição, matrizPesos, tamanho da Matris e os nos, no caso, y e onde eu quero chegar
    */
    int index;
    x = x-1;
    y = y-1;

    Lista* in = cria_lista();
    nos z, p;
    int *d = malloc(tamanho*sizeof(int));
    int *s = malloc(tamanho*sizeof(int));
    for(index = 0; index < tamanho; index ++) {
        d[index] = 0;
        if(index != x) {
            s[index] = x;
        } else {
            s[index] = 0;
        }
    }
    int distanciaAnterior;
    // inicializa o conjunto IN e os vetores d e s
    in = insere_lista(in, x);
    d[x] = 0;
    for(index = 0; index < tamanho; index ++) {
        if(busca_lista(in, index) == 0) {
            d[index] = acessa(matrizPesos, x, index);
        }
    }

    while(busca_lista(in, y) == 0) {
        p = menor_distancia(d, in, tamanho);
        in = insere_lista(in, p);
        for(index = 0; index < tamanho; index ++) {
            if(busca_lista(in, index) == 0) {
                distanciaAnterior = d[index];
                d[index] = min(d[index], d[p]+acessa(matrizPesos, p, index));
                if(d[index] != distanciaAnterior) {
                    s[index] = p;
                }
            }
        }
    }
    z = y;
    Lista* lista = cria_lista();
    while(z != x) {
        lista = insere_lista(lista, z+1);
        z = s[z];
    }
    lista = insere_lista(lista, x+1);

    imprime_lista(lista);
    libera_lista(lista);
    libera_lista(in);
    free(d);
    free(s);
}
int menor_distancia(int* d, Lista* in, int tamanho) {
    int i = 0;
    int menor = INT_MAX;
    int menorindice = tamanho+1;
    for(i = 0; i < tamanho; i++) {
        if(busca_lista(in, i) == 0) {
            if(d[i] < menor) {
                menor = d[i];
                menorindice = i;
            }
        }
    }
    return menorindice;
}
int main(int argc, char *argv[]) {
    int num1, num2;
    num1 = atoi(argv[2]);
    num2 = atoi(argv[3]);

    Matriz* matrizTransicoes;
    Matriz* matrizPesos;
    char arquivo[128];
    strcpy(arquivo, argv[1]);
    int tamanho = retornaTamanho(arquivo);
    matrizPesos = cria(tamanho, tamanho);
    matrizTransicoes = criaMatrizTransicoes(matrizPesos, arquivo, tamanho);
    caminhoMinimo(matrizTransicoes, matrizPesos, tamanho, num1, num2);

    libera(matrizTransicoes);
    libera(matrizPesos);
    system("exit");
}

