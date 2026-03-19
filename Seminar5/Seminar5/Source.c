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

//creare structura pentru Lista Dubla 
struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
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

void afisareListaMasini(ListaDubla lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	printf("Lista are %d noduri\n", lista.nrNoduri);
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}
void afisareInversaListaMasini(ListaDubla lista) {
	printf("Lista are %d noduri\n", lista.nrNoduri);
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));  //adresa unui nod, malloc
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->ultim;
	if (lista->ultim) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}
	lista->ultim = nou;
	lista->nrNoduri++;

}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->next = lista->prim;
	p->prev = NULL;
	if (lista->prim) {
		lista->prim->prev = p;
	}
	else {
		lista->ultim = p;
	} 
	lista->prim = p;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	ListaDubla lista;
	lista.ultim = NULL;
	lista.prim = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		return lista;
	}
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;

}

void dezalocareLDMasini(ListaDubla* lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* nod = lista->prim;
	while (nod) {
		Nod* aux = nod;
		nod = nod->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
		lista->nrNoduri--;
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	float pretTotal = 0;
	if (lista.prim == NULL) {
		return 0;
	}
	else {
		/*Nod* p = lista.prim;
		while (p) {
			pretTotal += p->info.pret;
			p = p->next;
		}*/
		for (Nod* p = lista.prim; p != NULL; p=p->next) {
			pretTotal += p->info.pret;
		}
	}
	return pretTotal / lista.nrNoduri;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	if (lista->prim) {
		Nod* p = lista->prim;
		for (/*il am deasupra*/; p != NULL && p->info.id != id; p = p->next);
		if (!p) return;
		else {
			if (p->prev == NULL) {
				if (p->next) {
					p->next->prev = NULL;
				}
				else {

					lista->ultim = p->next;  //in caz de e singurul nod din lista 
				}
				lista->prim = p->next;
				free(p->info.model);
				free(p->info.numeSofer);
				free(p);
			}
			else {
				p->prev->next = p->next;
				if (p->next) {
					p->next->prev = p->prev;
				}
				else {
					lista->ultim = p->prev;
				}
				free(p->info.model);
				free(p->info.numeSofer);
				free(p);
			}
			lista->nrNoduri--;
		}
	}
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	/*printf("\n\nDupa dezalocare:");
	dezalocareLDMasini(&lista);
	afisareListaMasini(lista);*/
	printf("pretul mediu al masinilor: %f\n", calculeazaPretMediu(lista));
	stergeMasinaDupaID(&lista, 1);
	afisareListaMasini(lista);
	return 0;
}

