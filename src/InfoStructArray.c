#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "InfoStructArray.h"

/** Inicialização do InfoArray **/
void initInfoArray(InfoArray* infoArray) {
    infoArray->array = NULL;
    infoArray->size = 0;
    infoArray->capacity = 0;
}

/** Adicionar nova Info ao Array **/
void appendInfo(InfoArray* infoArray, Info newInfo) {
    if (infoArray->size >= infoArray->capacity) {
        // Se a capacidade for insuficiente, passa ao dobro
        size_t newCapacity = infoArray->capacity == 0 ? 1 : infoArray->capacity * 2;
        infoArray->array = (Info*)realloc(infoArray->array, newCapacity * sizeof(Info));
        infoArray->capacity = newCapacity;
    }

    // Append da newInfo no array
    infoArray->array[infoArray->size] = newInfo;
    infoArray->size++;
}

/** Libertação de memória **/
void freeInfoArray(InfoArray* infoArray) {
    free(infoArray->array);
    infoArray->array = NULL;
    infoArray->size = 0;
    infoArray->capacity = 0;
}

/** Apagar Info **/
void deleteInfo(InfoArray* infoArray, size_t index) {
    if (index >= infoArray->size) {
        perror("[InfoArray] Invalid index to delete from InfoArray.");
        return;
    }

    // Os elementos depois do apagado são movidos uma posição
    for (size_t i = index; i < infoArray->size - 1; i++) {
        infoArray->array[i] = infoArray->array[i + 1];
    }

    // Decrementar o tamanho
    infoArray->size--;

    // Se o tamanho for significamente mais pequeno que a capacidade, faz-se resize do array
    if (infoArray->size <= infoArray->capacity / 4) {
        size_t newCapacity = infoArray->capacity / 2;
        if (newCapacity < 1) {
            newCapacity = 1;
        }
        infoArray->array = (Info*)realloc(infoArray->array, newCapacity * sizeof(Info));
        infoArray->capacity = newCapacity;
    }
}

/** Print do InfoArray **/
void printInfoArray(const InfoArray* infoArray) {
    printf("Current status:\n");

    for (size_t i = 0; i < infoArray->size; i++) {
        Info info = infoArray->array[i];
        struct timeval end1;
        gettimeofday(&end1, NULL);
        double time_spent1 = (end1.tv_sec - info.tempo.tv_sec) * 1000 + (end1.tv_usec - info.tempo.tv_usec) / 1000;
        info.tempofinal = time_spent1;
        printf("    PID: %d ", info.pid);
        printf("    Name: %s ", info.name);
        printf("    Time: %f\n", info.tempofinal);
    }
}

