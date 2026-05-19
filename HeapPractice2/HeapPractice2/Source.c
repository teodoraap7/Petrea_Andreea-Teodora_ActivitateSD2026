//vector folosit pt cozi de prioritate
//min heap - acces rapid la elem minim
//max heap - acces rapid la elem max 
//un vector practic
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;

struct Heap {
	int lungime;  //cate elem am in vector
	Masina* vector; 
	int nrMasiniVizibile;
};
typedef struct Heap Heap;

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
	printf("%f \n", m.pret);
	printf("%s \n", m.model);
	printf("%s \n", m.numeSofer);
	printf("%c \n\n", m.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasiniVizibile = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtrareHeap(Heap heap, int pozitie) {
	//radacina -> cei doi fii 
	//fiuStanga = 2*poz+1
	//fiuDreapta = 2*poz+2
	int pozFiuStanga = 2 * pozitie + 1;
	int pozFiuDreapta = 2 * pozitie + 2;
	//respecta pozitia? val nod sa fie mai mare decat a fiilor
	int pozMax = pozitie;
	if (heap.vector[pozMax].id < heap.vector[pozFiuStanga].id) {
		pozMax = pozFiuStanga;
	} 
	if (heap.vector[pozMax].id < heap.vector[pozFiuDreapta].id) {
		if (pozFiuDreapta < heap.nrMasiniVizibile) {
			pozMax = pozFiuDreapta;
		}
	}
	if (pozMax != pozitie) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;  
		if (pozMax <= (heap.nrMasiniVizibile - 2) / 2) {
			filtrareHeap(heap, pozMax);
		}
	}

}
Heap citireHeapDinFisier(const char* numeFisier) {
	FILE* fisier = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(fisier)) {
		heap.vector[heap.nrMasiniVizibile++] = citireMasinaDinFisier(fisier);
	}
	fclose(fisier);
	for (int i = (heap.nrMasiniVizibile - 2) / 2; i >= 0; i--) {
		filtrareHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	//nr masini elementele vizibile
	for (int i = 0; i < heap.nrMasiniVizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}

int main() {
	Heap heap = citireHeapDinFisier("masini.txt");
	afisareHeap(heap);
}