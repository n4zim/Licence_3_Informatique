#include "bor-util.h"
#define REQSIZE 4096
#define REPSIZE 4096

typedef enum { E_LIBRE, E_LIRE_REQUETE, E_ECRIRE_REPONSE } Etat;
typedef struct {
    Etat etat;
    int  soc;
    struct sockaddr_in adr;
    char req[REQSIZE];
    int  req_pos, fin_entete;
    char rep[REPSIZE];
    int  rep_pos;
    int  fic_fd;
} Slot;


void init_slot(Slot *o) {
    o->etat = E_LIBRE;
    o->soc = -1;
    o->req[0] = '\0';
    o->req_pos = 0;
    o->fin_entete = 0;
    o->rep[0] = '\0';
    o->rep_pos = 0;
    o->fic_fd = -1;
}

int slot_est_libre(Slot *o) { return o->etat == E_LIBRE; }


void liberer_slot(Slot *o) {
    if(slot_est_libre(o)) return;
    close(o->soc);
    if(o->fic_fd >= 0) close(o->fic_fd);
    init_slot (o);
}


#define SLOTS_NB 32

typedef struct {
    Slot slots[SLOTS_NB];
    int soc_ec;
    struct sockaddr_in adr;
} Serveur;


void init_serveur(Serveur *ser) {
    for(int i = 0; i < SLOTS_NB; i++) init_slot (&ser->slots[i]);
    ser->soc_ec = -1;
}


int chercher_slot_libre(Serveur *ser) {
    for(int i = 0; i < SLOTS_NB; i++)
        if(slot_est_libre(&ser->slots[i])) return i;
    return -1;
}


int demarrer_serveur(Serveur *ser, int port) {
    init_serveur (ser);

    ser->soc_ec = bor_create_socket_in (SOCK_STREAM, port, &ser->adr);
    if(ser->soc_ec < 0) return -1;

    if(bor_listen (ser->soc_ec, 8) < 0) { close(ser->soc_ec); return -1; }
    return 0;
}


void fermer_serveur(Serveur *ser) {
    close(ser->soc_ec);
    for(int i = 0; i < SLOTS_NB; i++) liberer_slot(&ser->slots[i]);
}


int accepter_connexion(Serveur *ser) {
    struct sockaddr_in adr_client;
    
    printf("Serveur : Connexion en cours ...\n");
    int soc_se = bor_accept_in (ser->soc_ec, &adr_client);
    if(soc_se < 0) return -1;

    int i = chercher_slot_libre (ser);
    if(i < 0) {
        close(soc_se);
        printf("Serveur : %s est de trop, il y a trop de clients\n", bor_adrtoa_in(&adr_client));
        return 0;
    }

    printf("Serveur[%d] : Connexion avec %s\n", soc_se, bor_adrtoa_in(&adr_client));
    Slot *o = &ser->slots[i];
    o->soc = soc_se;
    o->adr = adr_client;
    o->etat = E_LIRE_REQUETE;
    return 1;
}

int proceder_lecture_requete(Slot *o);
int proceder_ecriture_reponse(Slot *o);

void traiter_slot_si_eligible(Slot *o, fd_set *set_read, fd_set *set_write) {
    int k = 1; 
    if(slot_est_libre(o)) return;

    switch(o->etat) {
        case E_LIRE_REQUETE : 
            if(FD_ISSET(o->soc, set_read))
                k = proceder_lecture_requete (o);
            break;
        case E_ECRIRE_REPONSE : 
            if(FD_ISSET(o->soc, set_write))
                k = proceder_ecriture_reponse (o); 
            break;
        default : ;
    }

    if(k <= 0) {
        printf("Serveur[%d] : Libération du slot\n", o->soc); 
        liberer_slot (o);
    }
}


void inserer_fd (int fd, fd_set *set, int *maxfd) {
    FD_SET (fd, set);
    if(*maxfd < fd) *maxfd = fd;
}


void preparer_select(Serveur *ser, int *maxfd, fd_set *set_read, fd_set *set_write) {
    FD_ZERO (set_read);
    FD_ZERO (set_write);
    *maxfd = -1;
    inserer_fd (ser->soc_ec, set_read, maxfd);

    for(int i = 0; i < SLOTS_NB; i++) {
        Slot *o = &ser->slots[i];
        if(slot_est_libre(o)) continue;

        switch(o->etat) {
            case E_LIRE_REQUETE   : inserer_fd (o->soc, set_read , maxfd); break;
            case E_ECRIRE_REPONSE : inserer_fd (o->soc, set_write, maxfd); break;
            default : ;
        }
    }
}


int faire_scrutation(Serveur *ser) {
    int maxfd;
    fd_set set_read, set_write;

    preparer_select (ser, &maxfd, &set_read, &set_write);

    printf("Attente...\n");
    int res = select (maxfd+1, &set_read, &set_write, NULL, NULL);

    if(res < 0) {
        if(errno == EINTR) return 0;
        perror ("select");  return -1;
    }

    if(FD_ISSET (ser->soc_ec, &set_read)) {
        if(accepter_connexion (ser) < 0) return -1;
    }

    for(int i = 0; i < SLOTS_NB; i++)
        traiter_slot_si_eligible (&ser->slots[i], &set_read, &set_write);

    return 0;
}


int boucle_princ = 1;
void capter_sigint (int sig) {
    printf("Serveur : Signal %d capté\n", sig);
    boucle_princ = 0;
}


int main (int argc, char *argv[]) {
    Serveur ser;
    int k = -1;

    if(argc-1 != 1) {
        fprintf(stderr, "Usage: %s port (0 pour libre)\n", argv[0]);
        exit (1);
    }
    int port_serveur = atoi (argv[1]);

    if(demarrer_serveur (&ser, port_serveur) < 0) exit (1);

    bor_signal (SIGINT, capter_sigint, 0);
    bor_signal (SIGPIPE, SIG_IGN, SA_RESTART);

    while (boucle_princ) {
        k = faire_scrutation (&ser);
        if(k < 0) break;
    }

    printf("Fin du serveur...\n");
    fermer_serveur (&ser);
    exit (k < 0 ? 1 : 0);
}


typedef enum {
    M_NONE,
    M_GET,
    M_TRACE,
} Id_methode;

typedef enum {
    C_OK = 200,
    C_BAD_REQUEST = 400,
    C_NOT_FOUND = 404,
    C_METHOD_UNKNOWN = 501,
} Code_reponse;

typedef struct {
    char methode[REQSIZE], url[REQSIZE], version[REQSIZE], chemin[REQSIZE];
    Id_methode id_meth;
    Code_reponse code_rep;
} Infos_entete;


char *get_http_error_message (Code_reponse code) {
    switch(code) {
        case C_OK             : return "OK";
        case C_BAD_REQUEST    : return "Bad Request";
        case C_NOT_FOUND      : return "Not Found";
        case C_METHOD_UNKNOWN : return "Method Unknown";
        default               : return "Other Error";
    }
}


Id_methode get_id_methode (char *methode) {
    if(!strcasecmp(methode, "GET")) return M_GET;
    if(!strcasecmp(methode, "TRACE")) return M_TRACE;
    return M_NONE;
}


int preparer_fichier(Slot *o, Infos_entete *ie) {
	if(strlen(ie->url) == 1){
		strcpy(ie->chemin, "index.html");
		return 0;
	}
    if(sscanf (ie->url, "/%[^?]", ie->chemin) != 1) return -1;

    printf("Ouverture \"%s\" ...\n", ie->chemin);
    o->fic_fd = open (ie->chemin, O_RDONLY);
    if(o->fic_fd < 0) { perror ("open"); return -1; }

    return 0;
}


void analyser_requete(Slot *o, Infos_entete *ie) {
    int n;
    int k = sscanf (o->req, "%s %s %s%n", ie->methode, ie->url, ie->version, &n);
    if(k != 3) { ie->code_rep = C_BAD_REQUEST; return; }

    if(strcasecmp(ie->version, "HTTP/1.0") && strcasecmp(ie->version, "HTTP/1.1"))
    { ie->code_rep = C_BAD_REQUEST; return; }

    ie->id_meth = get_id_methode (ie->methode);
    if(ie->id_meth == M_NONE) { ie->code_rep = C_METHOD_UNKNOWN; return; }

    if(ie->id_meth == M_GET) {
        k = preparer_fichier (o, ie);
        if(k < 0) { ie->code_rep = C_NOT_FOUND; return; }
    }
    ie->code_rep = C_OK;
}


void preparer_reponse(Slot *o, Infos_entete *ie) {
    char *mess = get_http_error_message (ie->code_rep);
    int pos = sprintf(o->rep, "HTTP/1.1 %d %s\n", ie->code_rep, mess);
    time_t t = time(NULL); // \n
    pos += sprintf(o->rep + pos, "Date: %s", ctime(&t));

    pos += sprintf(o->rep + pos, "Server: serweb2\nConnection: close\n");

    if(ie->code_rep != C_OK) {
        sprintf(o->rep + pos,
            "Content-Type: text/html; charset=UTF-8\n\n"
            "<html><head>\n"
            "<title>%d %s</title>\n"
            "</head><body>\n"
            "<h1>%s</h1>\n"
            "</body></html>\n",
            ie->code_rep, mess, mess);
        return;
    }

    if(ie->id_meth == M_TRACE) {
        snprintf(o->rep + pos, REPSIZE-pos, 
            "Content-Type: message/http\n\n"
            "%s", o->req);
        return;
    }

    if(ie->id_meth == M_GET) {
        sprintf(o->rep + pos,
            "Content-Type: text/html; charset=UTF-8\n\n"
            "<html><head>\n"
            "<title>Et ça marche !</title>\n"
            "</head><body>\n"
            "<p>Vous vous trouvez sur <stong>%s</strong> !\n"
            "</body></html>\n",
            ie->chemin);
        return;
    }

    fprintf(stderr, "preparer_reponse : Problème avec %s\n", ie->methode);
}


int chercher_fin_entete(Slot *o, int debut) {
    for(int i = debut; o->req[i] != '\0'; i++)
        if((o->req[i  ] == '\n' && o->req[i+1] == '\n') ||
            (o->req[i  ] == '\r' && o->req[i+1] == '\n' &&
             o->req[i+2] == '\r' && o->req[i+3] == '\n')) return i;
    return -1;
}


int lire_suite_requete(Slot *o) {
    int k = bor_read_str (o->soc, o->req + o->req_pos, REQSIZE - o->req_pos);
    if(k > 0) o->req_pos += k;
    return k;
}


int proceder_lecture_requete(Slot *o) {
    Infos_entete ie;

    int prec_pos = o->req_pos;
    int k = lire_suite_requete (o);
    if(k <= 0) return -1;

    int debut = prec_pos-3 ; if(debut < 0) debut = 0;
    o->fin_entete = chercher_fin_entete (o, debut);
    if(o->fin_entete < 0) { 
        printf("Serveur[%d] : Erreur avec une requette !\n", o->soc); 
        return 1;
    }

    printf("Serveur[%d] : \"%s\"\n", o->soc, o->req);
    analyser_requete (o, &ie);
    preparer_reponse (o, &ie);
    o->etat = E_ECRIRE_REPONSE;
    return 1;
}


int ecrire_suite_reponse(Slot *o) {
    int k = bor_write_str (o->soc, o->rep + o->rep_pos);
    if(k > 0) o->rep_pos += k;
    return k;
}


int proceder_ecriture_reponse(Slot *o) {
    int k = ecrire_suite_reponse (o);
    if(k < 0) return -1;

    if(o->rep_pos < (int) strlen (o->rep)) {
        printf("Serveur[%d] : Problème de réponse\n", o->soc); 
        return 1;
    }

    if(o->fic_fd == -1) return -1;
    return 0;
}