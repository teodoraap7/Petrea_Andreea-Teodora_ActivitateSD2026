#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Reteta{
	unsigned int nrReteta;
	int nrMedicamente;
	char* listaMedicamente;
	char* numeMedic;
	float valoareBruta;
	int procentCompensare;
};
typedef struct Reteta Reteta;

struct Nod {
	Reteta info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	Nod** table;
	int dim;
};
typedef struct HashTable HashTable;

void afisareReteta(Reteta r) {
	printf("Id: %d \n", r.nrReteta);
	printf("Nr medicamente: %d \n", r.nrMedicamente);
	printf("Lista: %s \n", r.listaMedicamente);
	printf("Nume medic: %s \n", r.numeMedic);
	printf("Valoare: %f \n", r.valoareBruta);
	printf("Procent compensare: %d \n\n", r.procentCompensare);
}
void afisareListaReteta(Nod* cap) {
	while (cap) {
		afisareReteta(cap->info);
		cap = cap->next;
	}
}
Reteta citireRetetaDinFisier(FILE* f) {
	Reteta r;
	char buffer[110];
	char sep[4] = " \n;";
	char sepLista[1] = ",";
	fgets(buffer, 110, f);
	r.nrReteta = atoi(strtok(buffer, sep));
	r.nrMedicamente = atoi(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	r.listaMedicamente = (char*)malloc(sizeof(char)*strlen(aux)+1);
	strcpy(r.listaMedicamente, aux);
	aux = (strtok(NULL, sep));
	r.numeMedic = (char*)malloc(strlen(aux) + 1);
	strcpy(r.numeMedic, aux);
	r.valoareBruta = atof(strtok(NULL, sep));
	r.procentCompensare = atoi(strtok(NULL, sep));
	return r;
}
int hashFunction(unsigned int nrReteta, int dim) {
	return nrReteta % dim;
}
HashTable initializareHash(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.table = (Nod**)malloc(sizeof(Nod*)*dim);
	for (int i = 0; i < ht.dim; i++) {
		ht.table[i] = NULL;
	}
	return ht;
}
void adaugaRetetaInLista(Nod* cap, Reteta r) {
	Nod* p = cap;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = NULL;
	p->next = nou;
}
void inserareRetetaInTabela(HashTable ht, Reteta r) {
	int pozitie = hashFunction(r.nrReteta, ht.dim);
	if (ht.table[pozitie] == NULL) {
		ht.table[pozitie] = (Nod*)malloc(sizeof(Nod));
		ht.table[pozitie]->info = r;
		ht.table[pozitie]->next = NULL;
	}
	else {
		adaugaRetetaInLista(ht.table[pozitie], r);
	}
}

HashTable citireReteteDinFisier(const char* fisier, int dim) {
	FILE* f = fopen(fisier, "r");
	HashTable ht=initializareHash(dim);
	if (f) {
		while (!feof(f)) {
			inserareRetetaInTabela(ht, citireRetetaDinFisier(f));
		}
	}
	fclose(f);
	return ht;
}
void afisareTabelaRetete(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.table[i] != NULL) {
			printf("Retetele de pe poz %d \n", i);
			afisareListaReteta(ht.table[i]);
		}
		else {
			printf("Poz %d este libera \n", i);
		}
	}
}

int main() {
	HashTable ht = citireReteteDinFisier("retete.txt", 5);
	afisareTabelaRetete(ht);
	return 0;
}