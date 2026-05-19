#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date


//AVL arbore binar echilibrat 
//grad de echilibru/factor de echilibru = inaltimea pe partea stanga - inaltimea pe dreapta

//rotire stanga/dreapta 
typedef struct NodArbore NodArbore;

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct NodArbore {
	Masina info;
	NodArbore* left;
	NodArbore* right;
	//uneori avem si grad de echilibru
};
//creare structura pentru un nod dintr-un arbore binar de cautare

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

int calculeazaInaltimeArbore(NodArbore* root) {
//	//calculeaza inaltimea arborelui care este data de 
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (root) {
		return max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right)+1); //add nivel curent
	}
	else {
		return 0;
	}
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.
char calculGE(NodArbore* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->left) - calculeazaInaltimeArbore(root->right);
	} else {
		return 0;
	}
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->right;
	(*root)->right = aux->left;
	aux->left = (*root);
	(*root) = aux;

}
void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->left;
	(*root)->left = aux->right;
	aux->right = (*root);
	(*root) = aux;
}
void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if ((*root)) {
		if ((*root)->info.id> masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		} else {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
		int grad = calculGE((*root));
		if (grad == 2) { //dezechilibrat in partea stanga
			if (calculGE((*root)->left)==-1) {  //doua rotiri
				if (calculGE((*root)->left) == -1) {
					rotireStanga(&(*root)->left);
					rotireDreapta(&(*root));  //[utem si fara & * 
				} else {
					rotireDreapta(&(*root)->left); 
				}
			} else if (grad == -2) {
				if (calculGE((*root)->right)== 1){
					rotireDreapta(&(*root)->right);
				}
				rotireStanga(root);
			}
		}
	} else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
	fclose(f);
	return root;
}

void afisareMasiniDinArbore(NodArbore* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}  //in preordine 4 2 1 3 8 6 5 7 9 10 
}

void dezalocareArboreDeMasini(NodArbore** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->info.numeSofer);
		free((*root)->info.model);

	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root,  const char* numeSofer) {
	float sum = 0;
	if (root) {
		sum = calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
		sum += calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			sum += root->info.pret;
		}
	}
	return sum;
}

int main() {
	NodArbore* root = citireArboreDeMasiniDinFisier("masini.txt");
	float suma = calculeazaPretulMasinilorUnuiSofer(root, "Ionescu");

	afisareMasiniDinArbore(root);

	return 0;
}