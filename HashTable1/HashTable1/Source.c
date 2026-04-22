#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tichet {
	int cod;
	char* descriere;
	char* nume;
	char* dataDeschidere;
	char* stare;
};
typedef struct Tichet Tichet;

struct Nod {
	Tichet tichet;
	struct Nod* next;
};
typedef struct Nod Nod;

typedef struct {
	Nod** tabela;
	int dim;
} HashTable;  //vector de pointeri




void afisareTichet(Tichet t) {
	printf("Id: %d \n", t.cod);
	printf("Descriere: %s \n", t.descriere);
	printf("Nume: %s \n", t.nume);
	printf("Data deschidere: %s\n", t.dataDeschidere);
	printf("Stare: %s \n\n", t.stare);
}
Tichet citireTichetDinFisier(FILE* f) {
	Tichet tichet;
	char buffer[100];
	char sep[5] = "\n;";
	fgets(buffer, 100, f);
	tichet.cod = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	tichet.descriere = (char*)malloc(strlen(aux) + 1);
	strcpy(tichet.descriere, aux);
	aux = strtok(NULL, sep);
	tichet.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(tichet.nume, aux);
	aux = strtok(NULL, sep);
	tichet.dataDeschidere = (char*)malloc(strlen(aux) + 1);
	strcpy(tichet.dataDeschidere, aux);
	aux = strtok(NULL, sep);
	tichet.stare = (char*)malloc(strlen(aux));
	strcpy(tichet.stare, aux);

	return tichet;
}
void afisareListaTichete(Nod* cap) {
	while (cap) {
		afisareTichet(cap->tichet);
		cap = cap->next;
	}
}
//cand am coliziune, bag in lista, o apelez doar cand am coliziune
void adaugaTichetInLista(Nod* cap, Tichet tichet) {
	Nod* p = cap;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->tichet = tichet;
	nou->next = NULL;
	p->next = nou;
}
HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}
int calculeazaHash(const char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}

void inserareTichetInTabela(HashTable ht, Tichet tichet) {
	int pozitie = calculeazaHash(tichet.nume, ht.dim);
	if (ht.tabela[pozitie] == NULL) {
		ht.tabela[pozitie] = (Nod*)malloc(sizeof(Nod));
		ht.tabela[pozitie]->tichet = tichet;
		ht.tabela[pozitie]->next = NULL;
	}
	else {
		adaugaTichetInLista(ht.tabela[pozitie], tichet);
	}
}
HashTable citireTicheteDinFisier(const char* fisier, int dimensiune) {
	FILE* f = fopen(fisier, "r");
	HashTable ht = initializareHashTable(dimensiune);
	if (f) {
		while (!feof(f)) {
			inserareTichetInTabela(ht, citireTichetDinFisier(f));
		}
	}
	fclose(f);
	return ht;
}
void afisareTabelaMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			printf("Masinile de pe pozitia %d: \n", i);
			afisareListaTichete(ht.tabela[i]);
		}
		else {
			printf("Pozitia %d este goala\n", i);
		}
	}
}
void dezalocareListaMasini(Nod** cap) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	while (p) {
		Nod* temp = p;
		p = p->next;
		if (temp->tichet.dataDeschidere != NULL);
		free(temp->tichet.dataDeschidere);
		if (temp->tichet.descriere != NULL);
		free(temp->tichet.descriere);
		if (temp->tichet.nume != NULL);
		free(temp->tichet.nume);
		if (temp->tichet.stare != NULL);
		free(temp->tichet.stare);
		free(temp);

	}
	*cap = NULL;
}
void dezalocareHashTable(HashTable* h) {
	for (int i = 0; i < h->dim; i++) {
		if (h->tabela[i] != NULL) {
			dezalocareListaMasini(&h->tabela[i]);
		}
	}
	free(h->tabela);
	h->tabela = NULL;
	h->dim = 0;
}
int main() {
	HashTable ht = citireTicheteDinFisier("tichete.txt", 7);
	afisareTabelaMasini(ht);
}