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

//creare structura pentru un nod dintr-o lista dublu inlantuita

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;
//creare structura pentru Lista Dubla 
struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasiniDeLaInceput(ListaDubla listaDubla) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p = listaDubla.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla listaDubla) {
	Nod* p = listaDubla.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if(lista->last!=NULL){
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->prev = NULL;
	nou->next = lista->first;
	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	} 
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen("masini.txt", "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = (Nod*)malloc(sizeof(Nod));
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model != NULL) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer != NULL) {
			free(aux->masina.numeSofer);
		}
		free(aux);
		lista->first = NULL;
		lista->last = NULL;
		lista->nrNoduri = 0;
	}
}

float calculeazaPretMediu(ListaDubla lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	Nod* p = lista.first;
	while (p) {
		suma += p->masina.pret;
		p = p->next;
	}
	return suma / lista.nrNoduri;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	Nod* temp = lista->first;

	while (temp != NULL) {
		if (temp->masina.id == id) {

			//primul nod
			if (temp->prev == NULL) {
				lista->first = temp->next;
				if (lista->first != NULL) {			//vf daca lista devine goala 
					lista->last = NULL;
					lista->first->prev = NULL;    //noul prim nod nu mai are nimic inainte
				}
			}
			else if (temp->next == NULL) { //daca e ultimul nod
				lista->last = temp->prev;
				lista->last->next = NULL;
			}
			else {
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			free(temp->masina.model);
			free(temp->masina.numeSofer);
			lista->nrNoduri--;
			free(temp);
			return;
		}
		temp = temp->next;
	}
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei masini.
	return NULL;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("Pret mediu masini %0.2f", calculeazaPretMediu(lista));
	
	stergeMasinaDupaID(&lista, 10);
	afisareListaMasiniDeLaInceput(lista);
	dezalocareLDMasini(&lista);

	return 0;
}