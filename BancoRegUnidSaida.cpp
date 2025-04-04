#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int reg[8];
} BancoRegistradores;

void unidadedeSaida(BancoRegistradores *BR);

int main(){
    BancoRegistradores BR = {0};
    unidadedeSaida(&BR);
    return 0;
}

void unidadedeSaida(BancoRegistradores *BR){
    int i;
    const char *nomes[] = {"$zero", "$v0", "$a0", "$t0", "$t1", "$s0", "$s1", "$ra"};

    for (i=0; i<8; i++){
        printf("Registrador %s: %d\n", nomes[i], (*BR).reg[i]);
    }
}