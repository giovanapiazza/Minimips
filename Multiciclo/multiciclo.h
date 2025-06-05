#ifndef MULTICICLO_H__
#define MULTICICLO_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>



typedef union {
    char instrucao[17];
    int dado;
}Endereco;

typedef struct{
    Endereco addr[256]; 
} Memoria;

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

typedef struct{
    Instrucao inst;
} UnidadeControle;


typedef struct {
    int registradores[8];
} BancoRegistradores;

typedef struct {
    char ri[17];
    int A;
    int B;
    int ULA_OUT;
    int RDM;
} RegIntermediario;

typedef struct{
    int endereco_atual;
    int endereco_proximo;
} PC;

struct Node{
    PC pPc;
    Memoria pMem;
    RegIntermediario pReg_intermed;
    UnidadeControle pUc;
    BancoRegistradores pBanco;
    int pEstado_atual;
    struct Node *prox;
};

typedef struct Node nodo;

nodo *alocaNodo();
int estaVazia(nodo *p);
void push(nodo *p, PC *pPc, Memoria *pMem, RegIntermediario *pReg_intermed, UnidadeControle *pUc, BancoRegistradores *pBanco, int pEstado_atual);
nodo *pop(nodo *p);

void estado_0(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco, nodo *p);
void estado_1(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, BancoRegistradores *banco, UnidadeControle *uc);
void estado_2(RegIntermediario *reg_intermed, int immed);
void estado_3(Memoria *mem, RegIntermediario *reg_intermed);
void estado_4(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco);
void estado_5(Memoria *mem, RegIntermediario *reg_intermed);
void estado_6(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco);
void estado_7(RegIntermediario *reg_intermed, UnidadeControle *uc);
void estado_8(RegIntermediario *reg_intermed, UnidadeControle *uc, BancoRegistradores *banco);
void estado_9(PC *pc, RegIntermediario *reg_intermed);
void estado_10(PC *pc, Instrucao inst);

void controle(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p);
void run(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p);
void step(PC *pc, Memoria *mem, RegIntermediario *reg_intermed, int *estado_atual, BancoRegistradores *banco, UnidadeControle *uc, nodo *p);

void initMemoria(Memoria *mem);
void initBanco(BancoRegistradores *banco);
void imprimirMemoria(Memoria *mem);
void imprimirRegs(BancoRegistradores *banco);
void carregarMemoria(Memoria *mem);

Instrucao decod(char* inst, UnidadeControle *uc);
void imprimir_instrucao(Instrucao i);
int ula(int a, int b, int op, RegIntermediario *reg_intermed);
int check_overflow(int result);
void conv_asm(char *bin_inst, FILE *arquivo_asm, Instrucao inst);
void salvar_asm(Memoria *mem);
#endif /* MULTICICLO_H__*/
