#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cladire {
	int id;
	int nrEtaje;
	float valoare;
	char* numeCladire;
	char* numeAdministrator;
	char sector;
};
typedef struct Cladire Cladire;
typedef struct Nod Nod;

struct Nod {
	Cladire info;
	Nod* next;
};
Cladire citireCladireDinFisier(FILE* file) {
	Cladire c;
	char buffer[50];
	char sep[3] = ",\n";
	fgets(buffer, 50, file);
	c.id = atoi(strtok(buffer, sep));
	c.nrEtaje = atoi(strtok(NULL, sep));
	c.valoare= atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	c.numeCladire = (char*)malloc(strlen(aux) + 1);
	strcpy(c.numeCladire, aux);
	aux = strtok(NULL, sep);
	c.numeAdministrator = (char*)malloc(strlen(aux) + 1);
	strcpy(c.numeAdministrator, aux);
	c.sector = strtok(NULL, sep)[0];
	return c;
}
void afisareCladire(Cladire cladire) {
	printf("Id: %d, ", cladire.id);
	printf("Nr etaje: %d, ", cladire.nrEtaje);
	printf("Valoare: %.2f, ", cladire.valoare);
	printf("Nume cladire: %s, ", cladire.numeCladire);
	printf("Nume administrator: %s, ", cladire.numeAdministrator);
	printf("Sector: %c.", cladire.sector);
	printf("\n");

}
void afisareListaCladiri(Nod* cap) {
	while (cap != NULL) {
		afisareCladire(cap->info);
		cap = cap->next;
	}
}
void adaugaCladireInLista(Nod** cap, Cladire nouaCladire) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = nouaCladire;
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p=p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}
void adaugaLaInceputInLista(Nod** cap, Cladire nouaCladire) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = nouaCladire;
	nou->next = *cap;
	*cap = nou;
}
Nod* citireListaCladiriDinFisier(const char* file) {
	FILE* fisier= fopen(file, "r");
	Nod* cap = NULL;
	if (fisier) {
		while (!feof(fisier)) {
			adaugaCladireInLista(&cap, citireCladireDinFisier(fisier));
		}
	}

	fclose(fisier);
	return cap;
}


void dezalocareLista(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.numeAdministrator) {
			free(p->info.numeAdministrator);
		}
		if (p->info.numeCladire) {
			free(p->info.numeCladire);
		}
		free(p);  //stergem nodul
	}
	(*cap) = NULL;

}
float calculeazaValoareMedie(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.valoare;
		contor++;
		cap = cap->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
	return 0;
}
float calculeazaValoareaCladirilorUnuiAdministrator(Nod* cap, char* numeAdministrator) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeAdministrator, numeAdministrator) == 0) {
			suma += cap->info.valoare;
		}
		cap = cap->next;
	}
	return suma;
}
void stergeCladiriDinSector(Nod** cap, char sector) {
	//trb sa fac legatura intre nodul de dinainte si cel de dupa
	//trb sa ne oprim inaintea nodului pe care vrem sa il stergem
	//de tratat si cazurile in care cladirea este pe prima sau ultima pozitie
	while ((*cap) && (*cap)->info.sector==sector) {
		Nod* aux = (*cap);
		(*cap) = (*cap)->next;
		if (aux->info.numeAdministrator) {
			free(aux->info.numeAdministrator);
		}
		if (aux->info.numeCladire) {
			free(aux->info.numeCladire);
		}
		free(aux);
	} //pe prima pozitie nu avem sectorul
	//vf unde mai avem sectorul
	if (*cap) {
		Nod* p = *cap;
		while (p) {
			while (p->next && p->next->info.sector != sector) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = p->next->next;
				if (aux->info.numeAdministrator) {
					free(aux->info.numeAdministrator);
				}
				if (aux->info.numeCladire) {
					free(aux->info.numeCladire);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}

}
int main() {
	Nod* cap = NULL;
	cap=citireListaCladiriDinFisier("cladiri.txt");
	afisareListaCladiri(cap);
	printf("Valoare medie: %.f",calculeazaValoareMedie(cap));
	printf("Valoarea cladirilor lui Popescu este: %f", calculeazaValoareaCladirilorUnuiAdministrator(cap, "Popescu"));
	stergeCladiriDinSector(&cap, 'B');
	printf("Stergere din sectorul 1: \n");
	afisareListaCladiri(cap);
	return 0;
}
