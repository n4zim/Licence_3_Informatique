#include "bor-util.h"

int mainLoop = 1;

void capter(int sig) {
    printf("signal %d capté \n", sig);
    mainLoop = 0;
}

int ctoi(char c) { if('0'<=c && c<='9') return c - '0'; else return -1; }

void fils(int service) {
    int r, i;
    char buf[1024];

    while(mainLoop) {
        r = bor_read(service, buf, 1024);
        if(r <= 0) break;
        for(i=0; i<r; i++) if(ctoi(buf[i]) % 2 == 0)
        bor_write(service, buf+i, 1); 
    }
    close(service);
    exit(0);
}

int main(int argc, char *argv[]) {
    if(argc < 2) { printf("Il manque des arguments"); return 0; }

    int ecoute;
    struct sockaddr_un AdrServer, adrClient;
    
    char * nameServer = argv[1];

    printf("Initialisation du socket %s...\n", nameServer);
    ecoute = bor_create_socket_un(SOCK_STREAM, nameServer, &AdrServer);

    bor_listen(ecoute, SOMAXCONN);

    bor_signal(SIGPIPE, capter, 0);
    bor_signal(SIGINT, capter, 0);

    /*bor_signal(SIGPIPE, capter,int sig 0
    printf("signal %d capté \n", sig););
    
    bor_signal(SIGINT, capter,int sig 0
    printf("signal %d capté \n", sig););*/


    while(mainLoop) {
        int service = bor_accept_un(ecoute, &adrClient);
        int k = fork();
        if(k < 0) { printf("Fork Error"); return 0; }
        if(k == 0) fils(service);
        close(service);
    }

    printf("Serveur fermé !\n");
    close(ecoute);
    unlink(nameServer);
    return 0;

}