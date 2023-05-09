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


#define MAX_ARGS 20
#define BUFFER_SIZE 256

//struct de info para passar para o server
typedef struct info
{
    pid_t pid;
    char name[300];
    double tempo;
    int status;
} info;


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


    strcpy(command, argv[1]);

    for (int i = 2; i < argc && num_args < MAX_ARGS; i++) {
        strcpy(args[num_args], argv[i]);
        num_args++;
    }
    info i;

    //calcular o tempo antes da realização do programa
    clock_t begin1 = clock();

    pid_t pid = fork();

    if(pid < 0){
        perror("erro ao criar processo");
        exit(1);

    }else if(pid == 0){
        //obter pid do processo filho e por na struct o pid do processo
        pid_t child_pid = getpid();
        i.pid = child_pid;

        //obter o nome dos programas a fazer para a struct (f=2 pois "execute", "-u") não sei se faz muito sentido help xd
        for(int f = 1; f < argc; f++){
            strcat(i.name, args[f]);
        }

        //função que vai calcular o tempo de execução antes da realização do programa
        clock_t end1 = clock();
        double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
        i.tempo = time_spent1;

        //Programa em realização
        i.status = 0;


        //abrir pipe para notificar o servidor do novo programa em execução
        int server_fd = open("server_Fifo", O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }

        //falta aqui escrever para o server as informaçoes
        //atraves da estrutura que decidirmos fazer

        write(server_fd, &i, sizeof(struct info));
        close(server_fd);

        //mais 3 argumentos são "name of file", "execute", "-u"
        execvp(command, args);


        perror("erro ao executar o comando");
        exit(1);

        //tempo total depois de executar o programa


        //mandar para o server com a info atualizada(tempo e status) nao sei muito bem como fazer mas vou tentar


    }else{
        //codigo processo pai
        //espera pelo filho terminar
        int status;
        waitpid(pid,&status,0);

        clock_t end2 = clock();
        double time_spent2 = (double)(end2 - begin1) / CLOCKS_PER_SEC;

        //ler a mensagem do filho
        int read_fd = open("client_fifo",O_RDONLY);
        if(read_fd<0){
            perror("Erro ao abrir FIFO");
            exit(1);
        }
        char buffer[BUFFER_SIZE];
        int bytes_read = read(read_fd, buffer, BUFFER_SIZE);
        if(bytes_read < 0){
            perror("Erro ao ler do FIFO");
            exit(1);
        }
        close(read_fd);

        //mostra a mensagem do filho
        printf("Resultado: %s\n", buffer);

        //abrir o FIFO do servidor
        int server_fd = open("server_fifo", O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }

        //vai ser necessario para transmitir o tempo que passou
        //ao servidor

        info i;
        i.pid = pid;
        strcpy(i.name, command);
        i.tempo = -1;
        i.status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        write(server_fd, &i, sizeof(info));
        close(server_fd);
    }
    return 0;
}
