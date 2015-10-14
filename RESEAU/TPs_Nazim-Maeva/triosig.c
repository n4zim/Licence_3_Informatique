#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
 
int p1, p2;
void captepere (int sig) {
  printf ("pere recoit le signal\n");
  sleep (2);
  printf ("pere envoie le signal à son fils2 %d\n", p2);
  kill (p2, SIGUSR1);
}
 
void captefils1 (int sig) {
  printf ("fils1 recoit le signal\n");
  sleep (2);
  printf ("fils1 envoie le signal à son pere %d\n",getppid ());
  kill (getppid (), SIGUSR1);
}
 
void captefils2 (int sig) {
  printf ("fils2 recoit le signal\n");
  sleep (2);
  printf ("fils2 envoie le signal à son frere %d\n",p1);
  kill (p1, SIGUSR1);
}

void captemort (int sig) {
  printf ("pas de message pdt 5s\n");
  exit (0);
}

int bor_signal (int sig, void (*h), int options) {
  struct sigaction s;
  s.sa_handler = h;
  sigemptyset (&s.sa_mask);
  s.sa_flags = options;
  return sigaction (sig, &s, NULL);
}
 
int main (int argc, char ** argv) {
  printf ("je suis le pere %d\n", getpid ());
  
  /** fils1 */
  p1 = fork ();
  if (p1 < 0){perror("fork");exit (0);}
  if (p1 == 0)
    {
      bor_signal (SIGUSR1,captefils1, SA_RESTART);
      bor_signal (SIGALRM,captemort,NULL);
      
      printf ("je suis le fils1 %d\n", getpid ());
      
      while (1)
        { 
	  alarm (5);
	  pause ();
        }
      
      printf ("le fils1 meurt %d\n", getpid ());
      exit (0);
    }
  
  /** fils 2 */
  p2 = fork ();
  if(p2 < 0){perror("fork");exit (0);}
  if (p2 == 0)
    {
      bor_signal (SIGUSR1, captefils2, SA_RESTART);
      bor_signal (SIGALRM,captemort,NULL);
      
      printf ("je suis le fils2 %d\n", getpid ());
      printf ("fils2 envoie le signal à son frere %d\n",p1);
      kill (p1, SIGUSR1);       
      
      while (1)
        {
	  alarm (5);
	  pause ();
        }
      
      printf ("le fils2 meurt %d\n", getpid ());        
      exit (0);
    }
  
  bor_signal (SIGUSR1, captepere, SA_RESTART);
  bor_signal (SIGALRM,captemort,NULL);
  
  while (1)
    {
      alarm (5);
      pause ();
    }
  
  printf ("le pere meurt %d\n", getpid ());
  exit (0);
}
