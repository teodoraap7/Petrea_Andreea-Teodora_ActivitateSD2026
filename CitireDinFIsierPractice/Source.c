#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("ID: %d\n",masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}


void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini)+1));
	for (int i=0; i<*nrMasini; i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	free(*masini);
	*masini = aux;	
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina masina;
	//citim cu file gets, impartim pe bucati, avem nev de un buffer
	char buffer[65];  //se aloca pe stiva de memorie a acestei functii, cand se term functia se elibereaza automat
	char sep[5] = ",;.\n";
	fgets(buffer, 65, file);
	//token ce gaseste pana la primul separator
	masina.id = atoi(strtok(buffer, sep)); //m.id e int, strtok returneaza char* - trebuie convertit la int
	//ascii to integer, ascii to char atoi atof
	//acum strtok isi marcheaza pana unde a ajuns, de acum vom avea NULL pt a se uita de unde a ramas, adica de dupa primul separator
	masina.nrUsi = atoi(strtok(NULL, sep));
	masina.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	masina.model = (char*)malloc(strlen(aux) + 1);	
	strcpy(masina.model, aux);

	aux = strtok(NULL, sep);
	masina.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(masina.numeSofer, aux);

	masina.serie = strtok(NULL, sep)[0];
	//strtok returneaza char*(un vector care are la final \n) - noi vrem doar primul char, de aceea punem [0]
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	//numeleFisier -> deschidem un stream pe baza lui, din stream il dam ca parametru la functia citireMasinaFisier, care va citi o masina, o va returna, o vom adauga in vectorul de masini, iar la final returnam vectorul de masini citite din fisier
	FILE* file = fopen(numeFisier, "r");
	Masina* masini = NULL;
	(*nrMasiniCitite) = 0;
	//adaug masini in vector cu functia de adaugare pana ajunge la end of file
	while (!feof(file)) {
		//trb sa dau adresa de masini, la nr masini e deja un pointer)
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	
	return 0;
}