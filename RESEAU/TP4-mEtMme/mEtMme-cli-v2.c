#include "bor-util.h"

typedef struct{
	char* serverPipe;
	char readPipe[100];
	char writePipe[100];
	int readFd;
	int writeFd;
} Client;

int launch(Client* client) {
	char buf[1024];
	for(;;) {
		int k = bor_read_str(0, buf, 1024);
		if(k == 0) return 0;
		if(k < 0) return k;
		if(bor_write_str(client->writeFd, buf) <= 0) return -1;
		if(bor_read_str(client->readFd, buf, 1024) <= 0) return -1;
		printf("Response : %s\n", buf);
	}
}

int createPipe(Client* client) {
	puts("Creating pipes...");
	
	sprintf(client->readPipe, "read_%d", getpid());
	if(mkfifo(client->readPipe, 0600) < 0) { perror("mkfifo"); return -1; }

	sprintf(client->writePipe, "write_%d", getpid());
	if(mkfifo(client->writePipe, 0600) < 0) { perror("Mkfifo Error"); return -1; }

	return 0;
}

int openPipe(Client* client) {
	puts("Opening pipes...");

	client->readFd = open(client->readPipe, O_RDONLY);
	if(client->readFd < 0){ perror("Open Error"); return -1; }

	client->writeFd = open(client->writePipe, O_WRONLY);
	if(client->writeFd < 0) { perror("Open Error"); return -1; }

	return 0;
}

int connect(Client* client) {
	puts("Connecting...");

	int fd = open(client->serverPipe, O_WRONLY);
	if(fd < 0) { perror("Open Error"); return -1; }

	char buf[100];
	sprintf(buf, "%s\n%s\n", client->readPipe, client->writePipe);
	if(bor_write_str(fd, buf) < 0) return -1;

	close(fd);
	return 0;
}

void resetClient(Client* client) {
	client->readFd = -1;
	client->writeFd = -1;
	client->readPipe[0] = 0;
	client->writePipe[0] = 0;
}

void closeClient(Client* client) {
	if(client->readFd != -1) close(client->readFd);
	if(client->writeFd != -1) close(client->writeFd);
	if(client->readPipe[0] != 0) unlink(client->readPipe);
	if(client->writePipe[0] != 0) unlink(client->writePipe);
	resetClient(client);
}

int main(int argc, char** argv) {
	if(argc < 2) { puts("Bad arguments"); exit(1); }

	Client client;
	client.serverPipe = argv[1];
	resetClient(&client);

	if(createPipe(&client) < 0) { closeClient(&client); exit(1); }
	if(connect(&client) < 0) { closeClient(&client); exit(1); }
	if(openPipe(&client) < 0) { closeClient(&client); exit(1); }

	int error = launch(&client);
	closeClient(&client);
	exit(-error);	
}
