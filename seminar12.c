#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};

typedef struct StructuraMasina Masina;

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
    Masina m;
    NodP* next;
    NodS* vecini;
};

struct NodS {
    NodS* next;
    NodP* info;
};

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

int citireMasinaDinFisier(FILE* file, Masina* m1) {
    char buffer[200];
    char sep[3] = ",\n";

    if (!fgets(buffer, 200, file)) {
        return 0;
    }

    char* aux = strtok(buffer, sep);
    if (!aux) return 0;

    m1->id = atoi(aux);

    aux = strtok(NULL, sep);
    if (!aux) return 0;
    m1->nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (!aux) return 0;
    m1->pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    if (!aux) return 0;
    m1->model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1->model, aux);

    aux = strtok(NULL, sep);
    if (!aux) return 0;
    m1->numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1->numeSofer, aux);

    aux = strtok(NULL, sep);
    if (!aux) return 0;
    m1->serie = aux[0];

    return 1;
}

void inserareListaPrincipala(NodP** graf, Masina m) {
    NodP* nodNou = (NodP*)malloc(sizeof(NodP));
    nodNou->m = m;
    nodNou->next = NULL;
    nodNou->vecini = NULL;

    if (*graf) {
        NodP* aux = *graf;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nodNou;
    }
    else {
        *graf = nodNou;
    }
}

void inserareListaSecundara(NodS** lista, NodP* vecin) {
    NodS* nodNou = (NodS*)malloc(sizeof(NodS));
    nodNou->info = vecin;
    nodNou->next = NULL;

    if (*lista) {
        NodS* aux = *lista;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nodNou;
    }
    else {
        *lista = nodNou;
    }
}

NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
    NodP* temp = listaPrincipala;
    while (temp) {
        if (temp->m.id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
    NodP* start = cautaNodDupaID(listaPrincipala, idStart);
    NodP* stop = cautaNodDupaID(listaPrincipala, idStop);

    if (start != NULL && stop != NULL) {
        inserareListaSecundara(&(start->vecini), stop);
        inserareListaSecundara(&(stop->vecini), start);
    }
}

NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        return NULL;
    }

    NodP* graf = NULL;
    Masina m;

    while (citireMasinaDinFisier(f, &m)) {
        inserareListaPrincipala(&graf, m);
    }

    fclose(f);
    return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        return;
    }

    int idStart, idStop;

    while (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
        inserareMuchie(graf, idStart, idStop);
    }

    fclose(f);
}

void afisareListaVecini(NodP* graf, int id) {
    NodP* temp = cautaNodDupaID(graf, id);

    if (temp == NULL) {
        printf("Nu exista nod cu id=%d\n", id);
        return;
    }

    NodS* cap = temp->vecini;
    while (cap) {
        afisareMasina(cap->info->m);
        cap = cap->next;
    }
}

void dezalocareListaSecundara(NodS* cap) {
    while (cap) {
        NodS* temp = cap;
        cap = cap->next;
        free(temp);
    }
}

void dezalocareGraf(NodP* graf) {
    while (graf) {
        NodP* temp = graf;
        graf = graf->next;

        free(temp->m.model);
        free(temp->m.numeSofer);

        dezalocareListaSecundara(temp->vecini);

        free(temp);
    }
}

int main() {
    NodP* graf = NULL;

    graf = citireNoduriMasiniDinFisier("masini.txt");
    citireMuchiiDinFisier("muchii.txt", graf);

    afisareListaVecini(graf, 8);

    dezalocareGraf(graf);

    return 0;
}
