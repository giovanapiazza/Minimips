#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "multiciclo.h"
#include "multiciclo.c"

int main(){

    BancoRegistradores banco;  // ou passe como argumento
    RegIntermediario reg_intermed;
    PC pc;
    Memoria mem;
    UnidadeControle uc;
    int estado_atual = 0;

    initMemoria(&mem);
    initBanco(&banco);
    nodo *p = alocaNodo();

    nodo *tmp;

    pc.endereco_atual = 0;

    int op = 0;
    int c = 1;
    while(c == 1){
        printf("----------------- MENU ---------------\n");
        printf("Opcao 1: Carregar Memoria\n");
        printf("Opcao 2: Imprimir Memoria\n");
        printf("Opcao 3: Executar um ciclo\n");
        printf("Opcao 4: Imprimir Registradores\n");
        printf("Opcao 5: Converter asm\n");
        printf("Opcao 6: Run\n");
        printf("Opcao 7: Executar uma instrucao\n");
        printf("Opcao 8: Imprimir Simulador\n");
        printf("Opcao 9: Back por Instrucao\n");
        printf("Opcao 10: Back por ciclo\n");
        printf("--------------------------------------\n");
        setbuf(stdin, NULL);
        scanf("%d", &op);
        switch(op){
            case 1: 
                carregarMemoria(&mem);
                break;
            case 2:
                imprimirMemoria(&mem);
                break;
            case 3:
                controle(&pc, &mem, &reg_intermed, &estado_atual, &banco, &uc, p);
                break;
            case 4:
                imprimirRegs(&banco);
                break;
            case 5:
                salvar_asm(&mem);
                break;
            case 6:
                run(&pc, &mem, &reg_intermed, &estado_atual, &banco, &uc, p);
                break;
            case 7:
                step(&pc, &mem, &reg_intermed, &estado_atual, &banco, &uc, p);
                break;
            case 8:
                imprimirRegs(&banco);
                imprimirMemoria(&mem);
                break;
            case 9: 
                tmp = pop(p); // volta um ciclo pq pEstado_atual = 0
                if(tmp == NULL){
                    printf("Nao ha estados anteriores para voltar.\n");
                } else {        // volta ate pEstado_atual ser  = 0 / desfaz a ultima instrucao executada 
                    while(tmp->pEstado_atual != 0){
                        tmp = pop(p);
                    }
                    if(tmp != NULL){
                        uc = tmp->pUc;
                        pc = tmp->pPc;
                        reg_intermed = tmp->pReg_intermed;
                        mem = tmp->pMem;
                        banco = tmp->pBanco;
                        estado_atual = tmp->pEstado_atual;
                        printf("PC APOS BACK: %d\n", pc.endereco_atual);
                    }
                }
                    break;
            case 10:
                tmp = pop(p); //  desfaz o ultimo ciclo executado
                if(tmp != NULL){
                    uc = tmp->pUc;
                    pc = tmp->pPc;
                    reg_intermed = tmp->pReg_intermed;
                    mem = tmp->pMem;
                    banco = tmp->pBanco;
                    estado_atual = tmp->pEstado_atual;
                    printf("ESTADO APOS BACK: %d\n", estado_atual);
                } else {
                    printf("Nao ha estados anteriores para voltar.\n");
                }

                break;
        }

    }
    
}
