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
    printf("Escolha uma opcao: "); 
    setbuf(stdin, NULL);
    scanf("%d", &m);

    return m;
}
