#include<stdio.h>
#include<malloc.h>

struct Imobil {
	int id;
	int nrEtaje;
	char* denumire;
	int* nrCamere;
	float suprafata;
	char nume;
};
struct Imobil initializare(int id, int nrEtaje, char* denumire, 
	int* nrCamere, float suprafata, char nume) {
	struct Imobil s;
	s.id = id;
	s.nrEtaje = nrEtaje;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);
	s.nrCamere = (int*)malloc(sizeof(int));

	
}

void afisare(struct Imobil s) {
	//afisarea tuturor atributelor.
}

void afisareVector(struct Imobil* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
}

struct Imobil* copiazaPrimeleNElemente(struct Imobil* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Imobil *vectorNou=NULL;

	return vectorNou;
}

void dezalocare(struct Imobil** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
}

void copiazaAnumiteElemente(struct Imobil* vector, char nrElemente, float prag, struct Imobil** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Imobil getPrimulElementConditionat(struct Imobil* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Imobil s;
	s.id = 1;

	return s;
}
	


int main() {

	return 0;
}