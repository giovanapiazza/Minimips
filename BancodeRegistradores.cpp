#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int reg[8];
} BancoRegistradores;

void addValor(BancoRegistradores *BR, int numreg, int valor);
int mostrarValor(BancoRegistradores *BR, int numreg);
void printarRegistradores(BancoRegistradores *BR);

int main(){
    BancoRegistradores reg = {0};
    int op, numreg, valor;

    do {
        printf("Menu de opções:\n");
        printf("1- Adicionar um valor em um registrador\n");
        printf("2- Ver o valor de um registrador\n");
        printf("3- Ver o valor de todos registradores\n");
        printf("4- Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &op);

        switch (op){
            case 1:
                printf("Informe o número do registrador (0 - 7): ");
            scanf("%d", &numreg);
            printf("Digite  valor do registrador: ");
            scanf("%d", &valor);
            addValor(&reg, numreg, valor);
            break;
            case 2:
                printf("Digite o número do registrador (0 - 7): ");
            scanf("%d", &numreg);
            valor = mostrarValor(&reg, numreg);
            if (valor != -1){
                printf("Valor no registrador %d: %d\n", numreg, valor);
            }
            break;
            case 3:
                printf("Lista de registradores com seus valores:\n");
                printarRegistradores(&reg);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inváida!");
        }
    } while (op != 4);

    return 0;
}

void addValor(BancoRegistradores *BR, int numreg, int valor){
    if (numreg == 0){
        printf("O registrador $zero não pode ser alerado!\n");
    }
    else if (numreg >= 1 && numreg < 8){
        (*BR).reg[numreg] = valor;
    } else {
        printf("Registrador inválido!\n");
    }
}

int mostrarValor(BancoRegistradores *BR, int numreg){
    if (numreg >= 0 && numreg < 8){
        return (*BR).reg[numreg];
    } else {
        printf("Registrador não encontrado!\n");
        return -1;
    }
}

void printarRegistradores(BancoRegistradores *BR){
    int i;
    for (i=0; i<8; i++){
        printf("Registrador %d: %d\n", i, (*BR).reg[i]);
    }
}