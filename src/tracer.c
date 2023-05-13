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


#define MAX_ARGS 20
#define BUFFER_SIZE 256

//struct de info para passar para o server


int main(int argc, char* argv[]) {
    //Cria pipe privado
    char fifo_privado[10];
    char path_public[30];
    char path_privado[30];
    snprintf(fifo_privado, sizeof(fifo_privado), "fifo%d", getpid());
    snprintf(path_privado, sizeof(path_privado), "tmp/%s", fifo_privado);
    snprintf(path_public, sizeof(path_public), "../tmp/fifoPublic");

    if (mkfifo(path_privado, 0666) < 0) {
        printf(path_privado);
        perror("mkfifo deu porra");
        return 1;
    }

    int public_fifo = open(path_public, O_WRONLY);
    if(public_fifo < 0 ){
        perror("Open Public");
    }
    //sem argmuentos suficientes para fazer alguma coisa
    if (argc < 3 && (strcmp(argv[1],"status") == 1)){
        perror("not enough arguments");
        exit(1);
    }
    char command[BUFFER_SIZE];
    char* args[MAX_ARGS + 1];
    int num_args = 0;
    Info i;


    //falta completar (abrir um pipe de read para o server, e abrir um de write para receber do server as informçoes)
    if(strcmp(argv[1],"status") == 0){
        i.status = 1;
        i.pid = 2;

        /* Escreve o pedido de status no pipe com nome */
        write(public_fifo, &i, sizeof(Info));

        /* Fecha o pipe com nome */
        close(public_fifo);

        /* Abre o pipe com nome para leitura  vamos abrir pipe com pid 2 por simplicidade é pouco provavel que esteja a ser usado*/
        int fifo_private= open("2", O_RDONLY);
        if (fifo_private == -1) {
            perror("open");
            exit(1);
        }

        /* Lê a resposta do servidor */
        char buf[BUFFER_SIZE];
        while (read(fifo_private, buf, BUFFER_SIZE) > 0) {
            /* Processa a resposta do servidor */
            printf("%s\n", buf);
        }

        /* Fecha o pipe com nome */
        close(fifo_private);
    }

    strcpy(command, argv[0]);

    for (int k = 3; k < argc && num_args < MAX_ARGS; k++) {
        args[num_args] = strdup(argv[k]);
        num_args++;
    }
    args[num_args] = NULL;

    struct timeval begin1, end1;
    gettimeofday(&begin1, NULL);

    pid_t pid = fork();

    if(pid < 0){
        perror("erro ao criar processo");
        exit(1);

    }else if(pid == 0){



        pid_t child_pid = getpid();
        i.pid = child_pid;
        i.tempo =  begin1;




        //obter pid do processo filho e por na struct o pid do processo
        //obter o nome dos programas a fazer para a struct (f=2 pois "execute", "-u") não sei se faz muito sentido help xd

        //função que vai calcular o tempo de execução antes da realização do programa

        //Programa em realização
        i.status = 0;
        for (int f = 4; f < argc; f++) {
            strncat(i.name, argv[f], MAX_ARGS - strlen(i.name) - 1);
        }
        write(public_fifo, &i, sizeof(Info));
        close(public_fifo);

        ////lixo
        //char message[] = "Hello, world! filho \n";
////
        ////Write to stdout
        //if (write(1, message, sizeof(message) - 1) < 0) {
        //    perror("Error writing to stdout");
        //    return 1;
        //}
        ////fim do lixo

        //mais 3 argumentos são "name of file", "execute", "-u"

        execvp(argv[3], args);

        //char *args[] = {"ls", NULL};
        //execvp("ls", args);


        perror("erro ao executar o comando");
        exit(1);

    }else{
        //codigo processo pai
        //espera pelo filho terminar
        int status;
        waitpid(pid,&status,0);
        //lixo
        char message[] = "Hello, world pai !\n ";

        // Write to stdout
        if (write(1, message, sizeof(message) - 1) < 0) {
            perror("Error writing to stdout");
            return 1;
        }
        //fim do lixo
        gettimeofday(&end1, NULL);
        double time_spent2 = (end1.tv_sec - begin1.tv_sec) + (end1.tv_usec - begin1.tv_usec) / 1000000.0;


        //é necessario converter o pid de int para char*
        char pid_string[20];
        sprintf(pid_string, "%d", pid);

        //abrir o FIFO do servidor
        int server_fd = open(path_privado, O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }

        //transmissao de update para o servidor
        Info info;
        info.pid = pid;
        strcpy(info.name, command);
        info.tempofinal = time_spent2;
        info.processtatus = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        info.status = 0;
        write(server_fd, &info, sizeof(Info));
        close(server_fd);
    }
    return 0;
}
