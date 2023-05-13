#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "InfoStructArray.h"


int main(int argc, char* argv[]) {

    InfoArray infoArray;
    initInfoArray(&infoArray);



    // Criação do pipe publico
    char path[30] = "../tmp/fifoPublic";

    if (mkfifo(path, 0666) < 0) {
        perror("[Monitor] Error with mkfifo.");
        return 1;
    }

    int public_fifo = open(path, O_RDONLY);

    if (public_fifo < 0) {
        perror("[Monitor] Error opening public fifo.");
    }
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("[Monitor] Pipe error.");
        return 1;
    }

    while (1) {
        int bytes_read = 0;
        struct Info i;
        // Ler do pipe a informação
        while ((bytes_read = read(public_fifo, &i, sizeof(Info)) > 0)) {
            if (i.status == 0) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("[Monitor] Fork error.");
                    exit(1);
                } else if (pid == 0) {
                    Info childInfo;
                    printf("pid-%d\n", i.pid);
                    printf("Name-%s\n", i.name);
                    printf("Tempo-%f\n", i.tempo);
                    printf("processtatus-%d\n", i.processtatus);
                    printf("status-%d\n", i.status);
                    // Processo filho
                    close(pipefd[0]);
                    write(pipefd[1], &i, sizeof(Info));
                    close(pipefd[1]);

                    char fifo_privado[40];
                    snprintf(fifo_privado, sizeof(fifo_privado), "../tmp/fifo%d", i.pid-1);
                    int private_fifo = open(fifo_privado, O_RDWR);
                    if (private_fifo < 0) {
                        perror("[Monitor] Error with private fifo.");
                        exit(1);
                    }
                    Info temp;
                    int bytes_received = read(private_fifo, &temp, sizeof(Info));
                    if (bytes_received < 0) {
                        perror("[Monitor] Error reading client update.");
                        exit(1);
                    }
                    close(private_fifo);
                    close(pipefd[1]);
                    exit(0);
                } else {
                    while(1){
                    int bytes_read1;
                    Info receivedInfo;
                    close(pipefd[1]);
                    if((bytes_read1 = read(pipefd[0], &receivedInfo, sizeof(Info))) > 0){
                        appendInfo(&infoArray, receivedInfo);
                        printf("%d\n", receivedInfo.pid);
                    }else{
                        break;
                    }
                }
                    // Processo pai
                    // Fica livre para ir buscar novos Infos
                }

            } else if (i.status == 1) {
                char fifo_status[40];
                snprintf(fifo_status, sizeof(fifo_status), "../tmp/fifo2");
                int priv2_fifo = open (fifo_status, O_WRONLY);
                printInfoArray(&infoArray);
            }
        }
    }
    return 0;
}







