#include "bor-util.h"
 
int connecter_au_serveur(int soc, struct sockaddr_in* sas) {
    printf("Connexion avec %s: %d...\n",  inet_ntoa(sas->sin_addr), ntohs(sas->sin_port));
    if(bor_connect_in(soc,sas) < 0) return -1;
    printf("Connexion établie\n");
    return 0;
}
 
int fin_de_ligne(char* buf, int pos, int k) {
    for(int i=pos; i<k+pos; i++)
        if(buf[i]=='\n') return i;
    return pos+k;
}
 
void afficher_ligne(char* buf, int pos) {
    printf("%.*s", pos+1, buf);
    memmove(buf, buf+pos+1, strlen(buf)-pos);
}
 
int dialoguer_avec_serveur(int soc, char* buf, int pos) {
    printf("Client: lecture...\n");
   
    int k = bor_read_str(soc,buf+pos,sizeof(buf));
    if(k<=0) return k;
   
    printf("Récéption de %d octets : %s", k, buf);
   
    while(1) {
        pos = fin_de_ligne(buf, pos, k);
        if(buf[pos]!='\n') break;
       
        afficher_ligne(buf,pos);
        pos = 0;
        k = strlen(buf);
    }
    return 1;
}
 
int main(int argc, char* argv[]) {
    if(argc < 3 && argc > 3) { printf("Il manque des arguments !\n"); exit(1); }

    char* nom_serveur = argv[1];
    int port_serveur = atoi(argv[2]);
    
    int k = 0;
    struct sockaddr_in sas, sac;
    int soc = bor_create_socket_in(SOCK_STREAM,0,&sac);
    
    if(soc < 0) exit(1);
    if(bor_resolve_address_in(nom_serveur,port_serveur,&sas)<0) exit(1);
    if(connecter_au_serveur(soc,&sas)<0) { close(soc); exit(1); }
   
    char buf[2048];
    int pos = 0;
   
    while(1) {
        k = dialoguer_avec_serveur(soc, buf, pos);
        if(k<=0) break;
    }
   
    close(soc);
    exit(k<0?1:0);
}