
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include <string.h>
struct Biblioteca {
	int id;
	int nrCarti;
	char* nume;
	float suprafata;
	char sector;
};
struct Biblioteca initializare(int id, int nrCarti, char* nume, float suprafata, char sector) {
	struct Biblioteca s;

	s.id = id;
	s.nrCarti = nrCarti;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.suprafata = suprafata;
	s.sector = sector;


	return s;
}
struct Biblioteca  copiazaBiblioteca(struct Biblioteca sursa) {
	return initializare(sursa.id, sursa.nrCarti, sursa.nume, sursa.suprafata, sursa.sector);

}

void afisare(struct Biblioteca s) {
	//afisarea tuturor atributelor.
	printf("Id: %d\n", s.id);
	printf("Nr carti: %d\n", s.nrCarti);
	printf("Nume: %s\n", s.nume);
	printf("Suprafata: %f\n", s.suprafata);
	printf("Sector: %c\n", s.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Biblioteca* vectorNou = NULL;

	if (nrElemente <= nrElementeCopiate) {
		nrElementeCopiate = nrElemente;
	}

	vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = copiazaBiblioteca(vector[i]);
	}

	return vectorNou;
}

void dezalocare(struct Biblioteca** vector, int* nrElemente) {  //transmit prin adresa  
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < (*nrElemente); i++) {
		free((*vector)[i].nume);  //deref deplasare deref accesare (*vector) dereferentiere  (deref)+(deplasare+deref) + accesare == * [] + accesare 
	}
	free((*vector));
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Biblioteca* vector, int nrElemente, float prag, struct Biblioteca** vectorNou, int* dimensiune) {  //ex copiazaElementeleCuSuprafataMare
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata > prag) {
			(*dimensiune)++;
		}
	}
	*vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*dimensiune));

	int contor = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata > prag) {
			(*vectorNou)[contor] = copiazaBiblioteca(vector[i]);
			contor++;
		}
	}
}

struct Biblioteca getPrimulElementConditionat(struct Biblioteca* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Biblioteca s;
	s.id = 1;

	return s;
}



int main() {
	struct Biblioteca b1 = initializare(1, 123, "Biblioteca Centrala", 500.0, 'A');
	afisare(b1);

	int nrBiblioteci = 4;
	//pe heap cu malloc
	struct Biblioteca* biblioteci = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrBiblioteci);	//cu un pointer mai putin 
	biblioteci[0] = copiazaBiblioteca(b1);  //atribuire de adrese deci ambii vor avea aceeasi zona de memorie  ->>shallow copy
	biblioteci[1] = initializare(2, 100, "Biblioteca Ion Creanga", 300.0, 'B');
	biblioteci[2] = initializare(3, 100, "Biblioteca ASE", 700.0, 'C');
	biblioteci[3] = initializare(4, 100, "Biblioteca Sector2", 500.0, 'B');
	printf("Elementele din vector");
	afisareVector(biblioteci, nrBiblioteci);

	printf("\n\nELEMENTE COPIATEEEEEEEEEEEEEEEEEE\n\n\n");
	int nrCopiate = 2;
	struct Biblioteca* copie = copiazaPrimeleNElemente(biblioteci, nrBiblioteci, 2);
	afisareVector(copie, nrCopiate);
	dezalocare(&copie, &nrCopiate);

	struct Biblioteca* copie2 = NULL;
	int nrBibCUSuprafataMare = 0;
	copiazaAnumiteElemente(biblioteci, nrBiblioteci, 600, &copie2, &nrBibCUSuprafataMare);
	afisareVector(copie2, nrBibCUSuprafataMare);
	dezalocare(&copie2, &nrBibCUSuprafataMare);
	return 0;
}


//[] deplasare si dereferentiere
//->dereferentiere si accesare 