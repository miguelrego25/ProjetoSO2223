#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/time.h>


#define MAX_ARGS 20
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {

    if (argc < 3) {
        perror("not enough arguments");
        exit(1);
    }
    char command[BUFFER_SIZE];
    char args[MAX_ARGS][BUFFER_SIZE];
    int num_args = 0;

    /*
    int server_fd = open("server_Fifo", O_WRONLY);
    if (server_fd < 0) {
        perror("Erro ao abrir FIFO");
        exit(1);
    }
//
    //passar a informação para o servidor
    //esta errado tem que se arranjar uma maneira atraves de
    //uma struct ou as sim
    for (int i = 1; i < argc; i++) {
        write(server_fd, argv[i], strlen(argv[i]) + 1);
    }
     */
    strcpy(command,argv[1]);

    for (int i = 2; i < argc && num_args < MAX_ARGS; i++) {
        strcpy(args[num_args],argv[i]);
        num_args++;
    }
    pid_t pid = fork();

    if(pid < 0){
        perror("erro ao criar processo");
        exit(1);

    }else if(pid == 0){


        //obter pid do processo filho
        pid_t child_pid = getpid();

        //abrir pipe para notificar o servidor do novo programa em execução
        int server_fd = open("server_Fifo", O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }

        //falta aqui escrever para o server as informaçoes
        //atraves da estrutura que decidirmos fazer
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "%d %s \n",child_pid,command);

        write(server_fd, message, strlen(message));
        close(server_fd);

        //mais 3 argumentos são "name of file", "execute", "-u"
        execvp("cliente", argv+2);


        perror("erro ao executar o comando");
        exit(1);
    }else{
        //codigo processo pai

        //espera pelo filho terminar
        int status;
        waitpid(pid,&status,0);

        int server_fd = open("server_Fifo", O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }

        //vai ser necessario para transmitir o tempo que passou
        //ao servidor

        char message[BUFFER_SIZE];

        write(server_fd, message, strlen(message));
        close(server_fd);
    }
    return 0;
}