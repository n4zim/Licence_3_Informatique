#include "bor-util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int server_connection(int socket, struct sockaddr_in *addr_server){
	printf("connexion avec%s : %d \n", inet_ntoa(addr_server->sin_addr), ntohs(addr_server->sin_port));
	if(bor_connect_in(socket, addr_server) < 0) return -1;
	printf("Connexion etablie \n");
	return 0;
}

int server_read(int socket, char * outputFileName){
	char buf[2048]; 
	int k; 
	printf("Lecture de la sortie... \n");
	k = bor_read_str(socket, buf, sizeof(buf));

	if(k <= 0) return k;
	printf("Client a reçu %d octets : %s\n", k, buf);

	FILE *fp;
	if((fp = fopen(outputFileName, "w")) == NULL) {
		printf("Erreur d'ouverture du fichier\n");
		exit(1);
	}

	strcat(buf, "\n");
	fputs(buf, fp);

	return 1; 
}

int server_talk(int socket, char string[]) {
	char buf[2048];
	memcpy(buf, "GET /", 5);
	strcat(buf, string);
	strcat(buf, " HTTP/1.0\n\n");
	if(bor_write_str(socket, buf) <= 0) return -1;
	return 1;
}

int main(int argc, char *argv[]) {
	if(argc < 5) { printf("Il manque des arguments !\n"); exit(1); }

	struct sockaddr_in adr_client;
	struct sockaddr_in addr_server;

	int k;
	char * server_name = argv[1];
	int server_port = atoi(argv[2]);
	char * string = argv[3];
	char * outputFileName = argv[4];

	int socket = bor_create_socket_in(SOCK_STREAM, 0, &adr_client);
	if(socket < 0) exit(1);
	
	if(bor_resolve_address_in(server_name, server_port, &addr_server) < 0) {
		close(socket);
		perror("Connexion terminée");
		exit(1);
	} 
	
	if(server_connection(socket, &addr_server) < 0) {
		close(socket);
		perror("Connexion terminée");
		exit(1);
	}

	if(server_talk(socket, string) <= 0) {
		printf("Erreur lors de l'envoi des données\n");
		exit(1);
	}

	while(1) {
		k = server_read(socket, outputFileName);
		if(k <= 0) break;
	}

	close(socket);
	exit(k < 0 ? 1 : 0);
	return 0;
}