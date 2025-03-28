#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

char mem_p[256][17];
int pc = 0;

void carregarMemoria() {
    char arquivo[50];
    printf("Digite o nome do arquivo que deseja abrir: ");
    scanf("%s", arquivo);

    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    int i = 0;
    char linha[17];
    while (fgets(linha, 16 + 1, f)) { 
        if((strcmp(linha, "\n")) == 1){
            char *pos; // Ponteiro para a posição do caractere de nova linha
            if ((pos = strchr(linha, '\n')) != NULL) {
                *pos = '\0'; 
            }
            strncpy(mem_p[i], linha, 16); // Copia a linha para a memória de programa
            printf("mem_p[%d]: %s\n", i, mem_p[i]);
            i++;
        }
    }
    fclose(f);
    printf("Memoria de programa carregada com sucesso\n");
}


void decod(char* inst){

    char opcode[5];
    char rs[4];
    char rt[4];
    char rd[4];
    char funct[4];
    char imed[7];
    char addr[13];

    strncpy(opcode, inst, 4);
    opcode[4] = '\0';

    // TIPO R
    if((strcmp("0000\0", opcode) == 0)){
        printf("tipo r");
        strncpy(rs, inst+4, 3);
        rs[3] = '\0';
        strncpy(rt, inst+7, 3);
        rt[3] = '\0';
        strncpy(rd, inst+10, 3);
        rd[3] = '\0';
        strncpy(funct, inst+13, 3);
        rd[3] = '\0';
    }
    // TIPO J
    else if((strcmp("0010\0", opcode) == 0)){
        printf("tipo j");
        strncpy(addr, inst+4, 12);
        addr[13] = '\0';
    }
    
    // TIPO I
    else{
        printf("tipo i");
        strncpy(rs, inst+4, 3);
        rs[3] = '\0';
        strncpy(rt, inst+7, 3);
        rt[3] = '\0';
        strncpy(imed, inst+10, 6);
        imed[6] = '\0';
    }
    


    
    printf("INSTRUCAO: %s\n", inst);
    printf("OPCODE: %s\n", opcode);
    printf("RS: %s\n", rs);
    printf("RT: %s\n", rt);
    printf("RD: %s\n", rd);
    printf("FUNCT: %s\n", funct);
    printf("IMED: %s\n", imed);
    printf("ADDR: %s\n", addr);

    pc++;
}

int main(){

    carregarMemoria();
    for(int i=0;i<6;i++){

        printf("%d: %s\n", pc, mem_p[pc]);
        pc++;
    }
    pc = 0;
    decod(mem_p[pc]);
    return 0;

}