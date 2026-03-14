#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Pilot {
	int pozitie;
	int tururi;
	int puncte;
	char* pilot;
	char* echipa;
	char pneu;
};
struct Nod {
	struct Pilot info;
	struct Nod* next;
};
typedef struct Pilot Pilot;
typedef struct Nod Nod;

void afiseazaPilot(Pilot p) {
	printf("Pozitie: %d, ", p.pozitie);
	printf("Tururi: %d, ", p.tururi);
	printf("Puncte: %d, ", p.puncte);
	printf("Pilot: %s, ", p.pilot);
	printf("Echipa: %s, ", p.echipa);
	printf("Pneu: %c", p.pneu);
	printf("\n");
}

Pilot citirePilotDinFisier(FILE* file) {
	char buffer[50];
	char sep[3] = ",\n";
	fgets(buffer, 50, file);
	Pilot p;
	p.pozitie = atoi(strtok(buffer, sep));
	p.tururi = atoi(strtok(NULL, sep));
	p.puncte = atoi(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	p.pilot = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(p.pilot, aux);
	aux = strtok(NULL, sep);
	p.echipa = (char*)malloc(sizeof(char) * strlen(aux) + 1);
	strcpy(p.echipa, aux);
	p.pneu = strtok(NULL, sep)[0];

	return p;
}
void adaugaPilotInLista(Nod** cap, Pilot noulPilot) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = noulPilot;
	nou->next = NULL;
	if(*cap){
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		(*cap) = nou;
	}
}
void adaugaPilotLaInceputDeLista(Nod** cap, Pilot noulPilot) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = noulPilot;
	nou->next = (*cap);
	(*cap) = nou;
}

Nod* citesteListaDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* fisier = fopen(numeFisier, "r");
	if (fisier) {
		while (!feof(fisier)) {
			adaugaPilotInLista(&cap, citirePilotDinFisier(fisier));
		}
	}
	fclose(fisier);
	return cap;
}
int calculeazaPuncteTotale(Nod* cap) {
	int suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.puncte;
		contor++;
		cap = cap->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
	return 0;
}
int calculeazaPuncteEchipa(Nod* cap, char* echipaCautata) {
	int suma = 0;
	while (cap) {
		if (strcmp(cap->info.echipa, echipaCautata) == 0) {
			suma += cap->info.puncte;
		}
		cap = cap->next;
	}
	return suma;
}
void afisareListaPiloti(Nod* cap) {
	while (cap) {
		afiseazaPilot(cap->info);
		cap = cap->next;
	}
}
int nrPilotiCuUnAnumitPneu(Nod* cap, char pneu) {
	int nrPiloti = 0;
	while (cap) {
		if (cap->info.pneu==pneu) {
			nrPiloti++;
		}
		cap = cap->next;
	}
	return nrPiloti;
}
void stergerePilotInFunctieDeEchipa(Nod** cap, char* echipa) {
	while ((*cap) && strcmp((*cap)->info.echipa,echipa)==0) {
		Nod* aux = (*cap);
		(*cap) = (*cap)->next;
		if (aux->info.pilot) {
			free(aux->info.pilot);
		}
		if (aux->info.echipa) {
			free(aux->info.echipa);
		}
		free(aux);
	}
	if ((*cap)) {
		Nod* p = (*cap);
		while (p) {
			while (p->next && strcmp(p->next->info.echipa,echipa)!=0) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = p->next->next;
				if (aux->info.pilot) {
					free(aux->info.pilot);
				}
				if (aux->info.echipa) {
					free(aux->info.echipa);
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
	cap=citesteListaDinFisier("curse.txt");
	afisareListaPiloti(cap);
	printf("Echipa redbull a strans: %d puncte\n", calculeazaPuncteEchipa(cap, "RedBull"));
	printf("Nr de piloti cu pneu soft: %d\n\n\n", nrPilotiCuUnAnumitPneu(cap, 'S'));
	printf("Pilotii de la RedBull au fost eliminati: ");
	stergerePilotInFunctieDeEchipa(&cap, "RedBull");
	afisareListaPiloti(cap);
	return 0;
	
}