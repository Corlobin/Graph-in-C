/*
* Antonio Ricardo Alexandre Brasil
* Matematica Discreta 2014-2
* Prof: Jefferson Andrade
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*
*/
struct matriz {
	int linhas;
	int colunas;
	int* vetor;
};
typedef struct matriz Matriz;

Matriz* cria(int m, int n) {
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

int linhas(Matriz* mat) {
    return (mat->linhas);
}
int colunas(Matriz* mat) {
    return (mat->colunas);
}

int acessa(Matriz* mat, int i, int j)
{
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j;
    return mat->vetor[k];
 }

void atribui(Matriz* mat, int i, int j, int v)
{
    int k;
    int total_colunas = mat->colunas;
    k = i*total_colunas + j; // Indice
    mat->vetor[k] = v;
}

void libera (Matriz* mat)
{
    free(mat->vetor);
    free(mat);
}
void imprime(Matriz* mat, int tamanho) {
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

void caminhoEuler(Matriz* matrizTransicoes, int tamanho) {
    int total;
    int grau;
    int i, j;

    total = 0;
    i = 0;
    while(total <= 2 && i < tamanho) {
        grau = 0;
        for(j = 0; j < tamanho; j++) {
            grau = grau + acessa(matrizTransicoes, i, j);
        }
        if (grau%2 != 0) {
            total += 1;
        }
        i++;
    }

    printf("Total: %d\n", total);
    if(total > 2){
        printf("Não existe um caminho de euler.\n");
    } else {
        printf("Existe um caminho de euler.\n");
    }
}
int main(int argc, char *argv[]) {
    Matriz* matrizTransicoes;

    char arquivo[128];
    strcpy(arquivo, argv[1]);
    int tamanho = retornaTamanho(arquivo);

    matrizTransicoes = criaMatrizTransicoes(arquivo, tamanho);
    printf("Matriz transicao:\n");
    imprime(matrizTransicoes, tamanho);

    caminhoEuler(matrizTransicoes, tamanho);



    libera(matrizTransicoes);
    system("exit");
}

