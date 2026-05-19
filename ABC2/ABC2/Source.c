#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vacanta {
	int id;
	int nrZile;
	char* destinatie;
};
typedef struct Vacanta Vacanta; 
struct Nod {
	Vacanta vacanta;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Vacanta citesteVacantaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = "\n, ";
	char* aux;
	fgets(buffer, 100, f);
	Vacanta v;
	v.id = atoi(strtok(buffer, sep));
	v.nrZile = atoi(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	v.destinatie = (char*)malloc(sizeof(char)+1);
	strcpy(v.destinatie, aux);

	return v;
}
void afiseazaVacanta(Vacanta v) {
	printf("Id: %d \n", v.id);
	printf("Nr zile: %d \n", v.nrZile);
	printf("Destinatie: %s \n", v.destinatie);
}

Nod* creeazaNod(Vacanta v) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->dreapta = NULL;
	nod->stanga = NULL;
	nod->vacanta = v;
	return nod;
}

Nod* adaugaRecursiv(Nod* radacina, Vacanta v ) {
	if (radacina == NULL) {
		return creeazaNod(v);
	}
	if (v.id < radacina->vacanta.id) {
		radacina->stanga = adaugaRecursiv(radacina->stanga, v);
	}
	if (v.id > radacina->vacanta.id) {
		radacina->dreapta = adaugaRecursiv(radacina->dreapta, v);
	}
	return radacina;
}

void afisarePostOrdine(Nod* radacina) {
	if (radacina == NULL) return;
	afisarePostOrdine(radacina->stanga);
	afisarePostOrdine(radacina->dreapta);
	afiseazaVacanta(radacina->vacanta);
}

Nod* citireArboreDinFisier(const char* nume) {
	FILE* f = fopen(nume, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		arbore = adaugaRecursiv(arbore, citesteVacantaDinFisier(f));
	}
	fclose(f);
	return arbore;
}
void afisareArbore(Nod* arbore) {
	afisarePostOrdine(arbore);
}

void dezalocareArbore(Nod* arbore) {
	if (arbore == NULL) return;
	dezalocareArbore(arbore->stanga);
	dezalocareArbore(arbore->dreapta);
	free(arbore->vacanta.destinatie);
	free(arbore);

}

int main() {
	Nod* arbore = citireArboreDinFisier("vacante.txt");
	afisareArbore(arbore);
	dezalocareArbore(&arbore);
}