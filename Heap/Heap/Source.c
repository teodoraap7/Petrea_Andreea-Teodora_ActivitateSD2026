#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//heap structura de date omogena, 
//min heap, max heap cre ne aduc val minima sau maxima 

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrElemViz;
	Masina* vector;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Masina*)malloc(lungime * sizeof(Masina));
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	//determinam pozitiile 
	//extrage masina cu pretul cel mai mare
	int stanga = pozitieNod * 2 + 1;
	int dreapta = pozitieNod * 2 + 2;
	//trb sa vf daca are sens sa facem interschimbarea
	int pozMax = pozitieNod;
	if (stanga<heap.nrElemViz && heap.vector[stanga].pret > heap.vector[pozMax].pret) {
		pozMax = stanga;
	}
	if (stanga<heap.nrElemViz && heap.vector[dreapta].pret > heap.vector[pozMax].pret) {
		pozMax = dreapta;
	}
	//vf daca max se afla in radacina. daca nu, facem interschimbarea
	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrElemViz - 2) / 2) {  //reapelam pt nodurile parinte, vf daca e mai mic decat ultimul parinte 
			filtreazaHeap(heap, pozMax);
		}
	}

}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul

	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	if (f) {
		while (!feof(f)) {
			heap.vector[heap.nrElemViz++] = citireMasinaDinFisier(f);
		}
	}
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--) {
		{
			filtreazaHeap(heap, i);
		}
	}
	fclose(f);
	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrElemViz; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrElemViz; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	Masina aux;
	if (heap->nrElemViz>0) {
		heap->nrElemViz--;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux;
		filtreazaHeap(*heap, 0); 
		
	}
	return aux;

}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].numeSofer);
		free(heap->vector[i].model);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemViz = 0;

}

int main() {

	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Extrageri!");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	printf("\n\n\nHeap ascuns");
	afisareHeap(heap);
	dezalocareHeap(&heap);

	return 0;
}