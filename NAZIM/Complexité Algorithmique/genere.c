
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


int main (int nargs, char ** args)
{
   int n, max;

   if (nargs != 3)
   {
      fprintf(stderr, "Usage : genere <nbval> <maxval>\n");
      exit(1);
   }
   
   srand(time(NULL));

   n = atoi(args[1]);
   max = atoi(args[2]);

   while (n -- > 0)
      printf("%d\n", rand()%(max+1));
   
   return 0;
}


