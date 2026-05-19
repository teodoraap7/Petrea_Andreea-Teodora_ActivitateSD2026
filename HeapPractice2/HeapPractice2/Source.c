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
	char* aux;
	char sep[3] = ",\n ";
	fgets(buffer, 100, file);
	Masina m;
	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = (char*)malloc((strlen(aux) + 1));
	strcpy(m.model, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc((strlen(aux) + 1));
	strcpy(m.numeSofer, aux);
	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina m) {
	printf("%d \n", m.id);
	printf("%d \n", m.nrUsi);
	printf("%2.f \n", m.pret);
	printf("%s \n", m.model);
	printf("%s \n", m.numeSofer);
	printf("%c \n\n", m.serie);
}

struct Heap {
	Masina* vector;
	int lungime;
	int nrMasiniVizibile;
};

typedef struct Heap Heap;

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasiniVizibile = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuStanga = 2 * pozitieNod + 1;
	int pozFiuDreapta = 2 * pozitieNod + 2;
	int pozitieMin = pozitieNod;
	if ( pozFiuStanga < heap.lungime && heap.vector[pozitieMin].pret < heap.vector[pozFiuStanga].pret ) {
		pozitieMin = pozFiuStanga;
	}
	if (pozFiuDreapta < heap.lungime && heap.vector[pozitieMin].pret < heap.vector[pozFiuDreapta].pret ) {
		pozitieMin = pozFiuDreapta;
	}
	if (pozitieMin != pozitieNod) {
		Masina aux = heap.vector[pozitieMin];
		heap.vector[pozitieMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozitieMin <= (heap.nrMasiniVizibile - 2) / 2) {
			filtreazaHeap(heap, pozitieMin);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* fisier = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(fisier)) {
		heap.vector[heap.nrMasiniVizibile++] = citireMasinaDinFisier(fisier);
	}
	fclose(fisier);

	for (int i = (heap.nrMasiniVizibile - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;

}
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasiniVizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}
int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
}