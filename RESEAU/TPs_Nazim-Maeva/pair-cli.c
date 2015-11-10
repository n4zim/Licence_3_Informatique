#include "bor-util.h"

int mainLoop = 1;

void capterFin(int sig) {
    printf("Signal %d capté \n", sig);
    mainLoop = 0;
}


int main(int argc, char *argv[]) {
    if(argc < 3) { printf("Il manque des arguments"); return 0; }

    int socket; 
    struct sockaddr_un adrClient, adrServer;
    char buf[1024];

    char * nameClient = argv[1];
    char * nameServer = argv[2];

    printf("Initialisation du socket %s...\n", nameClient);
    socket = bor_create_socket_un(SOCK_STREAM, nameClient, &adrClient);

    bor_set_sockaddr_un(nameServer, &adrServer);
    printf("Connexion au serveur...\n");
    bor_connect_un(socket, &adrServer);

    bor_signal(SIGPIPE, capterFin, 0);
    bor_signal(SIGINT, capterFin, 0);

    while(mainLoop) {
        int r = bor_read(0, buf, 100);
        if(r <= 0) break;

        if(bor_write(socket, buf, r) < 0) break;

        r = bor_read(socket, buf, 100);
        if(r <= 0) break;
        printf("Sortie : ");
        if(bor_write(1, buf, r) < 0) break;
    }
    
    printf("Fin client.\n");
    close(socket);
    unlink(nameClient);
    
    return 0;
}