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



    //criação do pipe publico
    char path[30] = "../tmp/fifoPublic";

    if (mkfifo(path, 0666) < 0) {
        perror("mkfifo");
        return 1;
    }

    int public_fifo = open(path, O_RDONLY);

    if (public_fifo < 0) {
        perror("open public fifo");
    }
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        return 1;
    }

    while (1) {
        int bytes_read = 0;
        struct Info i;
        //ler do pipe a informação
        while ((bytes_read = read(public_fifo, &i, sizeof(Info)) > 0)) {
            if (i.status == 0) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Erro no fork");
                    exit(1);
                } else if (pid == 0) {
                    Info childInfo;
                    //processo filho
                    close(pipefd[0]);
                    write(pipefd[1], &childInfo, sizeof(Info));
                    close(pipefd[1]);

                    char fifo_privado[40];
                    snprintf(fifo_privado, sizeof(fifo_privado), "../tmp/fifo%d", i.pid-1);
                    int private_fifo = open(fifo_privado, O_RDWR);
                    if (private_fifo < 0) {
                        perror("erro private fifo ew");
                        exit(1);
                    }
                    Info temp;
                    int bytes_received = read(private_fifo, &temp, sizeof(Info));
                    if (bytes_received < 0) {
                        perror("erro ao ler o update do cliente");
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
                    }else{
                        break;
                    }
                }
                    //processo pai
                    // fica livre para ir buscar novos Infos
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




