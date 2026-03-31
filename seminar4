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

typedef struct Nod {
    Masina info;
    struct Nod* next;
} Nod;

// Citire masina din fisier
Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[] = ",\n";
    char* aux;

    Masina m1;
    m1.id = 0;
    m1.nrUsi = 0;
    m1.pret = 0;
    m1.model = NULL;
    m1.numeSofer = NULL;
    m1.serie = 0;

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return m1;
    }

    aux = strtok(buffer, sep);
    if (aux == NULL) return m1;
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m1.model);
        m1.model = NULL;
        return m1;
    }
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m1.model);
        free(m1.numeSofer);
        m1.model = NULL;
        m1.numeSofer = NULL;
        return m1;
    }
    m1.serie = aux[0];

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
    while (cap != NULL) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;   // shallow copy
    nou->next = NULL;

    if (*cap != NULL) {
        Nod* p = *cap;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = nou;
    }
    else {
        *cap = nou;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = *cap;
    *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }

    Nod* lista = NULL;

    while (1) {
        Masina masina = citireMasinaDinFisier(file);

        // daca nu s-a citit nimic valid, iesim
        if (masina.model == NULL || masina.numeSofer == NULL) {
            break;
        }

        adaugaMasinaInLista(&lista, masina);
    }

    fclose(file);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    Nod* head = *lista;
    while (head != NULL) {
        Nod* next = head->next;
        free(head->info.model);
        free(head->info.numeSofer);
        free(head);
        head = next;
    }
    *lista = NULL;
}

float calculeazaPretMediu(Nod* lista) {
    float suma = 0;
    int nrMasini = 0;

    while (lista != NULL) {
        suma += lista->info.pret;
        nrMasini++;
        lista = lista->next;
    }

    if (nrMasini != 0) {
        return suma / nrMasini;
    }

    return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
    float suma = 0;
    int nr = 0;

    while (cap != NULL) {
        if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
            suma += cap->info.pret;
            nr++;
        }
        cap = cap->next;
    }

    if (nr > 0) {
        return suma / nr;
    }
    return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
    while (*cap != NULL && (*cap)->info.serie == serieCautata) {
        Nod* temp = *cap;
        *cap = (*cap)->next;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }

    Nod* curent = *cap;
    while (curent != NULL && curent->next != NULL) {
        if (curent->next->info.serie == serieCautata) {
            Nod* temp = curent->next;
            curent->next = temp->next;
            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
        }
        else {
            curent = curent->next;
        }
    }
}

int main() {
    Nod* lista = citireListaMasiniDinFisier("masini.txt");

    if (lista == NULL) {
        printf("Lista este goala sau fisierul nu a putut fi citit.\n");
        return 0;
    }

    afisareListaMasini(lista);

    float pretMediu = calculeazaPretMediu(lista);
    printf("\nPret mediu = %.2f\n", pretMediu);

    printf("\nPret mediu pentru soferul Ana = %.2f\n",
        calculeazaPretulMasinilorUnuiSofer(lista, "Ana"));

    // exemplu stergere
    // stergeMasiniDinSeria(&lista, 'A');
    // afisareListaMasini(lista);

    dezalocareListaMasini(&lista);

    printf("\nDupa dezalocare nu mai avem nimic:\n");
    afisareListaMasini(lista);

    return 0;
}
