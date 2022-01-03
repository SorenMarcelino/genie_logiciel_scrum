//pour tester : ./test [-x/-t] [article]
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
	char * test = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(1000*sizeof(char));
	int estAbstract = 0;
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{			
			if ((strcasestr(buffer,"ntroduction")!= NULL) || (strcasestr(buffer,"words:")!= NULL)) 
			{
				free(ligne);
				return abstract;
			}
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

char* getIntroduction(FILE* fichier)
{	
	char * intro = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(10000*sizeof(char));
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
			if ((strcasestr(buffer,"2.")!= NULL) || (strcasestr(buffer,"2\n")!= NULL) || (strcasestr(buffer,"II.")!= NULL) )return intro;
			strcat(intro, buffer);
			strcat(intro, " ");
			buffer = strtok(NULL," \t\n"); 
		}		
	}
	if (ligne) free(ligne);
	return intro;
}

char* getCorps(FILE* fichier, int *zone)
{	
	char * corps = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(10000*sizeof(char));
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
			if (strcasestr(buffer,"Conclusion")!= NULL) {
				*zone = 1; // la zone est définis comme étant la partie conclusion
				return corps;
			}
				
			else if (strcasestr(buffer,"Discussion")!= NULL){
				*zone = 2; // la zone est définis comme étant la partie Discussion
				return corps;
			}
				
			else if (strcasestr(buffer,"References")!= NULL){
				*zone = 3; // la zone est définis comme étant la partie References
				return corps;
			}
			strcat(corps, buffer);
			strcat(corps, " ");
			buffer = strtok(NULL," \t\n"); 
		}		
	}
	if (ligne) free(ligne);
	return corps;
}

char* getConclusion(FILE* fichier, int *zone)
{	
	char * conclu = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(10000*sizeof(char));
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
				
			if (strcasestr(buffer,"Discussion")!= NULL){
				*zone = 2; // la zone est définis comme étant la partie Discussion
				return conclu;
			}
				
			else if (strcasestr(buffer,"References")!= NULL){
				*zone = 3; // la zone est définis comme étant la partie References
				return conclu;
			}
			strcat(conclu, buffer);
			strcat(conclu, " ");
			buffer = strtok(NULL," \t\n"); 
		}		
	}
	if (ligne) free(ligne);
	return conclu;
}

char* getDiscussion(FILE* fichier, int *zone)
{	
	char * discu = (char*)malloc(10000*sizeof(char));
	char * ligne = (char*)malloc(10000*sizeof(char));
	size_t len = 0;
	
	while (((getline(&ligne, &len, fichier)) != -1))
	{
		char * buffer = strtok(ligne," \t\n"); 
		while (buffer) 
		{ 
			if (strcasestr(buffer,"Conclusion")!= NULL) {
				*zone = 1; // la zone est définis comme étant la partie conclusion
				return discu;
			}
				
			else if (strcasestr(buffer,"References")!= NULL){
				*zone = 3; // la zone est définis comme étant la partie References
				return discu;
			}
			strcat(discu, buffer);
			strcat(discu, " ");
			buffer = strtok(NULL," \t\n"); 
		}		
	}
	if (ligne) free(ligne);
	return discu;
}

// biblio
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
			

			buffer = strtok(NULL," \t\n");
		}
	}
	if (ligne) free(ligne);
	return references;
}
/*
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
}*/
 

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

			free(abstract);

			strcpy(balise, "</abstract>");
			fwrite(balise, 1, sizeof(balise)+3, xml);

			fwrite("\n\t", 1, sizeof(char)+1, xml);

	
			// introduction	
			strcpy(balise, "<introduction>");
			fwrite(balise, 1, sizeof(balise)+6, xml);

			char* intro = getIntroduction(fichier);
			fwrite(abstract, 1, strlen(intro), xml);

			free(intro);

			strcpy(balise, "</introduction>");
			fwrite(balise, 1, sizeof(balise)+8, xml);

			fwrite("\n\t", 1, sizeof(char)+1, xml);
			
			
			// Pour determiner dans quelle sous partie on est 
			int zone = 0;
			
			// corps
			strcpy(balise, "<corps>");
			fwrite(balise, 1, sizeof(balise)-1, xml);

			char* corps = getCorps(fichier, &zone);
			fwrite(abstract, 1, strlen(corps), xml);

			free(corps);

			strcpy(balise, "</corps>");
			fwrite(balise, 1, sizeof(balise)+3, xml);

			fwrite("\n\t", 1, sizeof(char)+1, xml);
			
			
			while ( (zone == 1) || (zone == 2) ) {
				// conclusion
				if (zone == 1) {
					
					strcpy(balise, "<conclusion>");
					fwrite(balise, 1, sizeof(balise)+2, xml);

					char* conclu = getConclusion(fichier, &zone);
					fwrite(abstract, 1, strlen(conclu), xml);

					free(conclu);

					strcpy(balise, "</conclusion>");
					fwrite(balise, 1, sizeof(balise)+3, xml);

					fwrite("\n\t", 1, sizeof(char)+1, xml);
				}

				// discussion
				if (zone == 2) {
					
					strcpy(balise, "<discussion>");
					fwrite(balise, 1, sizeof(balise)+4, xml);

					char* discu = getDiscussion(fichier, &zone);
					fwrite(abstract, 1, strlen(discu), xml);

					strcpy(balise, "</discussion>");
					fwrite(balise, 1, sizeof(balise)+5, xml);

					fwrite("\n\t", 1, sizeof(char)+1, xml);
				}
			
			}

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

