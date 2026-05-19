#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;

	aux = strtok(buffer, sep);
	m1.id = atoi(aux);

	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal {
	NodPrincipal* next;
	Masina info;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodSecundar* next;
	NodPrincipal* info;
};

void inserareLista(NodPrincipal** graf, Masina masinaNoua) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*graf) {
		NodPrincipal* p = *graf;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareListaSecundara(NodSecundar** listaSecundara, NodPrincipal* nod) {
	NodSecundar* nodSecundar = (NodSecundar*)malloc(sizeof(NodSecundar));
	nodSecundar->info = nod;
	nodSecundar->next = NULL;

	if (*listaSecundara) {
		NodSecundar* p = *listaSecundara;
		while (p->next) {
			p = p->next;
		}
		p->next = nodSecundar;
	}
	else {
		*listaSecundara = nodSecundar;
	}
}

NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	while ((listaPrincipala) && listaPrincipala->info.id != id) {
		listaPrincipala = listaPrincipala->next;
	}
	return listaPrincipala;
}

void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		NodPrincipal* nodStart = cautaNodDupaID(listaPrincipala, idStart);
		NodPrincipal* nodStop = cautaNodDupaID(listaPrincipala, idStop);

		if (nodStart && nodStop) {
			inserareListaSecundara(&nodStart->vecini, nodStop);
			inserareListaSecundara(&nodStop->vecini, nodStart);
		}
	}
}

NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* listaPrincipala = NULL;

	if (f) {
		while (!feof(f)) {
			inserareLista(&(listaPrincipala), citireMasinaDinFisier(f));
		}
		fclose(f);
	}

	return listaPrincipala;
}

void citireMuchiiDinFisier(NodPrincipal* listaPrincipala, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			int idStart = 0;
			int idStop = 0;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(listaPrincipala, idStart, idStop);
		}
		fclose(f);
	}
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

int main() {

	NodPrincipal* graf = NULL;
	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");

	return 0;
}