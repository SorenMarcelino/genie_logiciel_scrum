#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv)
{
    //pour chaque pdf on lance pdftotext
    system("ls *.pdf | xargs -n1 pdftotext -nopgbrk -q");

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
        printf("Sprint3: Erreur : Argument '-t' (texte) ou '-x' (xml) manquant\n");

    //tous les txt créer avec pdftotext vont dans un dossier à part
    system("mv *.txt Test/");

    return 0;
}
