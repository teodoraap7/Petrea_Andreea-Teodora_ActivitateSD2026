#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Produs {
	int cod;
	char* denumire;
	float pret;
	char* categorie;
};
typedef struct Produs Produs;

struct Nod {
	struct Nod* next;
	Produs info;
};
typedef struct Nod Nod;

Produs citireProdusDinFisier(FILE* fisier) {
	char buffer[100];
	char sep[4] = " \n,";
	Produs p;
	fgets(buffer, 100, fisier);
	p.cod = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	p.denumire = (char*)malloc(strlen(aux)+1);
	strcpy(p.denumire, aux);
	p.pret =atof( strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.categorie = (char*)malloc(strlen(aux) + 1);
	strcpy(p.categorie, aux);
	return p;
}
void afisareProdus(Produs p) {
	printf(" %d, %s, %f, %s \n", p.cod, p.denumire,
		p.pret, p.categorie);
}

void adaugareProdusInLista(Nod** cap, Produs p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}
void adaugaLaInceputInLista(Nod** cap, Produs p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = *cap;
	*cap = nou;
}
Nod* citireListaDinFisier(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Nod* lista = NULL;
	if (f) {
		while (!feof(f)) {
			adaugaLaInceputInLista(&lista, citireProdusDinFisier(f));
		}
	}
	fclose(f);
	return lista;
}
void afisareListaSimpla(Nod *lista) {
	while (lista) {
		afisareProdus(lista->info);
		lista = lista->next;
	}
}

int main() {
	Nod* lista = citireListaDinFisier("produse.txt");
	afisareListaSimpla(lista);

}