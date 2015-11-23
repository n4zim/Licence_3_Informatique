#include "bor-util.h"

typedef enum {E_LIBRE, E_LIRE_REQUETE, E_ECRIRE_REPONSE} Etat;

typedef struct {
	Etat etat;
	int soc;				/* socket de service , default -1 */
	struct sockaddr_in adr; /* adresse du client */ 
} Slot;

#define SLOTS_NB 32

typedef struct {
	Slot slots[SLOTS_NB];
	int soc_ec; 			/* socket d'ecoute */
	struct sockaddr_in adr; /* adresse du serveur */
} Serveur;

void init_slot(Slot *o) {
	o->etat = E_LIBRE; 
	o->soc = -1;
	memset(&o->adr, 0, sizeof(o->adr)); 
}

int slot_est_libre(Slot *o) {
	if(E_LIBRE == o->etat) return 1;
	return 0;
}

void liberer_slot(Slot *o) {
	if(slot_est_libre(o)) return; 
	close(o->soc);
	init_slot(o);
}

void init_serveur(Serveur *ser) {
	for(int i = 0; i < SLOTS_NB; i++) init_slot(&ser->slots[i]);
	ser->soc_ec = -1;
	memset(&ser->adr, 0, sizeof(ser->adr));
}

int chercher_slot_libre(Serveur *ser) {
	for(int i = 0; i < SLOTS_NB; i++)
		if(slot_est_libre(&ser->slots[i])) return i;
	return -1;
}

int demarrer_serveur(Serveur *ser, int port) {
	init_serveur(ser);
	ser->soc_ec = bor_create_socket_in(SOCK_STREAM, port, &ser->adr);
	if(bor_listen(ser->soc_ec, SLOTS_NB) < 0) return -1;
	return ser->soc_ec;
}

void fermer_serveur(Serveur *ser) {
	close(ser->soc_ec);
	for(int i = 0; i < SLOTS_NB; i++) liberer_slot(&ser->slots[i]);
}

int accepter_connexion(Serveur *ser) {
	struct sockaddr_in adr_cli; 
	if(bor_accept_in(ser->soc_ec, &adr_cli) < 0) return 1; 
	
	int i = chercher_slot_libre(ser); 
	if(i < 0) {
		fermer_serveur(ser);
		return -1; 
	}

	ser->slots[i].etat = E_LIRE_REQUETE;
	ser->slots[i].adr = adr_cli;
	
	return 0; 
}

int proceder_ecriture_reponse(Slot *o) {
	char buf[2048];
	int k;
	//buf = "<html><body><h1>Serveur en construction !</html></body></h1>\n";
	k = bor_write_str(o->soc, buf);
	if(k >= 0) o->etat = E_LIRE_REQUETE;
	return k;
}

int proceder_lecture_requete(Slot *o) {
	char buf[2048];
	int k = bor_read_str(o->soc, buf, sizeof(buf));
	if(k >= 0) o->etat = E_ECRIRE_REPONSE;
	return k;
}

void traiter_slot_si_eligible(Slot *o, fd_set *set_read, fd_set *set_write) {
	int k = 0;
	if((o->etat == E_LIRE_REQUETE) && FD_ISSET(o->soc, set_read)) k = proceder_lecture_requete(o);
	if((o->etat == E_ECRIRE_REPONSE) && FD_ISSET(o->soc, set_write)) k = proceder_ecriture_reponse(o);
	if(k < 0) liberer_slot(o);
}

void preparer_select(Serveur *ser, int *maxfd, fd_set *set_read, fd_set *set_write) {
	FD_ZERO(set_read);
	FD_ZERO(set_write);
	*maxfd = ser->soc_ec;
	FD_SET(ser->soc_ec, set_read);
	for(int i = 0; i < SLOTS_NB; i++) {
		if(ser->slots[i].etat == E_LIRE_REQUETE) FD_SET(ser->slots[i].soc, set_read);
		if(ser->slots[i].etat == E_ECRIRE_REPONSE) FD_SET(ser->slots[i].soc, set_write);
		if(ser->slots[i].soc > *maxfd) *maxfd = ser->slots[i].soc;
	}
}

int faire_scrutation(Serveur *ser) {
	fd_set set_read, set_write;
	int maxfd;

	preparer_select(ser, &maxfd, &set_read, &set_write);

	printf("Scrutation.\n");
	int r = select(maxfd+1, &set_read, &set_write, NULL, NULL);

	if(r > 0) {
		printf("Scrutage %d\n", r);
		if(FD_ISSET(ser->soc_ec, &set_read)) accepter_connexion(ser);
		for(int i = 0; i<SLOTS_NB; i++) traiter_slot_si_eligible(&ser->slots[i], &set_read, &set_write);    
	}
	return r;
}

int boucle = 1;
void capter_signal(int sig){
	printf("Signal %d capté.\n", sig);
	boucle = 0;
}

int main(int argc, char *argv[]) {
	if(argc < 2) { printf("Il manque le port en argument !\n"); exit(1); }
	int server_port = atoi(argv[1]);

	Serveur ser;
	demarrer_serveur(&ser, server_port);

	bor_signal(SIGINT, capter_signal, 0);
	bor_signal(SIGPIPE, SIG_IGN, SA_RESTART);

	while(boucle) faire_scrutation(&ser);

	fermer_serveur(&ser);	
}