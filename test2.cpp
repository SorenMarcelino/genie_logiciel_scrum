// g++ -c test2.cpp
// g++ -o test2 test2.o

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main (int argc, char** argv)
{
	time_t debut;
	time_t fin;
	string ligne;
	string fichier;
	
	time(&debut);	
	printf("boucle: ");
	for (int i=0; i<10000; i++)
	{
		ifstream fichier("Boudin-Torres-2006.txt", ios::in);
		if (fichier.is_open()) {
			while(getline(fichier, ligne)) { cout<<ligne<<endl; }
			fichier.close();
		}
		
		printf("%d ", i);
	}
    time(&fin);
	
	printf("\nTemps: %ld secondes en c++ \n", fin - debut);
	// 23 secondes
}
