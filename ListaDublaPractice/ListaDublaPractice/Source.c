#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
	int id;
	char* client;
	float valoare;
	char* status;
};
typedef struct Comanda Comanda;

struct Nod {
	struct Nod* next;
	struct Nod* prev;
	Comanda info;
};

typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Comanda citireComandaDinFisier(FILE* f) {
	char buffer[100];
	char sep[4] = ",\n ";
	Comanda c;
	fgets(buffer, 100, f);
	c.id = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	c.client = (char*)malloc(strlen(aux) + 1);
	strcpy(c.client, aux);
	c.valoare = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c.status = (char*)malloc(strlen(aux) + 1);
	strcpy(c.status, aux);
	return c;
}
void afisareComanda(Comanda c) {
	printf("Id: %d Client: %s Valoare: %.2f Status: %s\n",
		c.id,
		c.client,
		c.valoare,
		c.status);
}

void adaugaNodInLista(ListaDubla* lista, Comanda c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	nou->prev = lista->ultim;
	if (lista->ultim != NULL) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}
	lista->ultim = nou;
	lista->nrNoduri++;
}
ListaDubla citireListaDinFisier(FILE* fisier) {
	FILE* f = fopen(fisier, "r");
	ListaDubla ld;
	if (f) {
		while (!feof(f)) {
			adaugaNodInLista(&ld, citireComandaDinFisier(f));
		}
	}
	fclose(f);
	return ld;

}
void afisareListaComenzi(ListaDubla ld) {
	Nod* p = ld.prim;
	while (p) {
		afisareComanda(p->info);
		p = p->next;
	}
}

int main() {
	ListaDubla ld = citireListaDinFisier("comenzi.txt");
	afisareListaComenzi(ld);
}