// pour compiler : gcc -o exo1 exo1.c
// pour executer : ./exo1 [arg]
//#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int TraduireEnTxt(char *pdf)
{
	int pid, res, ret;
	pid = fork();
		
	if (pid == 0)
	{
		//phrase = strcat(phrase, Commande[i]);
		execlp("pdftotext", "pdftotext", pdf, NULL); 
		exit(254);
	}
	
	else if (pid > 0)
	{
		res = wait(&ret);
		return WEXITSTATUS(ret);
	}
	else { return 255; }
}

char* getTitre(FILE* fichier)
{	
	char * titre = (char*)malloc(1000*sizeof(char));
	char * ligne = (char*)malloc(1000*sizeof(char));
	int cpt = 0;
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1) && cpt < 2)
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
			//printf("test %d : %s\n", cpt, buffer);
			strcat(titre, buffer);
			strcat(titre, " ");
			buffer = strtok(NULL," \t\n"); 
		} 
		cpt++;
	}
	if (ligne) free(ligne);
	return titre;
}

char* getAuteur(FILE* fichier)
{	
	//char ** auteurTab = (char**)malloc(10*sizeof(char*));
	char * auteur = (char*)malloc(1000*sizeof(char));
	char * ligne = (char*)malloc(1000*sizeof(char));
	int cpt = 0;
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1) && cpt < 10)
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
			if (strcasestr(buffer,"Abstract")!= NULL) return auteur;
			//printf("test %d : %s\n", cpt, buffer);
			strcat(auteur, buffer);
			strcat(auteur, " ");
			buffer = strtok(NULL," \t\n"); 
		} 
		cpt++;
	}
	if (ligne) free(ligne);
	return auteur;
}

char* getAbstract(FILE* fichier)
{	
	char * abstract = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(1000*sizeof(char));
	int estAbstract = 0;
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{			
			if ((strcasestr(buffer,"ntroduction")!= NULL) || (strcasestr(buffer,"words:")!= NULL)) return abstract;
			if(estAbstract == 1)
			{
				strcat(abstract, buffer);
				strcat(abstract, " "); 
			}
			if (strcasestr(buffer,"Abstract")!= NULL)
			{
				estAbstract = 1;
			}

			buffer = strtok(NULL," \t\n");
		}
	}
	if (ligne) free(ligne);
	return abstract;
}
char* getReferences(FILE* fichier)
{	
	char * references = (char*)malloc(100000*sizeof(char));
	char * ligne = (char*)malloc(1000*sizeof(char));
	int estAcknowledgments = 0;
	int estConclusion = 0;
	int estReferences = 0;
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{			
			if(estReferences == 1 && (estConclusion == 1 || estAcknowledgments == 1))
			{
				strcat(references, buffer);
				strcat(references, " "); 
			}
			if ((strcasestr(buffer,"Acknowledgments")!= NULL))
			{
				//On a passé la conclusion
				estAcknowledgments = 1;
			}
			if ((strcasestr(buffer,"Conclusion")!= NULL))
			{
				//On a passé la conclusion
				estConclusion = 1;
			}
			if (strcasestr(buffer,"References")!= NULL)
			{
				//On est arrivé aux réferences
				estReferences = 1;
			}

			buffer = strtok(NULL," \t\n");
		}
	}
	if (ligne) free(ligne);
	return references;
}
int main(int argc, char **argv)
{
	if (!argv[1]) { 
		printf("Erreur : Argument '-t' ou '-x' manquant\n");
	}
	else {
		
		//à cause de xargs on lance : ./test -t(argv[1]) nomDufichier(argv[2])
		char *pdf = argv[2];
		char *nomFichier = (char*)malloc(1000*sizeof(char));
		FILE *fichier = NULL;
		FILE *txt = NULL;
		FILE *xml = NULL;
		
		//TraduireEnTxt(pdf);
		strcpy(nomFichier, argv[2]);

		printf("%s\n", argv[2]);
		
		fichier = fopen(argv[2], "r");
		
		if (strcmp(argv[1],"-t")==0) { // si -t

			txt = fopen(strcat(nomFichier, "Resume.txt"), "wb");
			
			// nom fichier
			fwrite(strcat(pdf,"\n"), 1, strlen(pdf)+1, txt);
			
			// titre article
			char * titre = getTitre(fichier);
			fwrite(strcat(titre,"\n"), 1, strlen(titre)+1, txt);
			
			// resume (abstract)
			char *abstract = getAbstract(fichier);
			fwrite(strcat(abstract,"\n"), 1, strlen(abstract)+1, txt);
			
			// references
			char *references = getReferences(fichier);
			fwrite(strcat(references,"\n"), 1, strlen(references)+1, txt);			
			
			fclose(txt);
			
		}
		else if (strcmp(argv[1],"-x")==0) { // si -x
			
			xml = fopen(strcat(nomFichier, "Resume.xml"), "wb");
			char* balise = (char*)malloc(100*sizeof(char));

			strcpy(balise, "<article>");
			fwrite(balise, 1, sizeof(balise)+1, xml);
			fwrite("\n\t", 1, sizeof(char)+1, xml);

			// nom fichier d'origine
			strcpy(balise, "<preamble>");
			fwrite(balise, 1, sizeof(balise)+2, xml); //+2 sinon n'écrit pas tout 

			//fwrite(pdf, 1, strlen(pdf), xml);
			fwrite(strcat(pdf,"</preamble>\n\t"), 1, strlen(pdf)+strlen("</preamble>")+2, xml); //+2 pour \n\t
			
			// titre article
			strcpy(balise, "<titre>");
			fwrite(balise, 1, sizeof(balise)-1, xml); //-1 sinon écrit trop 

			char * titre = getTitre(fichier);
			fwrite(strcat(titre,"</titre>\n\t"), 1, strlen(titre)+strlen("<\titre>")+3, xml);
			
			// auteurs + adresses
			strcpy(balise, "<auteur>");
			fwrite(balise, 1, sizeof(balise), xml);
		
			char* auteur = getAuteur(fichier);
			//n'écrit pas tout avec la ligne dessous pour une raison obscure
			//fwrite(strcat(auteur,"</auteur>\n\t"), 1, strlen(titre)+strlen("<\titre>"), xml); 
			fwrite(auteur, 1, strlen(auteur), xml);

			strcpy(balise, "</auteur>");
			fwrite(balise, 1, sizeof(balise)+1, xml);

			fwrite("\n\t", 1, sizeof(char)+1, xml);
			
			// resume (abstract)
			strcpy(balise, "<abstract>");
			fwrite(balise, 1, sizeof(balise)+2, xml);

			char* abstract = getAbstract(fichier);
			fwrite(abstract, 1, strlen(abstract), xml);

			strcpy(balise, "</abstract>");
			fwrite(balise, 1, sizeof(balise)+3, xml);

			fwrite("\n\t", 1, sizeof(char)+1, xml);

			// references bibliographiques
			strcpy(balise, "<biblio>");
			fwrite(balise, 1, sizeof(balise), xml);

			char* biblio = getReferences(fichier);
			fwrite(biblio, 1, strlen(biblio), xml);

			strcpy(balise, "</biblio>");
			fwrite(balise, 1, sizeof(balise)+1, xml);

			fwrite("\n", 1, sizeof(char), xml);


			strcpy(balise, "</article>");
			fwrite(balise, 1, sizeof(balise)+2, xml);
			
		}
		else
		{
			printf("%s\n", argv[1]);
			printf("Erreur : Argument '-t' (texte) ou '-x' (xml) manquant\n");
		} 
		
		
		
		fclose(fichier);
	}
	return 0;
}

