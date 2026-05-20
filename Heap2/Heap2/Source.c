#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Animal {
	int id;
	int varsta;
	float viteza;
	char* nume;
	char* habitat;
};

typedef struct Animal Animal;
Animal citesteAnimalDinFisier(FILE* f) {
	Animal a;
	char buffer[100];
	char sep[4] = "\n, ";
	char* aux;
	fgets(buffer, 100, f);

	a.id = atoi(strtok(buffer, sep));
	a.varsta = atoi(strtok(NULL, sep));
	a.viteza = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);
	aux = strtok(NULL, sep);
	a.habitat = (char*)malloc(strlen(aux) + 1);
	strcpy(a.habitat, aux);

	return a;
}

void afiseazaAnimal(Animal a) {
	printf("ID: %d ", a.id);
	printf("Varsta: %d ", a.varsta);
	printf("Viteza: %f ", a.viteza);
	printf("Nume: %s ", a.nume);
	printf("Habitat: %s\n\n", a.habitat);
}

struct Heap {
	Animal* vector;
	int lungime;
	int nrAnimale;
};
typedef struct Heap Heap;


Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrAnimale = 0;
	heap.vector = (Animal*)malloc(sizeof(Animal) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitie) {
	int pozitieFiuStanga = 2 * pozitie + 1;
	int pozitieFiuDreapta = 2 * pozitie + 2;
	int pozMax = pozitie;
	if (pozitieFiuStanga< heap.nrAnimale && heap.vector[pozMax].viteza < heap.vector[pozitieFiuStanga].viteza) {
		pozMax = pozitieFiuStanga;
	} 
	if (pozitieFiuDreapta<heap.nrAnimale && heap.vector[pozitieFiuDreapta].viteza > heap.vector[pozMax].viteza) {
		pozMax = pozitieFiuDreapta;
	}
	if (pozMax != pozitie) {
		Animal aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (pozMax <= (heap.nrAnimale - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}


void filtrareHeap(Heap heap, int pozitie) {
	int pozFiuStanga = 2 * pozitie + 1;
	int pozFiuDreapta = 2 * pozitie + 2;
	int pozMin = pozitie;
	if (pozFiuStanga < heap.nrAnimale &&
		heap.vector[pozFiuStanga].varsta < heap.vector[pozMin].varsta) {
		pozMin = pozFiuStanga;
	}
	if (pozFiuDreapta < heap.nrAnimale &&
		heap.vector[pozFiuDreapta].varsta < heap.vector[pozMin].varsta) {
		pozMin = pozFiuDreapta;
	}
	if (pozMin != pozitie) {
		Animal aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (pozMin <= (heap.nrAnimale - 2) / 2) {
			filtrareHeap(heap, pozMin);
		}
	}
}

Heap citesteHeapDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(15);
	while (!feof(f)) {
		heap.vector[heap.nrAnimale++] = citesteAnimalDinFisier(f);
	}
	for (int i = (heap.nrAnimale - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	fclose(f);
	return heap;
}

Heap citireHeapDinFisier(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Heap heap = initializareHeap(15);
	while (!feof(f)) {
		heap.vector[heap.nrAnimale++] = citesteAnimalDinFisier(f);
	}
	for (int i = (heap.nrAnimale - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	fclose(f);
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrAnimale; i++) {
		afiseazaAnimal(heap.vector[i]);
	}
}
void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].habitat);
	}
	free(heap->vector);
	heap->lungime = 0;
	heap->vector = NULL;
	heap->nrAnimale = 0;

}


struct NodABC {
	struct NodABC* stanga;
	struct NodABC* dreapta;
	Animal animal;
};

typedef struct NodABC Nod;

Nod* creeazaNod(Animal a) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->animal = a;
	nod->dreapta = NULL;
	nod->stanga = NULL;
	return nod;
}

Nod* adaugaRecursiv(Nod* radacina, Animal a) {
	if (radacina == NULL) return creeazaNod(a);
	if (radacina->animal.viteza > a.viteza) {
		radacina->stanga = adaugaRecursiv(radacina->stanga, a);
	}
	if (radacina->animal.viteza < a.viteza) {
		radacina->dreapta = adaugaRecursiv(radacina->dreapta, a );
	}
	return radacina;
}

void adaugaAnimalInArbore(Nod** r, Animal a) {
	*r = adaugaRecursiv(*r, a);
}
Nod* citesteArboreDinFisier(const char* numeFisier) {
	Nod* arbore = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		adaugaAnimalInArbore(&arbore, citesteAnimalDinFisier(f));
	}
	fclose(f);
	return arbore;
}
void afisareInOrdine(Nod* radacina) {
	if (radacina == NULL) return;

	afisareInOrdine(radacina->stanga);
	afiseazaAnimal(radacina->animal);
	afisareInOrdine(radacina->dreapta);
}


struct NodAVL {
	struct NodAVL* stanga;
	struct NodAVL* dreapta;
	int inaltime;
	Animal a;
};
typedef struct NodAVL NodAVL;

int maxDinDoi(int a, int b) {
	if (a > b) return a;
	return b;
}
int inaltime(NodAVL* radacina) {
	if (radacina == NULL) return 0;
	return radacina->inaltime;
}
void actualizeazaInaltime(NodAVL* radacina) {
	if (radacina != NULL)
		radacina->inaltime = 1 + maxDinDoi(
			inaltime(radacina->stanga), inaltime(radacina->dreapta)
		);
}

NodAVL* creeazaNodAVL(Animal a) {
	NodAVL* nodAV = (NodAVL*)malloc(sizeof(NodAVL));
	nodAV->a = a;
	nodAV->dreapta = NULL;
	nodAV->stanga = NULL;
	nodAV->inaltime = 1;
	return nodAV;
}
//rotatii 

NodAVL* rotireDreapta(NodAVL* radacina) {
	NodAVL* B = radacina->stanga;
	NodAVL* T = B->dreapta;
	B->dreapta = radacina;
	radacina->stanga = T;
	actualizeazaInaltime(radacina);
	actualizeazaInaltime(B);
	return B;
}

NodAVL* rotireStanga(NodAVL* radacina) {
	NodAVL* B = radacina->dreapta;
	NodAVL* T = B->stanga;
	B->stanga = radacina;
	radacina->dreapta = T;
	actualizeazaInaltime(radacina);
	actualizeazaInaltime(B);
	return B;
}

NodAVL* echilibreazaArbore(NodAVL* radacina) {
	int bf = inaltime(radacina->stanga) - inaltime(radacina->dreapta);
	int bfS = 0; 
	int bfD = 0;
	if (radacina->stanga != NULL) {
		bfS = inaltime(radacina->stanga->stanga) - inaltime(radacina->stanga->dreapta);
	}
	if (radacina->dreapta != NULL) {
		bfD = inaltime(radacina->dreapta->stanga) - inaltime(radacina->dreapta->dreapta);
	}
	
	if (bf > 1 && bfS >= 0) {
		return rotireDreapta(radacina);
	} else 
	if (bf > 1 && bfS < 0) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	} else 
	if (bf < -1 && bfD <= 0) {
		return rotireStanga(radacina);
	} else 
	if (bf < -1 && bfD >0) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	}
	return radacina;
}

NodAVL* adaugaRecursivAVL(NodAVL* radacina, Animal a) {
	if (radacina == NULL)
		return creeazaNodAVL(a);
	if (a.viteza < radacina->a.viteza) {
		radacina->stanga = adaugaRecursivAVL(radacina->stanga, a);
	}
	if (a.viteza > radacina->a.viteza) {
		radacina->dreapta = adaugaRecursivAVL(radacina->dreapta, a);
	}
	 return echilibreazaArbore(radacina);
}

void adaugaAnimalInArboreAVL(NodAVL** radacina, Animal a) {
	*radacina = adaugaRecursivAVL(*radacina, a);
}

NodAVL* citireAVLDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodAVL* arbore = NULL;
	while (!feof(f)) {
		adaugaAnimalInArboreAVL(&arbore, citesteAnimalDinFisier(f));
	}
	fclose(f);
	return arbore;
}

void afisareInOrdineAVL(NodAVL* radacina) {
	if (radacina == NULL) return;
	afisareInOrdineAVL(radacina->stanga);
	afiseazaAnimal(radacina->a);
	afisareInOrdineAVL(radacina->dreapta);
}

int main() {
	Heap heap = citireHeapDinFisier("animale.txt");
	afisareHeap(heap);
	printf("===========================\n");
	printf("===========================\n");
	printf("===========================\n");
	Heap h2 = citesteHeapDinFisier("animale.txt");
	afisareHeap(h2);
	printf("===========================\n");
	printf("===========================\n");
	printf("===========================\n");
	Nod* arbore = citesteArboreDinFisier("animale.txt");
	
	afisareInOrdine(arbore);

	printf("===========================\n");
	printf("===========================\n");
	printf("===========================\n");

	NodAVL* avl = citireAVLDinFisier("animale.txt");

	
	afisareInOrdineAVL(avl);

}