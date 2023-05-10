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


typedef struct info
{
    pid_t pid;
    char name [300];
    double tempo;
    int processtatus;
    int status;
}info;


int main(int argc, char* argv[]) {
    char buffer[1024];
    struct info i;

    int log;
    log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

    if (mkfifo("server_fifo", 0600) < 0)
        perror("Erro fifo");

    int fifo = open("server_fifo", O_RDONLY);
    if (fifo < 0) perror("Erro fifo");

    while (1) {
        int bytes_read = 0;
        //ler do pipe a informação
        while ((bytes_read = read(fifo, &i, sizeof(info)) > 0)) {
            if(i.status == 1){
                //falta criar
            }
            if()

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

