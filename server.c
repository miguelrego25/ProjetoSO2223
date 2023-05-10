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


int main(int argc, char* argv[]) {
    int fileSize= 1;
    char buffer[1024];
    struct Info j;
    struct infoTable i;

    //int log;
    //log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

    if (mkfifo("server_fifo", 0600) < 0)
        perror("Erro fifo");

    int fifo = open("fifo", O_RDONLY);
    if (fifo < 0) perror("Erro fifo");

    while (1) {
        int bytes_read = 0;
        //ler do pipe a informação
        while ((bytes_read = read(fifo, &i, sizeof(Info)) > 0)) {
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
                while(){

                }
            /*read(, &i, sizeof(i));

            //Armazenar a informação num ficheiro para ler posteriormente
            FILE* fp = fopen("dados.txt", "w");

            fprintf(fp, "name:%s \ tempo =%f  \ status = %d\n", i.name, i.tempo, i.status);

            //isto não está correto temos que arranjar outra forma(talvez fazer se for status por na struct na mesma mas pôr tudo a NULL e o status a 1 ou alguma coisa parecida)
            if(strcmp(&i, "status" == 0)){

            }

        }else{
            close("server_Fifo");
        }
            */
        }
    }
    return 0;
}

