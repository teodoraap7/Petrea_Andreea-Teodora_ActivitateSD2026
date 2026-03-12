#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int pret;
	char* denumire;
	float pixeli ;
	char serie;
};
struct Telefon initializare(int id, int pret, char* denumire, float pixeli,
	char serie) {
	struct Telefon t;
	t.id = 1;
	t.pret = pret;
	t.denumire = (char*)malloc(sizeof(char) *( strlen(denumire) + 1));
	strcpy(t.denumire, denumire);
	t.pixeli = pixeli;
	t.serie = serie;
	return t;
}

void afisare(struct Telefon s) {
	printf("Id: %d ", s.id);
	printf("Pret: %d ", s.pret);
	printf("Denumire: %s ", s.denumire);
	printf("Pixeli: %f", s.pixeli);
	printf("Serie: %c", s.serie);
	printf("\n");
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Telefon *vectorNou=NULL;
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].denumire =(char*) malloc(sizeof(strlen(vector[i].denumire) + 1));
		strcpy(vectorNou[i].denumire, vector[i].denumire);
	}
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		free((*vector)[i].denumire);
	}
	free(*vector);
}

void copiazaAnumiteElemente(struct Telefon* vector, char nrElemente, float prag, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	struct Telefon* vectorNou = NULL;
	return vectorNou;
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon s;
	s.id = 1;

	return s;
}
	


int main() {
	struct Telefon t;
	t = initializare(1, 200, "iPhone", 360, 'A');
	struct Telefon t2;
	t2 = initializare(2, 400, "Samsung", 820, 'A');
	afisare(t);
	struct Telefon* telefoane=NULL;
	int nrTelefoane = 3;
	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] =t;
	telefoane[1] = t2;
	telefoane[2] = t2;
	afisareVector(telefoane,nrTelefoane);

	struct Telefon* primeleTelefoane = NULL;
	primeleTelefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * 2);
	primeleTelefoane = copiazaPrimeleNElemente(telefoane, 3,2);
	printf("\nAFISARE PRIMELE 2 elemente \n");
	afisareVector(primeleTelefoane, 2);
	return 0;
}