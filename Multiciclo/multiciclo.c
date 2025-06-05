#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "multiciclo.h"

nodo *alocaNodo(){
    nodo *novo = (nodo *) malloc(sizeof(nodo));
    if(!novo){
        printf("Erro ao alocar nodo");
    }else{
        novo->prox = NULL;
        return novo;
    }
}

void liberanodo(nodo *p){
    if(!estaVazia(p)){ // verifica se a pilha não está vazia
        nodo *proxNodo, *atual;

        atual = p->prox;
        while(atual != NULL){
            proxNodo = atual->prox;
            liberanodo(atual); // libera recursivamente
            atual = proxNodo;
        }
    }
}

int estaVazia(nodo *p){ // verifica se a pilha está vazia
    if(p->prox == NULL) return 1;

    else return 0;
}

void push(nodo *p, PC *pPc, Memoria *pMem, RegIntermediario *pReg_intermed, UnidadeControle *pUc, BancoRegistradores *pBanco, int pEstdo_atual){ \\inserir um novo e salvar

    nodo *novo = alocaNodo(); // cria novo nodo
    novo->prox = NULL;
    novo->pPc = *pPc;  // copia o estado atual para o nodo
    novo->pMem = *pMem;
    novo->pReg_intermed = *pReg_intermed;
    novo->pUc = *pUc;
    novo->pBanco = *pBanco;
    novo->pEstado_atual = pEstdo_atual;

    if(estaVazia(p)){ // insere no fim da pilha
        p->prox = novo;
    } else{
        nodo *tmp = p->prox;
        while(tmp->prox != NULL){
            tmp = tmp->prox;
        }
        tmp->prox = novo;
    }
}

nodo *pop(nodo *p){ // remove o último elemento da pilha
    if(p->prox == NULL){
        return NULL;
    }else{
        nodo *ult = p->prox, *penult = p;
        
        while(ult->prox != NULL){
            penult = ult;
            ult = ult->prox;
        }
        
        penult->prox = NULL;
        return ult;
    }
}


void estado_0(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco, nodo *p){ //busca
    
    strncpy(reg_intermed->ri, mem->addr[pc->endereco_atual].instrucao, 17); // copia a instrução para o ri
    pc->endereco_atual++; 

}

void estado_1(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, BancoRegistradores *banco, UnidadeControle *uc){ //decode
    decod(reg_intermed->ri, uc); // decodifica instrução

    // carrega operandos A e B dos registradores
    reg_intermed->A = banco->registradores[uc->inst.rs];
    reg_intermed->B = banco->registradores[uc->inst.rt];

     // calcula o endereco de desvio caso seja uma instrucao de desvio
    reg_intermed->ULA_OUT = ula(pc->endereco_atual, uc->inst.immediate, 0, reg_intermed);

    // imprimir_instrucao(uc->inst);

}

void estado_2(RegIntermediario *reg_intermed, int immed){  // operação com imediato
    reg_intermed->ULA_OUT = ula(reg_intermed->A, immed, 0, reg_intermed);
}

void estado_3(Memoria *mem, RegIntermediario *reg_intermed){
    if(!(reg_intermed->ULA_OUT > 127)){
        return;
    } // verifica se é área de dados
    reg_intermed->RDM = mem->addr[reg_intermed->ULA_OUT].dado; // lê da memória
}

void estado_4(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco){ // escreve dado lido no registrador RDM
    banco->registradores[uc->inst.rt] = reg_intermed->RDM; 
}

void estado_5(Memoria *mem, RegIntermediario *reg_intermed){ // escreve valor na memória
    mem->addr[reg_intermed->ULA_OUT].dado = reg_intermed->B;

}

void estado_6(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco){ // escreve resultado do addi no registrador rt
    banco->registradores[uc->inst.rt] = reg_intermed->ULA_OUT;
}

void estado_7(RegIntermediario *reg_intermed, UnidadeControle *uc){ // execucao de instrucao tipo R
    reg_intermed->ULA_OUT = ula(reg_intermed->A, reg_intermed->B, uc->inst.funct, reg_intermed);
}

void estado_8(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco){ // escrever registrador tipo R 
    banco->registradores[uc->inst.rd] = reg_intermed->ULA_OUT;
}

void estado_9(PC *pc, RegIntermediario *reg_intermed){ //desvio condicional branch / se A == B pula para o endereco que esta em ULA_OUT, que foi calculado no estado 1
    if(ula(reg_intermed->A, reg_intermed->B, 6, reg_intermed) == 1) pc->endereco_atual = reg_intermed->ULA_OUT;
}

void estado_10(PC *pc, Instrucao inst){ //desvio incodicional jump
    pc->endereco_atual = inst.address;
}

void controle(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p){
    
    switch(*estado_atual){
        case 0: // busca
            push(p, pc, mem, reg_intermed, uc, banco, 0);
            estado_0(pc, mem, reg_intermed, uc, banco, p);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,pc->endereco_atual);
            *estado_atual = 1;
            break;

        case 1: //decodificação e leitura dos regs RS e RT e calculo do endereco de desvio condicional
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_1(pc, mem, reg_intermed, banco, uc);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,pc->endereco_atual);
            if(uc->inst.opcode == 4 || uc->inst.opcode == 11 || uc->inst.opcode == 15){
                *estado_atual = 2;
            } else if(uc->inst.opcode == 0){
                *estado_atual = 7;
            } else if(uc->inst.opcode == 8){
                *estado_atual = 9;
            } else if(uc->inst.opcode == 2){
                *estado_atual = 10;
            }
            break;
        case 2:// executa / calcula o endereco de acesso a memoria de uma instrucao do tipo I
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_2(reg_intermed, uc->inst.immediate);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,pc->endereco_atual);
            if(uc->inst.opcode == 11){
                *estado_atual = 3;
            } else if(uc->inst.opcode == 15){
                *estado_atual = 5;
            } else if(uc->inst.opcode == 4){
                *estado_atual = 6;
            }
            break;
        case 3:// busca na memoria o dado a ser carregado no registrador intermediario RDM
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_3(mem, reg_intermed);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->RDM,reg_intermed->ULA_OUT,pc->endereco_atual);
            *estado_atual = 4;
            break;
        case 4:// escreve o dado do registrador RDM no registrador do banco
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_4(reg_intermed, uc, banco);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->RDM,reg_intermed->ULA_OUT,pc->endereco_atual);
            *estado_atual = 0;
            break;
        case 5: // escreve na mem
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_5(mem, reg_intermed);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->RDM,reg_intermed->ULA_OUT,pc->endereco_atual);
            *estado_atual = 0;
            break;
        case 6:// escreve no registrador o resultado da instrucao addi
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_6(reg_intermed, uc, banco);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->RDM,reg_intermed->ULA_OUT,pc->endereco_atual);
            *estado_atual = 0;
            break;
        case 7: // executa instrucao tipo R
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_7(reg_intermed, uc);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,reg_intermed->RDM,pc->endereco_atual);
            *estado_atual = 8;
            break;
        case 8: // escreve registrador do tipo R
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_8(reg_intermed, uc, banco);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,reg_intermed->RDM,pc->endereco_atual);
            *estado_atual = 0;
            break;
        case 9: // desvio condicional (beq)
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_9(pc, reg_intermed);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,reg_intermed->RDM,pc->endereco_atual);
            *estado_atual = 0;
            break;
        case 10: // desvio incondicional 
            push(p, pc, mem, reg_intermed, uc, banco, *estado_atual);
            estado_10(pc, uc->inst);
            printf("ESTADO: %d | RI: %s| A: %d| B: %d| ULA: %d| RDM: %d| PC: %d\n",*estado_atual, reg_intermed->ri,reg_intermed->A,reg_intermed->B,reg_intermed->ULA_OUT,reg_intermed->RDM,pc->endereco_atual);
            *estado_atual = 0;
            break;
        }
        
            
}

void run(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p){ // tudo
    int i = 0;
    while(strcmp(mem->addr[pc->endereco_atual - 1].instrucao, "0000000000000000\0") != 0){
        controle(pc, mem, reg_intermed, estado_atual, banco, uc, p);
        i++;
    }
}

void step(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p){ // um ciclo por vez
    int c = 0;
    int t = 0;

    while(t == 0){
        t = c;
        controle(pc, mem, reg_intermed, estado_atual, banco, uc, p);
        if (*estado_atual == 4 || *estado_atual == 5 || *estado_atual == 6 || *estado_atual == 8 || *estado_atual == 9 ||*estado_atual == 10) c = 1;
    }

}

void initMemoria(Memoria *mem){
    for(int i=0;i<128;i++){
        strncpy(mem->addr[i].instrucao, "0000000000000000\0", 17); 
    }
    for(int i=128;i<256;i++){
       mem->addr[i].dado = 0;
    }
}

void initBanco(BancoRegistradores *banco){
    for (int i = 0;i<8;i++){
        banco->registradores[i] = 0;
    }
}

void imprimirMemoria(Memoria *mem){
        for(int i=0;i<256;i++){
        if(i < 128){
            printf("%d: %s\n", i, mem->addr[i].instrucao);
        } else {
            printf("%d: %d\n", i, mem->addr[i].dado);
        }
    }
}

void imprimirRegs(BancoRegistradores *banco){
    for (int i = 0; i<8; i++){
        printf("REG %d: %d\n", i, banco->registradores[i]);
    }
}

void carregarMemoria(Memoria *mem){// arq de memoria
    char arquivo[50];
    printf("Digite o nome do arquivo que deseja abrir: ");
    scanf("%s", arquivo);

    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    int i = 0;
    int tipo_dado = 0;
    char linha[17];
    while(!feof (f)){
        while (fgets(linha, 17, f) && i < 256 && tipo_dado == 0) { 
            if(linha[0] == '\n') continue;
            
            if(strcmp(linha, ".data\n") == 0){
                tipo_dado = 1;
                i = 128;
                break;
            };
            
            char *pos; // Ponteiro para a posicao do caractere de nova linha
            if ((pos = strchr(linha, '\n')) != NULL) {
                *pos = '\0'; 
            }
            
            strncpy(mem->addr[i].instrucao, linha, 17); // Copia a linha para a memória de programa
            i++;
        }
        if(tipo_dado == 1){
            int valor, pos;
            int j = 128;
            while ((fscanf(f, "%i:%i", &pos, &valor) == 2) && i < 256) { 
                if(!pos){
                mem->addr[j].dado = valor;
                j++;
                }
                mem->addr[pos].dado = valor;
                i++;
            }
        }
    }

    fclose(f);
    printf("Memoria de programa carregada com sucesso\n");

}

Instrucao decod(char* inst, UnidadeControle *uc) {
    char buffer[17];

    strncpy(buffer, inst, 4);
    buffer[4] = '\0';
    uc->inst.opcode = strtol(buffer, NULL, 2);

    if (uc->inst.opcode == 0) {
        uc->inst.tipo = 1;
        strncpy(buffer, inst + 4, 3); buffer[3] = '\0';
        uc->inst.rs = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 7, 3); buffer[3] = '\0';
        uc->inst.rt = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 10, 3); buffer[3] = '\0';
        uc->inst.rd = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 13, 3); buffer[3] = '\0';
        uc->inst.funct = strtol(buffer, NULL, 2);
    } else if (uc->inst.opcode == 2) {
        uc->inst.tipo = 3;
        strncpy(buffer, inst + 4, 12); buffer[12] = '\0';
        uc->inst.address = strtol(buffer, NULL, 2);
    } else {
        uc->inst.tipo = 2;
        strncpy(buffer, inst + 4, 3); buffer[3] = '\0';
        uc->inst.rs = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 7, 3); buffer[3] = '\0';
        uc->inst.rt = strtol(buffer, NULL, 2);

        strncpy(buffer, inst + 10, 6); buffer[6] = '\0';
        int imm = strtol(buffer, NULL, 2);
        if (inst[10] == '1') imm -= 64;
        uc->inst.immediate = imm;
    }

    imprimir_instrucao(uc->inst);
    return uc->inst;

}

void imprimir_instrucao(Instrucao i) {
    printf("Tipo: %d | Opcode: %d | RS: %d | RT: %d | RD: %d | Funct: %d | Immediate: %d | Address: %d\n",
        i.tipo, i.opcode, i.rs, i.rt, i.rd, i.funct, i.immediate, i.address);
}

int ula(int a, int b, int op, RegIntermediario *reg_intermed) { 

    switch (op) { 
        case 0:
            return a + b;  
            break;
        case 2: 
            return a - b;  
            break;
        case 4: 
            return a & b;
            break;  
        case 5: 
            return a | b;  
            break;
        case 6:
            return (a == b) ? 1 : 0;
        default: 
            printf("Operacao nao reconhecida: %d\n", op);
            return 0;
            break;
    }

} 

int check_overflow(int result) {
    if (result < -128 || result > 127) {
        return 1;
    }
    return 0;
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

void salvar_asm(Memoria *mem) {
    FILE *arquivo_asm = fopen("programa.asm", "w"); //w = write / ponteiro para escrever no arquivo
    if (arquivo_asm == NULL) {
        printf("Erro ao criar o arquivo\n");
        return;
    }
    // Iterar sobre cada instrução na memória e converter para assembly
    UnidadeControle uc_aux;
    for (int i = 0; i < 128; i++) {
        conv_asm(mem->addr[i].instrucao, arquivo_asm, decod(mem->addr[i].instrucao, &uc_aux));
        fprintf(arquivo_asm, "\n");
    }
    fclose(arquivo_asm);
    printf("Arquivo .asm salvo com sucesso!\n");
}
