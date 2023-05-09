#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFF_SIZE 256
typedef struct{
    int pid;
    char program_name[BUFF_SIZE];
    struct timeval start_time;
}ProgramInfo;

int main(){
    int fd;
    char buff[BUFF_SIZE];
    char *prog_info[BUFF_SIZE];

    for (int i = 0; i < BUFF_SIZE; i++) {
        prog_info[i] = NULL;
    }

    mkfifo("server_fifo",0666);
    printf("server walking...\n");

    while(1){
        fd = open("server_fifo",O_RDONLY);
        read(fd,buff,BUFF_SIZE);

        //ver o que foi recebido e comparar atraves de if clauses
        if(strncmp(buff,"execute ",))
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/time.h>
//#include <time.h>


typedef struct info
{
    pid_t pid;
    char name [300];
    double tempo;
    int status;
}info;


int main(int argc, char* argv){

    struct info i;

    pid_t pid = fork();

    if(pid == 0){
        
        //ler do pipe a informação
        int fd= open("server_Fifo", O_RDONLY);

        read(fd, &i, sizeof(i));

        //Armazenar a informação num ficheiro para ler posteriormente
        FILE* fp = fopen("dados.txt", "w");

        fprintf(fp, "name:%s \ tempo =%f  \ status = %d\n", i.name, i.tempo, i.status);

        //isto não está correto temos que arranjar outra forma(talvez fazer se for status por na struct na mesma mas pôr tudo a NULL e o status a 1 ou alguma coisa parecida)
        if(strcmp(&i, "status" == 0)){

        }





    }else{
        close("server_Fifo");
    }
  
  return 0;


}


