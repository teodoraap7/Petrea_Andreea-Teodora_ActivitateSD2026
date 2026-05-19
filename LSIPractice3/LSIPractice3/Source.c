#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Destinatie {
	int id;
	int nrRecenzii;
	int* note;
	int pretMediu;
	char* numeDestinatie;
	char* tipVacanta;
};
struct Nod {
	struct Destinatie info;
	struct Nod* next;
};
typedef struct Destinatie Destinatie;
typedef struct Nod Nod;

void afisareDestinatie(Destinatie d) {
	printf("Id: %d\n", d.id);
	printf("Nr recenzii: %d\n", d.nrRecenzii);
	printf("Note:");
	for (int i = 0; i < d.nrRecenzii; i++) {
		printf("%d,", d.note[i]);
	}
	printf("\nPret mediu: %d\n", d.pretMediu);
	printf("Destinatia: %s\n", d.numeDestinatie);
	printf("Vacanta este: %s\n", d.tipVacanta);
	printf("\n\n");
}
void afisareListaDestinatii(Nod* cap) {
	while (cap!=NULL) {
		afisareDestinatie(cap->info);
		cap = cap->next;
	}
}
void adaugaCladireInLista(Nod** cap, Destinatie nouaDestinatie) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = nouaDestinatie;
	nou->next = NULL;
	if ((*cap)) {
		Nod* p = (*cap);
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		(*cap) = nou;
	}
}
void adaugaLaInceputDeLista(Nod* *cap, Destinatie nouaDestinatie) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = nouaDestinatie;
	nou->next = (*cap);
	(*cap) = nou;
}
Destinatie citireDestinatieDinFisier(FILE* file) {
	Destinatie d;
	char buffer[100];
	char sep[3] = "\n,";
	fgets(buffer, 100, file);
	d.id = atoi(strtok(buffer, sep));
	d.nrRecenzii=atoi(strtok(NULL, sep));
	d.note = (int*)malloc(sizeof(int) * d.nrRecenzii);
	for (int i = 0; i < d.nrRecenzii; i++) {
		d.note[i] = atoi(strtok(NULL, sep));
	}
	d.pretMediu=atoi(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	d.numeDestinatie=(char*)malloc(sizeof(char)*strlen(aux)+1);
	strcpy(d.numeDestinatie, aux);
	aux = strtok(NULL, sep);
	d.tipVacanta=(char*)malloc(sizeof(char)*strlen(aux)+1);
	strcpy(d.tipVacanta, aux);
	return d;
}

Nod* citireListaDestinatiiDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* fisier = fopen(numeFisier, "r");
	if (fisier) {
		while (!feof(fisier)) {
			adaugaCladireInLista(&cap, citireDestinatieDinFisier(fisier));
		}
	}
	fclose(fisier);
	return cap;

}
Nod* stergereDestinatiiDeTip(Nod** cap, const char* tipulCautat) {


	while (*cap && strcmp((*cap)->info.tipVacanta, tipulCautat) == 0) {

		Nod* aux = *cap;
		*cap = (*cap)->next;

		free(aux->info.note);
		free(aux->info.numeDestinatie);
		free(aux->info.tipVacanta);

		free(aux);
	}
	Nod* p = *cap;

	while (p && p->next) {

		if (strcmp(p->next->info.tipVacanta, tipulCautat) == 0) {

			Nod* aux = p->next;
			p->next = aux->next;

			free(aux->info.note);
			free(aux->info.numeDestinatie);
			free(aux->info.tipVacanta);

			free(aux);
		}
		else {
			p = p->next;
		}
	}

	return *cap;
}

int main() {
	Nod* cap = NULL;
	cap = citireListaDestinatiiDinFisier("destinatii.txt");
	afisareListaDestinatii(cap);
	stergereDestinatiiDeTip(&cap, "Exotica");
	printf("\n\n\n");
	afisareListaDestinatii(cap);
	return 0;
}