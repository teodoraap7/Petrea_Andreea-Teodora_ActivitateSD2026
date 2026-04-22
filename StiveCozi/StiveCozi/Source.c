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

struct Nod {
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;

struct Stiva {
	Nod* varf;
	int nrElem;
};

typedef struct Stiva Stiva;

Stiva initializareStiva() {
	Stiva s;
	s.nrElem = 0;
	s.varf = NULL;
	return s;
}

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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
void pushStack(Stiva* stiva, Masina masina) {
	//inserarea se face la inceput
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = stiva->varf;
	stiva->varf = nou;
	stiva->nrElem++;
	stiva->varf=nou;

}

Masina popStack(Stiva* stiva) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.nrUsi = 0;
	m.numeSofer = NULL;
	m.pret = 0;
	m.serie = '-';
	if (stiva->varf == NULL) {
		return m;
	}
	Nod* temp = stiva->varf;
	m = temp->info;
	stiva->varf = temp->next;
	free(temp);
	stiva->nrElem--;
	return m;
}

int emptyStack(Stiva *stiva) {
	return stiva->varf == NULL;
}

Stiva citireStackMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Stiva s = initializareStiva();
	while (!feof(f)) {
		pushStack(&s, citireMasinaDinFisier(f));
	}
	fclose(f);
	return s;
}

void dezalocareStivaDeMasini(Stiva *stiva) {
	//sunt dezalocate toate masinile si stiva de elemente
	
	while (stiva->varf) {
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		stiva->varf = aux;
		free(aux->info.model);
		free(aux->info.nrUsi);
		free(aux);
	}
	stiva->nrElem = 0;
}

int size(Stiva s) {
	//returneaza numarul de elemente din stiva
	return s.nrElem;
}

void afisareStiva(Stiva s) {
	Nod* p = s.varf;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

struct Coada {
	Nod* prim;
	Nod* ultim;
	int nrElem;
};
typedef struct Coada Coada;

Coada initializareCoada() {
	Coada c;
	c.nrElem = 0;
	c.prim = NULL;
	c.ultim = NULL;
	return c;
}
//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
void enqueue(Coada *coada, Masina masina) {
	//adauga o masina in coada
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = NULL;

	if (coada->ultim == NULL) {
		coada->prim = nou;
		coada->ultim = nou;
	}
	else {
		coada->ultim->next = nou;
		coada->ultim = nou;
	}
	coada->nrElem++;
}

Masina dequeue(Coada*coada) {
	//extrage o masina din coada
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.nrUsi = 0;
	m.pret = 0;
	m.serie = '-';
	if (coada->nrElem == 0) {
		return m;
	}
	Nod* temp = coada->prim;
	m = temp->info;
	coada->prim= temp->next;

	if (coada->prim == NULL) {
		coada->ultim = NULL;
	}
	free(temp);
	coada->nrElem--;

	return m;
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Coada coada = initializareCoada();
	if (f) {
		while (!feof(f)) {
			enqueue(&coada, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return coada;
}
void afisareCoada(Coada c) {
	Nod* p = c.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void dezalocareCoadaDeMasini(Coada* coada) {
	//sunt dezalocate toate masinile si coada de elemente
	while (coada->prim) {
		Nod* temp = coada->prim;
		coada->prim = temp->next;
		free(temp->info.numeSofer);
		free(temp->info.model);
		free(temp);
	}
	coada->ultim = NULL;
	coada->nrElem = 0;
}


//metode de procesare
Masina getMasinaByID(Coada coada, int id) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.nrUsi = 0;
	m.pret = 0;
	m.serie = '-';
	Nod* p =coada.prim;
	while (p) {
		{
			if (p->info.id == id) {
				return p->info;
			}
			p = p->next;
		}
		
	}
	return m;
}

float calculeazaPretTotal(/*stiva sau coada de masini*/);

int main() {
	Coada c = initializareCoada();
	c = citireCoadaDeMasiniDinFisier("masini.txt");

	printf("Masinile din coada sunt:\n");
	afisareCoada(c);


	Masina m = getMasinaByID(c, 2);
	if (m.id != -1) {
		printf("\nMasina gasita dupa ID:\n");
		afisareMasina(m);
	}

	Masina extrasa = dequeue(&c);
	if (extrasa.id != -1) {
		printf("\nMasina extrasa din coada:\n");
		afisareMasina(extrasa);

		free(extrasa.model);
		free(extrasa.numeSofer);
	}

	dezalocareCoadaDeMasini(&c);

	return 0;
}