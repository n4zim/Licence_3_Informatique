#include <sys/types.h>
#include <sys/stat.h>

/*
int main(int argc, char const *argv[])
{
	if(argc == 2)
	{
		if(mkfifo(argv[0], O_RDONLY) == 0)
		{

		}
		else
			exit(1);
	}
	else
	{
		perror("Erreur arguments");
		exit(1);
	}

	exit(0);
}
*/

typedef struct
{
	char * tub_ec_nom;
	char tub_sc_nom[100];
	char tub_cs_nom[100];
	int tub_sc;
	int tub_cs;
}Client;

int creer_tubes_service(Client *c)
{
	int k;
	sprintf(c->tub_sc_nom, "tub_sc_%d", getpid());
	k = mkfifo(c->tub_sc_nom, 0600);

	if(k < 0)
	{
		perror("mkfifo ...");
		return (-1);
	}

	sprintf(/* ... */);
	k = mkfifo( /* ... */);

	if(k < 0)
	{
		perror("mkfifo ...");
		unlink(c->tub_sc_nom);
		return (-1);
	}

	return 0;
}

int ouvrir_tubes_service(Client * c)
{
	c->tub_sc = open(c->tub_sc_nom, O_RDONLY);

	if(c->tub_sc < 0)
	{
		perror("open");
		supprimer_tubes_service(c);
		return(-1);
	}

	c->tub_cs = open(c->tub_cs_nom, O_WRONLY);

	if(c-> tub_cs < 0)
	{
		perror("open"); //close(c->tub_sc);
		supprimer_tubes_service(c);
		return (-1);
	}

	return (0);
}

int fermer_tubes_service(Client * c)
{
	close(c->tub_sc);
	close(c->tub_cs);
	return 0;
}

void supprimer_tubes_service(Client * c)
{
	unlink(c->tub_sc_nom);
	unlink(c->tub_cs_nom);
}

int prendre_contact(Client * c)
{
	int k;
	int n;
	k = open(c->tub_ec_nom, O_WRONLY);

	if(k < 0)
	{
		perror("open");
		supprimer_tubes_service(c);
		return (-1);
	}
	/* ... */
	n = bor_write_str(k, buf);
	/* ... */

	int dialogue(Client * c)
	{
		int k;
		char buf[1024];
		printf("Ecrivez ...");
		k = bor_read_str(0, buf, sizeof(buf)-1);

		if(k <= 0)
		{
			return k;
		}

		k = bor_write_str(c->tub_cs, buf);

		if(k <= 0)
		{
			return k;
		}

		k = bor_read_str(c->tub_sc, buf, sizeof(buf)-1);

		if(k <= 0)
		{
			return k;
		}

		sprintf("Reponse= %s/n", buf);

		return 1;
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	Client c;

	if(argc < 1)
	{
		/* Message erreur */
		perror("Erreur arguments");
		exit(1);
	}

	c.tub_ec_nom = argv[1];

	if(creer_tubes_service(&c) < 0)
		exit(1);

	if(prendre_contact(&c) < 0)
	{
		/* ? */
	}

	if(ouvrir_tubes_service(&c) < 0)
	{
		/* ? */
	}

	/* Jeu */

	fermer_tubes_service(&c);
	supprimer_tubes_service(&c);

	exit(0);
}