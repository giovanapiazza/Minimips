#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    int reg[8];
} BancoRegistradores;

typedef struct {
    int tipo;         // 1 = R, 2 = I, 3 = J
    int opcode;
    int rs;
    int rt;
    int rd;
    int funct;
    int immediate;
    int address;
} Instrucao;

char mem_p[256][17];
char mem_d[256][9];
int pc = 0;

typedef struct {
    int pc;
    BancoRegistradores BR;
} Estado;

Estado antes[256];
int topo = -1; // posição do último estado salvo

void salvar_estado(int pc_atual, BancoRegistradores BR_atual) {
    topo++;
    antes[topo].pc = pc_atual;
    antes[topo].BR = BR_atual;
}

int restaurar_estado(int *pc, BancoRegistradores *BR) {
    if (topo < 0) {
        printf("Nao há instrucao anterior para voltar.\n");
        return 0;
    }
    *pc = antes[topo].pc;
    *BR = antes[topo].BR;
    topo--;
    return 1;
}

void unidadedeSaida(BancoRegistradores *BR) {
    printf("Conteudo do banco de registradores:\n");
    for (int i = 0; i < 8; i++) {
        printf("R%d: %d\n", i, banco_registradores->registradores[i]);
    }
}

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
        if(linha[0] == '\n') continue;

        char *pos; // Ponteiro para a posição do caractere de nova linha
        if ((pos = strchr(linha, '\n')) != NULL) {
            *pos = '\0'; 
        }
        strncpy(mem_p[i], linha, 16); // Copia a linha para a memória de programa
        printf("mem_p[%d]: %s\n", i, mem_p[i]);
        i++;
    }
    fclose(f);
    printf("Memoria de programa carregada com sucesso\n");
}


void carregarMemoriaDados(){
    char arquivo[50];
    printf("Digite o nome do arquivo que deseja abrir: ");
    scanf("%s", arquivo);

    FILE *f = fopen(arquivo, "r");
    if (f == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }
    int i = 0;
    char linha[9];
    while (fgets(linha, 9, f)) { 
        if(linha[0] == '\n') continue;

        char *pos; // Ponteiro para a posição do caractere de nova linha
        if ((pos = strchr(linha, '\n')) != NULL) {
            *pos = '\0'; 
        }
        strncpy(mem_d[i], linha, 9); // Copia a linha para a memória de dados
        printf("mem_d[%d]: %s\n", i, mem_d[i]);
        i++;
    }
    fclose(f);
    printf("Código carregado com sucesso na memória de dados!\n");
}

void ImprimirMemoriaDados(){
    for(int i = 0; i < 256; i++) {
        printf("%d: %s\n", i, mem_d[i]);
    }
}

void imprimirRegistradores(BancoRegistradores *BR) {
    const char *nomes[] = {"$zero", "$v0", "$a0", "$t0", "$t1", "$s0", "$s1", "$ra"};
    for (int i = 0; i < 8; i++) {
        printf("Registrador %s: %d\n", nomes[i], BR->reg[i]);
    }
}

Instrucao decod(char* inst) {
    Instrucao i;
    char buffer[17];

    strncpy(buffer, inst, 4);
    buffer[4] = '\0';
    i.opcode = strtol(buffer, NULL, 2);

    if (i.opcode == 0) {
        i.tipo = 1;
        strncpy(buffer, inst + 4, 3); buffer[3] = '\0';
        i.rs = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 7, 3); buffer[3] = '\0';
        i.rt = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 10, 3); buffer[3] = '\0';
        i.rd = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 13, 3); buffer[3] = '\0';
        i.funct = strtol(buffer, NULL, 2);
    } else if (i.opcode == 2) {
        i.tipo = 3;
        strncpy(buffer, inst + 4, 12); buffer[12] = '\0';
        i.address = strtol(buffer, NULL, 2);
    } else {
        i.tipo = 2;
        strncpy(buffer, inst + 4, 3); buffer[3] = '\0';
        i.rs = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 7, 3); buffer[3] = '\0';
        i.rt = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 10, 6); buffer[6] = '\0';
        int imm = strtol(buffer, NULL, 2);
        if (inst[10] == '1') imm -= 64;
        i.immediate = imm;
    }

    return i;
}

void imprimir_instrucao(Instrucao i) {
    printf("Tipo: %d | Opcode: %d | RS: %d | RT: %d | RD: %d | Funct: %d | Immediate: %d | Address: %d\n",
        i.tipo, i.opcode, i.rs, i.rt, i.rd, i.funct, i.immediate, i.address);
}

int ula(int a, int b, int op) {
    switch (op) {
        case 0: //add
            return a + b;
        case 1: //sub
            return a - b;
        case 2: //and
            return a & b;
        case 3: //or
            return a | b;
        default:
            printf("Operacao invalida.\n"); 
            return 0;
    }
}


int mux(int a, int b, int select) {
    if (select == 0) {
        return a;
    } else {
        return b;
    }
}

void conv_asm(char *bin_inst, FILE *arquivo_asm, Instrucao inst) {
    if(inst.rd == 0 && inst.rt == 0 && inst.rs == 0){
        return;
    }
    switch (inst.tipo) {
        case 1:
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
        case 2:
            switch (inst.opcode) {
                case 4:
                    fprintf(arquivo_asm, "addi $r%d, $r%d, %d", inst.rt, inst.rs, inst.immediate);
                    break;
                case 11:
                    fprintf(arquivo_asm, "lw $r%d, %d($r%d)", inst.rt, inst.immediate, inst.rs);
                    break;
                case 15:
                    fprintf(arquivo_asm, "sw $r%d, %d($r%d)", inst.rt, inst.immediate, inst.rs);
                    break;
                case 8:
                    fprintf(arquivo_asm, "beq $r%d, $r%d, %d", inst.rt, inst.rs, inst.immediate);
                    break;
                default:
                    fprintf(arquivo_asm, "Opcode I nao reconhecido: %d", inst.opcode);
                    break;
            }
            break;
        case 3:
            fprintf(arquivo_asm, "j %d", inst.address);
            break;
    }

}

void salvar_asm() {
    FILE *arquivo_asm = fopen("programa.asm", "w"); 
    if (arquivo_asm == NULL) {
        printf("Erro ao criar o arquivo\n");
        return;
    }
    for (int i = 0; i < 256; i++) {
        conv_asm(mem_p[i], arquivo_asm, decod(mem_p[i]));
        fprintf(arquivo_asm, "\n");
    }
    fclose(arquivo_asm);
    printf("Arquivo .asm salvo com sucesso!\n");
}

void salvar_data() {
    FILE *arquivo_memoria = fopen("programa.data", "w");
    if (arquivo_memoria == NULL) {
        printf("Erro ao criar o arquivo\n");
        return;
    }

    for (int i = 0; i < 256; i++) {
        fprintf(arquivo_memoria, "Endereco de memoria[%d]: %s\n", i, mem_d[i]);
    }

    fclose(arquivo_memoria);
    printf("Arquivo .data salvo com sucesso!\n");
}

void executar_instrucao(char* bin_instr, BancoRegistradores *BR) {
    salvar_estado(pc, *BR);
    Instrucao inst = decod(bin_instr);
    int pulou = 0;

    printf("inst: %s\n", bin_instr);
    printf("opcode: %d\n", inst.opcode);
    printf("tipo: %d\n", inst.tipo);
    printf("rs: %d\n", inst.rs);
    printf("rt: %d\n", inst.rt);
    printf("im: %d\n", inst.immediate);
    
    switch(inst.tipo) {
        case 1: // R
            if (inst.funct == 0) { // and
                BR->reg[inst.rd] = ula(BR->reg[inst.rs], BR->reg[inst.rt], 0);
            } else if (inst.funct == 1) { // or
                BR->reg[inst.rd] = ula(BR->reg[inst.rs], BR->reg[inst.rt], 1);
            } else if (inst.funct == 2) { // add
                BR->reg[inst.rd] = ula(BR->reg[inst.rs], BR->reg[inst.rt], 2);
            } else if (inst.funct == 3) { // sub
                BR->reg[inst.rd] = ula(BR->reg[inst.rs], BR->reg[inst.rt], 4);
            }
            break;
        
        case 2: // I
            if (inst.opcode == 8) { // beq
                if (BR->reg[inst.rs] == BR->reg[inst.rt]){
                    pc += (inst.immediate*2);
                    pulou = 1;
                }
            } else if (inst.opcode == 11) { // lw
                BR->reg[inst.rt] = strtol(mem_d[BR->reg[inst.rs] + inst.immediate], NULL, 10);
            } else if (inst.opcode == 15) { // sw
                int val = BR->reg[inst.rt];
                sprintf(mem_d[(BR->reg[inst.rs] + inst.immediate)], "%08d", val);
            } else if(inst.opcode == 4){
                BR->reg[inst.rt] = BR->reg[inst.rs];
            }
            break;
        case 3: // J
            pc = inst.address;
            break;
    }
    if (!pulou) {
        pc++; // Atualiza PC para a próxima instrução
    }
}

int main() {
    BancoRegistradores BR = {{0}};
    int c = 1;
    while (c) {
        int m;

        printf("\n MINI-MIPS 8 BITS - UNIPAMPA\n"); 
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
        
        switch(m){
            case 1: carregar memroia
                carregarMemoria();
                break;
            case 2: /carregar memoria de dados
                carregarMemoriaDados();
                break;
            case 3: //imprimir memoria
                ImprimirMemoria(); 
                break;
            case 4: //imprimir memoria de dados
                ImprimirMemoriaDados(); 
                break;
            case 5: //imprimir registradores
                unidadedeSaida(&BR); 
                break;
            case 6: // imprimir tudo
                ImprimirMemoria(); 
                ImprimirMemoriaDados();
                unidadedeSaida(&BR);
                break;
            case 7: //Salvar .asm
                salvar_asm();
                break;
            case 8: //Salvar .data
                salvar_data();
                break;
            case 9: //run
                while (pc < 256 && strlen(mem_p[pc]) > 0) {
                    executar_instrucao(mem_p[pc], &BR);
                }
                break;
            case 10: //step
                if (strlen(mem_p[pc]) > 0)
                    executar_instrucao(mem_p[pc], &BR);
                break;
            case 11: //back
                if (restaurar_estado(&pc, &BR)){
                    printf("Instrucao anterior restaurada. PC = %d\n", pc);
                }
                break;
            case 0:
                printf("Programa finalizado.\n");
                c = 0;
                break;
            default:
                printf("Opcao invalida.\n");
        }
    }
    return 0;
}
