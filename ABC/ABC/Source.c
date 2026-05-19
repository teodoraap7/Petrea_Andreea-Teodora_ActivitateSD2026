#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;


//creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct NodABC Nod;

struct NodABC {
	Masina info;
	struct NodABC* stanga;
	struct NodABC* dreapta;
};


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


Nod* creeazaNod(Masina m) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = m;
	nod->dreapta = NULL;
	nod->stanga = NULL;
	return nod;
}

Nod* adaugaRecursiv(Nod* radacina, Masina m) {
	if (radacina == NULL) {
		return creeazaNod(m);
	} 
	if (m.pret < radacina->info.pret) {
		radacina->stanga = adaugaRecursiv(radacina->stanga, m);
	} else 
	if (m.pret > radacina->info.pret) {
		radacina->dreapta = adaugaRecursiv(radacina->dreapta, m);
	}
	return radacina;
}

Nod* citesteArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		arbore=adaugaRecursiv(arbore, citireMasinaDinFisier(f));
	}
	fclose(f);
	return arbore;
}
void afisareInOrdine(Nod* radacina) {
	if (radacina == NULL) return;

	afisareInOrdine(radacina->stanga);
	afisareMasina(radacina->info);
	afisareInOrdine(radacina->dreapta);
}
void afisarePreOrdine(Nod* radacina) {
	if (radacina == NULL) return;
	afisareMasina(radacina->info);
	afisarePreOrdine(radacina->stanga);
	afisarePreOrdine(radacina->dreapta);
}

void afisarePostOrdine(Nod* radacina) {
	if(radacina == NULL) return;
	afisarePostOrdine(radacina->stanga);
	afisarePostOrdine(radacina->dreapta);
	afisareMasina(radacina->info);
}

void afisareMasiniDinArbore(Nod* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	printf("========Preordine==============");
	afisarePreOrdine(radacina);
	printf("==========Inordine=========");
	afisareInOrdine(radacina);
	printf("========PostOrdine===========");
	afisarePostOrdine(radacina);
}

void dezalocareArboreDeMasini(Nod* radacina) {
	if (radacina == NULL) return;
	dezalocareArboreDeMasini(radacina->stanga);
	dezalocareArboreDeMasini(radacina->dreapta);
	free(radacina->info.model);
	free(radacina->info.numeSofer);
	free(radacina);

}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m = { 0 };
	if (radacina == NULL) return m;
	if (radacina->info.id == id) {
		return radacina->info;
	}
		Masina dinStanga = getMasinaByID(radacina->stanga, id);
		if (dinStanga.id == id) {
			return dinStanga;
		}
	return getMasinaByID(radacina->dreapta, id);
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL) return;
	int nrNoduri = 1;
	nrNoduri += determinaNumarNoduri(radacina->stanga);
	nrNoduri += determinaNumarNoduri(radacina->dreapta);
	//calculeaza numarul total de noduri din arborele binar de cautare
	return nrNoduri;
}

int calculeazaInaltimeArbore(/*arbore de masini*/) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/) {
	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	Nod* arbore = citesteArboreDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);
	Masina masina = getMasinaByID(arbore, 3);
	afisareMasina(masina);
	int nrNoduri = determinaNumarNoduri(arbore);
	printf(" Nr de noduri %d", nrNoduri);
	return 0;
}