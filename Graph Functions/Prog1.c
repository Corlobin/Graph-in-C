/*
* Antonio Ricardo Alexandre Brasil
* Matematica Discreta 2014-2
* Prof: Jefferson Andrade
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
* Estruturas das matrizes.
* Preferi por nao criar a biblioteca.
*/
struct matriz {
	int linhas;
	int colunas;
	int* vetor;
}; typedef struct matriz Matriz;

Matriz* cria(int m, int n) {
    // Cria uma matriz mXn
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
// Retorna linhas e colunas..
int linhas(Matriz* mat) {
    return (mat->linhas);
}
int colunas(Matriz* mat) {
    return (mat->colunas);
}
// Acessa a matriz
int acessa(Matriz* mat, int i, int j)
{
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j;
    return mat->vetor[k];
 }
//Atribui um valor na matriz
void atribui(Matriz* mat, int i, int j, int v)
{
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j; // Indice
    mat->vetor[k] = v;
}
// Libera a matriz
void libera (Matriz* mat)
{
    free(mat->vetor);
    free(mat);
}
void imprime(Matriz* mat, int tamanho) {
    // Imprime a matriz formatada
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

Matriz* criaMatrizTransicoes(char arquivo[], int tamanho) {
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
    int i,j;
    fscanf(fp,"%d",&i);
    for(i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++) {
            atribui(matriz, i, j, 0);
        }
    }
    while(1){
        fscanf(fp,"%d %d",&i,&j);
        atribui(matriz, i-1, j-1, 1);
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
Matriz* criaMatrizAcessibilidade(Matriz* matrizTransicao, int tamanho) {
    Matriz* mat = cria(tamanho, tamanho);
    int i, j, k;
    for(i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++) {
            atribui(mat, i, j, acessa(matrizTransicao, i,j));
        }
    }
    for(k = -1; k < tamanho-1; k++) {
        for(i = 0; i < tamanho; i++) {
            for(j = 0; j < tamanho; j++) {
                int valorLogico = (acessa(mat, i, j) || (acessa(mat, i, k+1) && acessa(mat, k+1, j)));
                atribui(mat, i, j, valorLogico);
            }

        }
    }
    return mat;
}

int main(int argc, char *argv[]) {
    Matriz* matrizTransicoes;
    Matriz* matrizAcessibilidade;

    char arquivo[128];
    strcpy(arquivo, argv[1]);
    int tamanho = retornaTamanho(arquivo);

    matrizTransicoes = criaMatrizTransicoes(arquivo, tamanho);
    //printf("Matriz transicao:\n");
    //imprime(matrizTransicoes, tamanho);

    matrizAcessibilidade = criaMatrizAcessibilidade(matrizTransicoes, tamanho);
    imprime(matrizAcessibilidade, tamanho);



    libera(matrizTransicoes);
    libera(matrizAcessibilidade);
    system("exit");
}
