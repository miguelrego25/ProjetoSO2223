#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "InfoStructArray.h"

void initInfoArray(InfoArray* infoArray) {
    infoArray->array = NULL;
    infoArray->size = 0;
    infoArray->capacity = 0;
}

void appendInfo(InfoArray* infoArray, Info newInfo) {
    if (infoArray->size >= infoArray->capacity) {
        // If the capacity is not enough, double the capacity
        size_t newCapacity = infoArray->capacity == 0 ? 1 : infoArray->capacity * 2;
        infoArray->array = (Info*)realloc(infoArray->array, newCapacity * sizeof(Info));
        infoArray->capacity = newCapacity;
    }

    // Append the newInfo to the array
    infoArray->array[infoArray->size] = newInfo;
    infoArray->size++;
}

void freeInfoArray(InfoArray* infoArray) {
    free(infoArray->array);
    infoArray->array = NULL;
    infoArray->size = 0;
    infoArray->capacity = 0;
}
void deleteInfo(InfoArray* infoArray, size_t index) {
    if (index >= infoArray->size) {
        // Invalid index
        return;
    }

    // Move elements after the deleted element by one position
    for (size_t i = index; i < infoArray->size - 1; i++) {
        infoArray->array[i] = infoArray->array[i + 1];
    }

    // Decrement the size
    infoArray->size--;

    // If the size is significantly smaller than the capacity, resize the array
    if (infoArray->size <= infoArray->capacity / 4) {
        size_t newCapacity = infoArray->capacity / 2;
        if (newCapacity < 1) {
            newCapacity = 1;
        }
        infoArray->array = (Info*)realloc(infoArray->array, newCapacity * sizeof(Info));
        infoArray->capacity = newCapacity;
    }
}

void printInfoArray(const InfoArray* infoArray) {
    printf("InfoArray:\n");
    printf("  Size: %zu\n", infoArray->size);
    printf("  Capacity: %zu\n", infoArray->capacity);


    for (size_t i = 0; i < infoArray->size; i++) {
        Info info = infoArray->array[i];
        struct timeval end1;
        gettimeofday(&end1, NULL);
        double time_spent1 = (end1.tv_sec - info.tempo.tv_sec) * 1000000 + (end1.tv_usec - info.tempo.tv_usec);
        info.tempofinal = time_spent1;
        printf("  Info %zu:\n", i);
        printf("    PID: %d\n", info.pid);
        printf("    Name: %s\n", info.name);
        printf("    Tempo: %f\n", info.tempofinal);
        printf("    Processtatus: %d\n", info.processtatus);
        printf("    Status: %d\n", info.status);
    }
}

