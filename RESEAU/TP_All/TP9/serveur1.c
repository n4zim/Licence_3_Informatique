#include "bor-util.h"

#define SLOTS_NB 32
#define REQSIZE 1500

typedef enum{
	E_LIBRE,
	E_LIRE_REQUETE,
	E_ECRIRE_REPONSE
} Etat;

typedef struct{
	Etat etat;
	int soc;
	struct sockaddr_in adr;
} Slot;

typedef struct{
	Slot slots[SLOTS_NB];
	int soc_ec;
	struct sockaddr_in adr;
} Serveur;

int boucle = 1;

void capter(int sig){
	printf("Signal %d capté\n", sig);
	boucle = 0;
	}

void init_slot(Slot *o)
{
	o->etat = E_LIBRE;
	o->soc = -1;
	memset(&o->adr,0,sizeof(o->adr));
	o->req[0] = '\0';
	o->req_pos = 0;
	o->fin_entete = 0;
}

int slot_est_libre(Slot*o)
{
	if(o->etat == E_LIBRE)
		return 1;
	return 0;
}

void liberer_slot(Slot *o)
{
	if(slot_est_libre(o)==1) return;
	
	close(o->soc);
	init_slot(o);
}

int chercher_slot_libre(Serveur *ser)
{
	for(int i=0; i < SLOTS_NB; i++)
	{
		if(slot_est_libre(&ser->slots[i])==1)
			return i;
	}

	return -1;
}

void init_serveur(Serveur *ser)
{
	for(int i = 0; i < SLOTS_NB; i++)
	{
		init_slot(&ser->slots[i]);
	}

	ser->soc_ec = -1;
	memset(&ser->adr,0,sizeof(ser->adr));
}

int demarrer_serveur(Serveur *ser, int port)
{
	init_serveur(ser);
	ser->soc_ec = bor_create_socket_in(SOCK_STREAM, port, &ser->adr);
	if(bor_listen(ser->soc_ec,SLOTS_NB)<0)return -1;
	return ser->soc_ec;
}

void fermer_serveur(Serveur *ser)
{
	close(ser->soc_ec);

	int i;
	for(i=0; i < SLOTS_NB; i++)
	{
		liberer_slot(&ser->slots[i]);
	}
}

int accepter_connexion(Serveur *ser)
{
	printf("Connexion à client...\n");

	int soc = bor_accept_in(ser->soc_ec, &ser->adr);
	if(soc < 0){
		bor_perror("bor_accept_in");
	}

	int i = chercher_slot_libre(ser);
	if(i < 0)
	{
		//fermer le client
		return -1;
	}
	ser->slots[i].etat = E_LIRE_REQUETE;
	ser->slots[i].soc  = soc;
	return 0;
}

int proceder_lecture_requete(Slot* o)
{
	printf("Lecture...\n");
	char buf[2048];
	int k = bor_read_str(o->soc, buf, sizeof(buf));
	if(k<0)
	{
		perror("read");
		return k;
	}
	o->etat = E_ECRIRE_REPONSE;
	return k;
}

int proceder_ecriture_reponse(Slot* o)
{
	printf("Ecriture...\n");
	char buf[2048];
	strncpy(buf, "", 1);
	int k = bor_read_str(o->soc, buf, sizeof(buf));
	if(k<0)
	{
		perror("write");
		return k;
	}
	o->etat = E_LIRE_REQUETE;
	return k;
}

void traiter_slot_si_eligible(Slot *o, fd_set *set_read, fd_set *set_write)
{
	int k=0;
	if(o->etat == E_LIRE_REQUETE && FD_ISSET(o->soc, set_read))
	{
		k = proceder_lecture_requete(o);
	}

	if(o->etat == E_ECRIRE_REPONSE && FD_ISSET(o->soc, set_write))
	{
		k = proceder_ecriture_reponse(o);
	}

	if(k < 0) liberer_slot(o);
}

void preparer_select(Serveur *ser, int *maxfd, fd_set *set_read, fd_set *set_write)
{
	FD_ZERO(set_read);
	FD_ZERO(set_write);

	FD_SET(ser->soc_ec, set_read);
	*maxfd = ser->soc_ec;
	for (int i = 0; i < SLOTS_NB; i++)
	{
		if(ser->slots[i].etat != E_LIBRE)
		{
			if(ser->slots[i].etat == E_LIRE_REQUETE)
				FD_SET(ser->slots[i].soc, set_read);
			if(ser->slots[i].etat == E_ECRIRE_REPONSE)
				FD_SET(ser->slots[i].soc, set_write);
			if(ser->slots[i].soc > *maxfd)
				*maxfd = ser->slots[i].soc;
			
		}
	}
}

int faire_scrutation(Serveur *ser)
{
	fd_set set_read, set_write;
	int maxfd;
	
	preparer_select(ser, &maxfd, &set_read, &set_write);
	
	printf("Scrutation...\n");
	
	if(select(maxfd+1, &set_read, &set_write, NULL, NULL) < 0)
	{
		perror("select");
		return -1;
	}
		printf("Scrutation faite\n");
	if(accepter_connexion(ser)<0) return -1;
	
	for(int i=0;i<SLOTS_NB;i++)
		traiter_slot_si_eligible(&ser->slots[i], &set_read, &set_write);
		
	return 0;
}

int main(int argc, char const *argv[])
{
	bor_signal(SIGINT, capter, SA_RESTART);
	
	if(argc<2||argc>2)
	{
		perror("Nombre d'agruments incorrect\n");
		exit(1);
	}
	
	int port = atoi(argv[1]);
	Serveur ser;
	
	printf("Démarrage serveur...\n");
	
	if(demarrer_serveur(&ser, port)<0) exit(1);
	
	printf("Serveur démarré\n");
	
	while(boucle)
	{
		if(faire_scrutation(&ser)<0) break;
	}
	
	fermer_serveur(&ser);
	exit(0);
}
