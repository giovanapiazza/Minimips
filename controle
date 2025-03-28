#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char dados[9];
} MemoriaDados;

void carregarMemoria(MemoriaDados *memdados, const char *ArquivoMemoria){
    int i;
    FILE *arquivo = fopen("arquivomem.txt", "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", ArquivoMemoria);
        return;
    }
    printf("Carregando memória de dados...\n");
    for (i = 0; i < 256; i++){
        if (fgets(memdados[i].dados, 9, arquivo) == NULL){
            break;
        }
        memdados[i].dados[strcspn(memdados[i].dados, "\n")] = '\0';
    }
    fclose(arquivo);
    printf("Código carregado com sucesso na memória de dados!\n");
}

int main(){
    int i;
    MemoriaDados memoria[256];
    carregarMemoria(memoria, "arquivomem.txt");

    printf("\nConteúdo da memória de dados:\n");
    for (i = 0; i < 256 && memoria[i].dados[0] != '\0'; i++) {
        printf("Linha %d: %s\n", i, memoria[i].dados);
    }

    return 0;
}
