#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Film {
	int id;
	int anAparitie;
	float rating;
	char* titlu;
	char* regizor;
};
typedef struct Film Film;


Film citireFilmDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = "\n, ";
	Film f;
	fgets(buffer, 100, file);
	char* aux;
	f.id = atoi(strtok(buffer, sep));
	f.anAparitie = atoi(strtok(NULL, sep));
	f.rating = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	f.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(f.titlu, aux);

	aux = strtok(NULL, sep);
	f.regizor = (char*)malloc(strlen(aux) + 1);
	strcpy(f.regizor, aux); 
	return f;
}

void afisareFilm(Film film) {
	printf("Titlu:       %s\n", film.titlu);
	printf("Regizor:     %s\n", film.regizor);
	printf("An aparitie: %d\n", film.anAparitie);
	printf("Rating:      %.1f\n", film.rating);
	printf("Id:          %d\n\n", film.id);
}

struct NodAVL {
	Film film;
	struct NodAVL* stanga;
	struct NodAVL* dreapta;
	int inaltime;
};
typedef struct NodAVL Nod;

int inaltime(Nod* radacina) {
	if (radacina == NULL) return 0 ;
	return radacina->inaltime;
}
int maxDinDoi(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

void actualizeazaInaltime(Nod* nod) {
	if (nod != NULL) {
		nod->inaltime = 1 + maxDinDoi(inaltime(nod->stanga), inaltime(nod->dreapta));
	}
}
Nod* creeazaNod(Film film) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->inaltime = 1;
	nod->dreapta = NULL;
	nod->stanga = NULL;
	nod->film = film;
	return nod;
}


//rotatii AVL
Nod*  rotireDreapta(Nod* radacina) {
	Nod* B = radacina->stanga;
	Nod* T = B->dreapta;
	B->dreapta = radacina;
	radacina->stanga = T;
	actualizeazaInaltime(radacina);
	actualizeazaInaltime(B);
	return B;
}

Nod*  rotireStanga(Nod* radacina) {
	Nod* B = radacina->dreapta;
	Nod* T = B->stanga;
	B->stanga = radacina;
	radacina->dreapta = T;
	
	actualizeazaInaltime(radacina);
	actualizeazaInaltime(B);
	return B;
}

Nod* reechilibreaza(Nod* radacina) {
	actualizeazaInaltime(radacina);
	int bf = inaltime(radacina->stanga) - inaltime(radacina->dreapta);
	int inaltimeFiuStanga = 0;
	int inaltimeFiuDreapta = 0;
	if (radacina->stanga != NULL) {
		inaltimeFiuStanga = inaltime(radacina->stanga->stanga) -
			inaltime(radacina->stanga->dreapta);
	}
	if (radacina->dreapta != NULL) {
		inaltimeFiuDreapta = inaltime(radacina->dreapta->stanga) -
			inaltime(radacina->dreapta->dreapta);
	}
	if(bf>1 && inaltimeFiuStanga>=0){
		return rotireDreapta(radacina);
	}
	if (bf > 1 && inaltimeFiuStanga < 0) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	}
	if (bf < -1 && inaltimeFiuDreapta <= 0) {
		return rotireStanga(radacina);
	}

	if (bf < -1 && inaltimeFiuDreapta > 0) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	} 
	return radacina;

}
Nod* recursiv(Nod* radacina, Film film) {

	if (radacina == NULL)
		return creeazaNod(film);
	if (film.anAparitie < radacina->film.anAparitie) {
		radacina->stanga = recursiv(radacina->stanga, film);
	}
	if (film.anAparitie >= radacina->film.anAparitie) {
		radacina->dreapta = recursiv(radacina->dreapta, film);
	}
	return reechilibreaza(radacina);
}
Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		arbore = recursiv(arbore, citireFilmDinFisier(f));
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* radacina) {
	if (radacina == NULL) {
		return;
	}
	afisareInOrdine(radacina->stanga);
	afisareFilm(radacina->film);
	afisareInOrdine(radacina->dreapta);
}

void afisareMasiniDinArbore(Nod* radacina) {
	afisareInOrdine(radacina);
}

int main() {
	Nod* arbore = citireArboreDinFisier("filme.txt");
	afisareMasiniDinArbore(arbore);
}