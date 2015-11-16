#include "bor-util.h"

int server_connection(int socket, struct sockaddr_in *addr_server){
	printf("connexion avec%s : %d \n", inet_ntoa(addr_server->sin_addr), ntohs(addr_server->sin_port));
	if(bor_connect_in(socket, addr_server) < 0) return -1;
	printf("Connexion etablie \n");
	return 0; 
}

int server_talk(int socket){
	char buf[2048]; 
	int k; 
	printf("Lecture de la sortie... \n");
	k = bor_read_str(socket, buf, sizeof(buf));

	if(k <= 0) return k;
	printf("Client a reçu %d octets : %s\n", k, buf);
	return 1; 
}

int main(int argc, char *argv[]) {

	if(argc < 2) { printf("Il manque des arguments !\n"); exit(1); }

	struct sockaddr_in adr_client;
	struct sockaddr_in addr_server;

	int k; 
	char * server_name = argv[1];

	int socket = bor_create_socket_in(SOCK_STREAM, 0, &adr_client); 
	if(socket < 0) exit(1);
	if(bor_resolve_address_in(server_name, 13, &addr_server) < 0){ 
		close(socket);
		perror("Connexion terminée");
		exit(1); 
	} 
	if(server_connection(socket, &addr_server) < 0){
		close(socket);
		perror("Connexion terminée");
		exit(1); 
	}

	while(1){
		k = server_talk(socket);
		if(k <= 0) break;
	}

	close(socket);
	exit(k < 0 ? 1 : 0);
	return 0;
}