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
    char name [300];
    double tempo;
    int processtatus;
    int status;
}info;


int main(int argc, char* argv[]) {

    //Cria pipe privado
    char fifo_privado[10];
    char path_public[30];
    char path_privado[30];
    snprintf(fifo_privado, sizeof(fifo_privado), "fifo%d", getpid());
    snprintf(path_privado, sizeof(path_privado), "tmp/%s", fifo_privado);
    snprintf(path_public, sizeof(path_public), "tmp/fifoPublic");

    //acho que nao vai ser necessario
    /*if(mkfifo(path_privado, 0666) < 0){
        perror("mkfifo");
        return 1;
    }*/

    //abrir o pipe privado logo quando começa o programa
    int fifo_Public = open(path_public, O_WRONLY);

    if(fifo_Public < 0){
        perror("Open Public");
    }

    //sem argmuentos suficientes para fazer alguma coisa
    if (argc < 3 && (strcmp(argv[1],"status") == 0)){
        perror("not enough arguments");
        exit(1);
    }
    char command[BUFFER_SIZE];
    char args[MAX_ARGS][BUFFER_SIZE];
    int num_args = 0;
    info i;


    //falta completar ( abrir um pipe de read para o server, e abrir um de write para receber do server as informçoes)
    if(argv[1] == "status"){
        i.status = 1;
        i.pid = 2;

        /* Escreve o pedido de status no pipe com nome */
        write(fifo_Public, &i, sizeof(info));

        /* Fecha o pipe com nome */
        close(fifo_Public);

        /* Abre o pipe com nome para leitura */
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

    for (int i = 3; i < argc && num_args < MAX_ARGS; i++) {
        strcpy(args[num_args], argv[i]);
        num_args++;
    }

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

        /*
        //---------------------------------------------------------------------------
        //ESTE BOCADO JÁ NÂO É PRECISO PORQUE ABRIMOS O PIPE PUBLICO LOGO NO INICIO DO PROGRAMA
        //abrir pipe para notificar o servidor do novo programa em execução
        //int server_fd = open("server_fifo", O_WRONLY);
        //if(server_fd < 0){
        //    perror("erro ao abrir pipe do servidor");
        //    exit(1);
        //}*/

        //falta aqui escrever para o server as informaçoes
        //atraves da estrutura que decidirmos fazer

        write(fifo_Public, &i, sizeof(struct info));
        close(fifo_Public);

        //mais 3 argumentos são "name of file", "execute", "-u"
        execvp(command, args);


        perror("erro ao executar o comando");
        exit(1);

    }else{
        //codigo processo pai
        //espera pelo filho terminar
        int status;
        waitpid(pid,&status,0);

        clock_t end2 = clock();
        double time_spent2 = (double)(end2 - begin1) / CLOCKS_PER_SEC;

        //ler a mensagem do filho
        /**int read_fd = open("server_fifo",O_RDONLY);
        if(read_fd<0){
            perror("Erro ao abrir FIFO");
            exit(1);
        }

        **/


        /*
        //nao sei o porque disto// tipo no ha necessidade de ler nada do lado do pai ele apenas precisa
        //de mandar pra o servidor a dizer que o processo ja terminou
        int fifo_private = open(path_privado, O_RDONLY);
        if(fifo_private < 0){
            perror("Open Private");
        }
        char buffer[BUFFER_SIZE];
        int bytes_read = read(fifo_private, buffer, BUFFER_SIZE);
        if(bytes_read < 0){
            perror("Erro ao ler do FIFO");
            exit(1);
        }
        close(fifo_private);

        //mostra a mensagem do filho
        printf("Resultado: %s\n", buffer);
        //////////////////////////////////////////
        */

        //é necessario converter o pid de int para char*
        char pid_string[20];
        sprintf(pid_string, "%d", pid);

        //abrir o FIFO do servidor
        int server_fd = open(pid_string, O_WRONLY);
        if(server_fd < 0){
            perror("erro ao abrir pipe do servidor");
            exit(1);
        }


        //transmissao de update para o servidor
        info i;
        i.pid = pid;
        strcpy(i.name, command);
        i.tempo = time_spent2;
        i.processtatus = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        i.status = 0;
        write(server_fd, &i, sizeof(info));
        close(server_fd);
    }
    return 0;
}
