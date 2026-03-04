#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



struct StructuraProdus {
	int id;
	int nrStocuri;
	float* stocuri;
	char* denumire;
};
typedef struct StructuraProdus Produs;

void afisareProdus(Produs produs) {
	printf("ID: %d ", produs.id);
	printf("Nr stocuri: %d ", produs.nrStocuri);
	printf("Stocuri: ");
	for (int i = 0; i < produs.nrStocuri; i++) {
		printf("%f, ", produs.stocuri[i]);
	}
	printf("Denumire: %s ", produs.denumire);
	printf("\n");
}

void afisareVectorProduse(Produs* produse, int nrProduse) {
	for (int i = 0; i < nrProduse; i++) {
		afisareProdus(produse[i]);
	}
}
void adaugareProdusInVector(Produs** produse, int* nrProduse, Produs produsNou) {
	Produs* aux = (Produs*)malloc(sizeof(Produs) * ((*nrProduse) + 1));
	for (int i = 0;  i < *nrProduse; i++) {
		aux[i] = (*produse)[i];
	}
	aux[(*nrProduse)] = produsNou;
	free(*produse);
	*produse = aux;
	(*nrProduse)++;
}
Produs citireProdusDinFisier(FILE* fisier) {
	Produs p;
	char buffer[100];
	char sep[4] = ", \n";
	fgets(buffer, 100, fisier); //am citit prima linie
	//acum vrem sa luam fiecare element de pe linie
	p.id = atoi(strtok(buffer, sep)); //strtok imi returneaza un char* dar m.id e un int
	p.nrStocuri = atoi(strtok(NULL, sep));
	p.stocuri =(float*) malloc(sizeof(float) * p.nrStocuri);
	for (int i = 0; i < p.nrStocuri; i++) {
		p.stocuri[i] = atof(strtok(NULL, sep));
	}
	char* aux = strtok(NULL, sep);
	p.denumire = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(p.denumire, aux);
	return p;
}

Produs* citireVectorProduse(const char* numeFisier, int* nrProduseCitite) {
	FILE* file = fopen("produse.txt", "r");
	Produs* produse = NULL;
	(*nrProduseCitite) = 0;
	while (!feof(file)) {
		adaugareProdusInVector(&produse, nrProduseCitite, citireProdusDinFisier(file));
	}
	fclose(file);
	return produse;
}


int main() {
	int nrProduse= 0;
	Produs* produse = citireVectorProduse("produse.txt", &nrProduse);
	afisareVectorProduse(produse, nrProduse);
	return 0;
}