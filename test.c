// pour compiler : gcc -o test test.c
// pour executer : ./test [arg]

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{	
	time_t debut;
	time_t fin;
	char ligne[256];
	FILE *fichier;
	
	time(&debut);
	printf("boucle: ");
	for (int i=0; i<10000; i++)
	{
		fichier = fopen("Boudin-Torres-2006.txt", "r");
		while (fgets(ligne, 255, fichier)) { printf("%s ", ligne); }
		fclose(fichier);
		
		printf("%d ", i);
	}
	time(&fin);
	
	printf("\nTemps: %ld secondes en c \n", fin - debut);
	// 23 secondes
}
