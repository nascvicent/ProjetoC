#include <stdio.h>
#include <stdlib.h>
#include "rank.h"

void ranqueamento() {
    // Abre o arquivo para leitura
    FILE *file = fopen("pontuacao.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    // L� as pontua��es do arquivo e as exibe
    int pontuacao;
    int position = 1;
    while (fscanf(file, "%d", &pontuacao) != EOF) {
        printf("Posi��o %d: Pontua��o %d\n", position, pontuacao);
        position++;
    }

    // Fecha o arquivo
    fclose(file);
}
