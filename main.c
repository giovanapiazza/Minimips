#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "minimips.h"


int menu() {
    int m;
    printf("\t MINI-MIPS 8 BITS - UNIPAMPA\n"); 
    printf("1. Carregar memoria\n"); 
    printf("2. Carregar memoria de dados\n");
    printf("3. Imprimir memoria \n");
    printf("4. Imprimir memoria de dados\n");
    printf("5. Imprimir registradores \n"); 
    printf("6. Imprimir todo o simulador \n"); 
    printf("7. Salvar .asm \n"); 
    printf("8. Salvar .data \n"); 
    printf("9. Executa Programa (run)\n"); 
    printf("10. Executa uma instrucao (Step)\n"); 
    printf("11. Volta uma instrucao (Back)\n"); 
    printf("0. Sair \n"); 
    printf("Escolha uma opção: "); 
    setbuf(stdin, NULL);
    scanf("%d", &m);

    return m;
}

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

void carregarMemoriaDados() {
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

int ula(int a, int b, int op) { 
    switch (op) { 
        case 0: return a & b;  
        case 1: return a | b;  
        case 2: return a + b;  
        case 3: return a - b;  
        default: printf("Operação não reconhecida: %d\n", op); return 0; 
    } 
} 

int mux(int a, int b, int select) {
    return (select == 0) ? a : b;
}


//conversor
void salvar_asm() {
    FILE *arquivo_asm = fopen("programa.asm", "w");
    if (arquivo_asm == NULL) {
        printf("Erro ao criar o arquivo\n");
        return;
    }

    for (int i = 0; i <[256][17]; i++) {
        converter_asm(memoria_instrucao[i], arquivo_asm, codificarInstrucao(memoria_instrucao[i]));
        fprintf(arquivo_asm, "\n");
    }
    fclose(arquivo_asm);
    printf("Arquivo .asm salvo com sucesso!\n");
}

void converter_asm(char instrucao_binaria[256][17], FILE *arquivo_asm, Instrucao inst) {
    if(inst.rd == 0 && inst.rt == 0 && inst.rs == 0){
        return;
    }
    switch (inst.tipo) {
        case R_TYPE:
            switch (inst.funct) {
                case 0:
                    fprintf(arquivo_asm, "add $r%d, $r%d, $r%d", inst.rd, inst.rs, inst.rt);
                    break;
                case 2:
                    fprintf(arquivo_asm, "sub $r%d, $r%d, $r%d", inst.rd, inst.rs, inst.rt);
                    break;
                case 4:
                    fprintf(arquivo_asm, "and $r%d, $r%d, $r%d", inst.rd, inst.rs, inst.rt);
                    break;
                case 5:
                    fprintf(arquivo_asm, "or $r%d, $r%d, $r%d", inst.rd, inst.rs, inst.rt);
                    break;
                default:
                    fprintf(arquivo_asm, "Funcao R nao reconhecida: %d", inst.funct);
                    break;
            }
            break;
        case I_TYPE:
            switch (inst.opcode) {
                case 4:
                    fprintf(arquivo_asm, "addi $r%d, $r%d, %d", inst.rt, inst.rs, inst.imm);
                    break;
                case 11:
                    fprintf(arquivo_asm, "lw $r%d, %d(R%d)", inst.rt, inst.imm, inst.rs);
                    break;
                case 15:
                    fprintf(arquivo_asm, "sw $r%d, %d($r%d)", inst.rt, inst.imm, inst.rs);
                    break;
                case 8:
                    fprintf(arquivo_asm, "beq $r%d, $r%d, %d", inst.rt, inst.rs, inst.imm);
                    break;
                default:
                    fprintf(arquivo_asm, "Opcode I nao reconhecido: %d", inst.opcode);
                    break;
            }
            break;
        case J_TYPE:
            fprintf(arquivo_asm, "j %d", inst.addr);
            break;
    }

}
int check_overflow(int result) {
    if (result < -128 || result > 127) {
        return 1;
    }
    return 0;

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
