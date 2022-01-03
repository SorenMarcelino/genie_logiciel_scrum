#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

using namespace std;

#define MAX_ARG 100

char **Ligne2Argv(char* commande)
{
    char **arg; 
    char *li; 
    int i; 
    char *p; 

    li = strdup(commande);
    arg = (char **)malloc(MAX_ARG * sizeof(char *));
    p = strtok(li," \t\n"); 
    i=0;

    while (p) 
    { 
        arg[i]=strdup(p); 
        i++; 
        p = strtok(NULL," \t\n"); 
    } 
    arg[i]=NULL; 
    return arg; 
}



char *** File2TabArgv(char* f, int* nb_commande)
{
    char *** commandes = (char***)malloc(MAX_ARG * sizeof(char**));
    FILE* fichier = fopen(f, "r");
    char* commande = (char*)malloc(MAX_ARG * sizeof(char));
    int i = 0;

    if(fichier != NULL)
    {
        while(fgets(commande, MAX_ARG, fichier) != NULL)
        {
            commandes[i] = Ligne2Argv(commande);
            i++;
        }
    }
    *nb_commande = i;
    return commandes;
}


int main(int argc, char ** argv)
{
    system("ls *.pdf > listePDF");
    
    int nbCommandes = 0;
    char *** listePDF = (char***)malloc(MAX_ARG * sizeof(char**));
    char* nomFichierListePDF = (char*)malloc(MAX_ARG * sizeof(char));
    char* choix = (char*)malloc(MAX_ARG * sizeof(char));;
    char *commande = (char*)malloc(MAX_ARG * sizeof(char));
    char* fichier = (char*)malloc(MAX_ARG * sizeof(char));
    
    strcpy(commande, "pdftotext xargs -n1 pdftotext -nopgbrk -q");
    
    strcpy(nomFichierListePDF, "listePDF");

    listePDF = File2TabArgv(nomFichierListePDF, &nbCommandes);
    
    do
    {			
		for(int i =0; i < nbCommandes; i++)
		{
			printf("fichier n° %d : %s\n", i+1, listePDF[i][0]);
		}
		
		printf("Choisissez le ou les fichier à convertir (q pour quitter) : ");
		cin >> choix;
		
		if(choix[0] != 'q')
		{
			int test = atoi(choix);
			printf("Le fichier choisit : %s\n", listePDF[test-1][0]);
            int pid,res,ret;

            pid = fork();
            if(pid==0)
            {
                execlp("pdftotext", "pdftotext", listePDF[test-1][0], "-nopgbrk", "-q", NULL);
                exit(254);
            }
            else if(pid >0)
            {
                res = wait(&ret);
            }
		}		
		
	}while(choix[0] != 'q');

    if(strcmp(argv[1],"-t")==0)
    {
        //pour chaque txt on lance notre code d'extraction
        system("ls *.txt | xargs -n1 test -t");
        //on met tous les résumé dans un dossier Resume
        system("mv *Resume.txt* Resume/");
    }
        
    else if (strcmp(argv[1],"-x")==0)
    {
        //pour chaque txt on lance notre code d'extraction
        system("ls *.txt | xargs -n1 test -x");
        //on met tous les résumé dans un dossier Resume
        system("mv *Resume.xml* Resume/");

    }
    else
    {
        printf("Sprint3: Erreur : Argument '-t' (texte) ou '-x' (xml) manquant\n");
    }
        

    //tous les txt créer avec pdftotext vont dans un dossier à part
    system("mv *.txt Test/");

    return 0;
}
