
#ifndef INFO_ARRAY_H
#define INFO_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct Info {
    pid_t pid;
    char name[300];
    struct timeval tempo;
    double tempofinal;
    int processtatus;
    int status;
} Info;

typedef struct InfoArray {
    Info* array;
    size_t size;
    size_t capacity;
} InfoArray;

void initInfoArray(InfoArray* infoArray);
void appendInfo(InfoArray* infoArray, Info newInfo);
void freeInfoArray(InfoArray* infoArray);
void deleteInfo(InfoArray* infoArray, size_t index);
void printInfoArray(const InfoArray* infoArray);

#endif /* INFO_ARRAY_H */
