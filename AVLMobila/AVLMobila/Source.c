#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMobilier {
    int    id;
    int    nrSertare;
    float  pret;
    char* denumire;   // pointer 1
    char* material;   // pointer 2
};
typedef struct StructuraMobilier Mobilier;

Mobilier citireMobilierDinFisier(FILE* f) {
    char    buffer[100];
    char    sep[] = ",\n";
    char* aux;
    Mobilier m;

    fgets(buffer, 100, f);

    m.id = atoi(strtok(buffer, sep));

    aux = strtok(NULL, sep);
    m.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(m.denumire, aux);

    aux = strtok(NULL, sep);
    m.material = (char*)malloc(strlen(aux) + 1);
    strcpy(m.material, aux);

    m.nrSertare = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));

    return m;
}

void afisareMobilier(Mobilier m) {
    printf("Id:         %d\n", m.id);
    printf("Denumire:   %s\n", m.denumire);
    printf("Material:   %s\n", m.material);
    printf("Nr sertare: %d\n", m.nrSertare);
    printf("Pret:       %.2f\n\n", m.pret);
}

void dezalocareMobilier(Mobilier* m) {
    free(m->denumire);
    free(m->material);
}

struct NodAVL {
    int inaltime;
    Mobilier info;
    struct NodAVL* stanga;
    struct NodAVL* dreapta;
};
typedef struct NodAVL Nod;

Nod* creareNod(Mobilier m) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->dreapta = NULL;
    nod->stanga = NULL;
    nod->inaltime = 1;
    nod->info = m;
    return nod;
}

int maxDintreDoi(int a, int b) {
    if (a > b) return a;
    return b;
}
int inaltime(Nod* radacina) {
    if (radacina == NULL) return 0;
    return radacina->inaltime;
}
void actualizeazaInaltime(Nod* radacina) {
    if (radacina != NULL)
        radacina->inaltime= 1 + maxDintreDoi(inaltime(radacina->stanga),
            inaltime(radacina->dreapta));
}

Nod* rotatieLaDreapta(Nod* radacina) {
    Nod* B = radacina->stanga;
    Nod* T = B->dreapta;
    B->dreapta= radacina;
    radacina->stanga = T;
    actualizeazaInaltime(radacina);
    actualizeazaInaltime(B);
    return B;
}
Nod* rotatieLaStanga(Nod* radacina) {
    Nod* B = radacina->dreapta;
    Nod* T = B->stanga;
    B->stanga = radacina;
    radacina->dreapta = T;
    actualizeazaInaltime(radacina);
    actualizeazaInaltime(B);
    return B;
}
Nod* echilibreaza(Nod* radacina) {
    int bf = inaltime(radacina->stanga) - inaltime(radacina->dreapta);
    int bfS = 0;
    int bfD = 0;
    if (radacina->stanga != NULL) {
        bfS = inaltime(radacina->stanga->stanga) - inaltime(radacina->stanga->dreapta);
    }
    if (radacina->dreapta != NULL) {
        bfD = inaltime(radacina->dreapta->stanga) - inaltime(radacina->dreapta->dreapta);
    }
    if (bf > 1 && bfS >= 0) 
        return rotatieLaDreapta(radacina);
    if (bf > 1 && bfS < 0) {
        radacina->stanga = rotatieLaStanga(radacina->stanga);
        return rotatieLaDreapta(radacina);
    }

    if (bf < -1 && bfD <= 0) {
        return rotatieLaStanga(radacina);
    }
    if (bf < -1 && bfD >0) {
        radacina->dreapta = rotatieLaStanga(radacina->dreapta);
        return rotatieLaStanga(radacina);
    }
    return radacina;
}
Nod* adaugaRecursivInArbore(Nod* radacina, Mobilier m) {
    
    if (radacina == NULL) return creareNod(m);
    if (m.pret < radacina->info.pret) {
        radacina->stanga = adaugaRecursivInArbore(radacina->stanga, m);
    }
    else {
        if (m.pret > radacina->info.pret) {
            radacina->dreapta = adaugaRecursivInArbore(radacina->dreapta, m);
        }
    }
    return echilibreaza(radacina);
}
void adaugaInArbore(Nod** radacina, Mobilier m) {
    *radacina = adaugaRecursivInArbore(*radacina, m);
}
Nod* citesteArboreDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* arbore = NULL;
    while (!feof(f)) {
        adaugaInArbore(&arbore, citireMobilierDinFisier(f));
    }
    fclose(f);
    return arbore;
}

void afiseazaPostOrdine(Nod* radacina) {
    if (radacina == NULL) return;
    afiseazaPostOrdine(radacina->stanga);
    afiseazaPostOrdine(radacina->dreapta);
    afisareMobilier(radacina->info);
}
int main() {
    Nod* arbore = citesteArboreDinFisier("mobila.txt");
    afiseazaPostOrdine(arbore);
}