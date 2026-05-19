#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Angajat {
	int id;
	char* nume;
	float salariu;
	char* departament;
};
typedef struct Angajat Angajat;

struct Nod {
	Angajat info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	Nod** tabela;
	int dim;
};
typedef struct HashTable HashTable;
void afisareAngajat(Angajat a) {
	printf("Id: %d Nume: %s Salariu: %.2f Departament: %s\n\n",
		a.id, a.nume, a.salariu, a.departament);
}
void initHashTable(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.tabela = (Nod**)malloc(sizeof(Nod) * dim);
	for (int i = 0; i < ht.dim; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}
int calcHash(int id, int dim) {
	return id % dim;
}
Angajat citireAngajatDinFisier(FILE* f) {
	char buffer[100];
	char sep[] = ",\n";
	Angajat a;
	a.id = -1;
	a.nume = NULL;
	a.salariu = 0;
	a.departament = NULL;

	if (fgets(buffer, 100, f) == NULL) {
		return a;  // EOF
	}
	a.id = atoi(strtok(buffer, sep));

	char* aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);

	a.salariu = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	a.departament = (char*)malloc(strlen(aux) + 1);
	strcpy(a.departament, aux);

	return a;
}
void adaugaAngajatInLista(Nod** cap, Angajat a) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = a;
	nou->next = *cap;
	*cap = nou;
}
void insereazaInHashTable(HashTable ht, Angajat a) {
	int poz = calcHash(a.id, ht.dim);
	inse
}
int main() {

}
