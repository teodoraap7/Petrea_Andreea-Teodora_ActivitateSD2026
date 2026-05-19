#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Vagon {
	int numarVagon;
	char* firmaTransport;
	int nrBileteVandute;
	int capacitateVagon;
};
typedef struct Vagon Vagon;

struct Nod {
	Vagon info;
	struct Nod* prev;
	struct Nod* next;
};

typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

void afisareVagon(Vagon v) {
	printf("Numar vagon: %d \n", v.numarVagon);
	printf("Firma transport: %s \n", v.firmaTransport);
	printf("Bilete vandute: %d \n", v.nrBileteVandute);
	printf("Capacitate vagon: %d \n\n", v.capacitateVagon);
}

void afisareListaMasiniInceput(ListaDubla ld) {
	Nod* p = ld.prim;
	while (p) {
		afisareVagon(p->info);
		p = p->next;
	}
}
void afisareListaMasiniSfarsit(ListaDubla ld) {
	Nod* p = ld.ultim;
	while (p) {
		afisareVagon(p->info);
		p = p->prev;
	}
}

void adaugaVagonInLista(ListaDubla* ld, Vagon vagon) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = vagon;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim != NULL) {
		ld->ultim->next = nou;
	}
	else {
		ld->prim = nou;
	}
	ld->ultim = nou;
	ld->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* ld, Vagon vagon) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = vagon;
	nou->prev = NULL;
	nou->next = ld->prim;
	if (ld->prim != NULL) {
		ld->prim->prev = nou;
	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;
	ld->nrNoduri++;
}
Vagon citireVagonDinFisier(FILE* fisier) {
	Vagon v;
	char buffer[100];
	char sep[4] = ", \n";
	fgets(buffer, 100, fisier);
	v.numarVagon = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	v.firmaTransport = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(v.firmaTransport, aux);
	v.nrBileteVandute = atoi(strtok(NULL, sep));
	v.capacitateVagon = atoi(strtok(NULL,sep));
	return v;
}
ListaDubla citireListaDinFisier(const char* fisier) {
	FILE* file = fopen("vagoane.txt", "r");
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	lista.nrNoduri = 0;
	while (!feof(file)) {
		adaugaVagonInLista(&lista, citireVagonDinFisier(file));
	}
	fclose(file);
	return lista;
}
void stergeVagCuMinimBilete(ListaDubla* ld) {

}
int main() {
	ListaDubla ld = citireListaDinFisier("vagoane.txt");
	afisareListaMasiniInceput(ld);
	afisareListaMasiniSfarsit(ld);
	return 0;
}