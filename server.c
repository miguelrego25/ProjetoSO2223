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


typedef struct Info
{
    pid_t pid;
    char name [300];
    double tempo;
    int processtatus;
    int status;
}Info;


typedef struct infoTable{
    size_t n_info;
    Info* info;
}infoTable;

int private_pipe;


int main(int argc, char* argv[]) {
    char buffer[1024];
    struct Info j;
    struct infoTable i;

    //int log;
    //log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    //criar o pipe privado

    //criação do pipe publico
    char path[30] = "tmp/fifoPublic";

    if(mkfifo(path, 0666) < 0){
        perror("mkfifo");
        return 1;
    }
    /*
    if (mkfifo("server_fifo", 0600) < 0)
        perror("Erro fifo");

    int fifo = open("fifo", O_RDONLY);
    if (fifo < 0) perror("Erro fifo");
    */

   int public_fifo = open(path,O_WRONLY);

   if(public_fifo < 0){
    perror("open public fifo");
   }

    while (1) {
        int bytes_read = 0;
        //ler do pipe a informação
        while ((bytes_read = read(public_fifo, &i, sizeof(Info)) > 0)) {



            if(j.status == 0){
               i.n_info = j.pid;
               i.info->pid = j.pid;
               strcpy(i.info->name, j.name);
               i.info->status = j.status;
               i.info->processtatus = j.processtatus;



                //falta criar
            }
            if(j.status == 1){
                int bytesRead = 0;
                while(/*Não sei como iterar pela struct até ver quais dos elementos estão em processo*/){
                    if(i.info->processtatus == 1){
                        write(private_pipe, &j, sizeof(struct infoTable));
                    }
                }

    }
    close(private_pipe);
    return 0;
}

