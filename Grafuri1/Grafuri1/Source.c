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
	NodSecundar* vecini;
	Masina info;
};
struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

void inserareLista(NodPrincipal** graf, Masina m) {
	NodPrincipal* nod = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nod->info = m;
	nod->next = NULL;
	nod->vecini = NULL;
	if (*graf) {
		NodPrincipal* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nod;
	}  else 
	*graf = nod;
}

void inserareListaSecundara(NodSecundar** listaSecundara, NodPrincipal* nodNou) {
	NodSecundar* nod = (NodSecundar*)malloc(sizeof(NodSecundar));
	nod->info = nodNou;
	nod->next = NULL;
	if (*listaSecundara) {
		NodSecundar* aux = *listaSecundara;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nod;
	} else 
	*listaSecundara = nod;
}
NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	while ((listaPrincipala) && listaPrincipala->info.id != id) {
		listaPrincipala = listaPrincipala->next;
	} return listaPrincipala;

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
	while (!feof(f)) {
		inserareLista(&(listaPrincipala), citireMasinaDinFisier(f));
	}
	fclose(f);
	return listaPrincipala;
}

void citireMuchiiDinFisier(NodPrincipal* listaPrincipala, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		int idStart = 0;
		int idStop = 0;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(listaPrincipala, idStart, idStop);
	}
	fclose(f);
}

int main() {
	NodPrincipal* lista = NULL;
	lista = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(lista, "muchii.txt");
	return 0;
}


