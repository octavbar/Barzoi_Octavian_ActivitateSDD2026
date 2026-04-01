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
typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** head;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* nod) {
	while (nod->next) {
		afisareMasina((*nod).info);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(Nod** nod, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; // ->sageata face dererferntiere si acces
	nou->next = NULL;
	if (!(*nod)) {
		*nod = nou;
	}
	else {
		Nod* aux = *nod;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.head = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.head[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 31) % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masini) {
	int hashValue = calculeazaHash(masini.id, hash.dim);
	//verificam daca avem coliziune
	if (hash.head[hashValue]) {
		adaugaMasinaInLista(&(hash.head[hashValue]), masini);
		return;
	}
	Nod* nod = malloc(sizeof(Nod));
	nod->info = masini;
	nod->next = NULL;
	hash.head[hashValue] = nod;
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(file);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i]) {
			printf("Cluster: %d\n", i + 1);
			afisareListaMasini(ht.head[i]);
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
	for (int i = 0; i < ht->dim; i++) {
		if (ht->head[i]) {
			Nod* aux = ht->head[i];
			Nod* next = NULL;
			while (aux) {
				free(aux->info.model);
				free(aux->info.numeSofer);
				next = aux->next;
				free(aux);
				aux = next;
			}
		}
	}
	free(ht->head);
	ht->dim = 0;
	ht->head = NULL;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	int nr = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i]) {
			nr++;
		}
	}
	float* mediiPerCluster = malloc(sizeof(float) * nr);
	int j = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i]) {
			float suma = 0;
			int nrNoduri = 0;
			Nod* aux = ht.head[i];
			while (aux) {
				nrNoduri++;
				suma += aux->info.pret;
				aux = aux->next;
			}
			mediiPerCluster[j++] = suma / nrNoduri;
		}
	}
	*nrClustere = nr;
	return mediiPerCluster;
}

Masina getMasinaById(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	int hash = calculeazaHash(id, ht.dim);
	Nod* aux = ht.head[hash];
	while (aux) {
		if (aux->info.id == id) {
			m = aux->info;
			m.model = malloc(strlen(aux->info.model) + 1);
			strcpy_s(m.model, strlen(aux->info.model) + 1, aux->info.model);
			m.numeSofer = malloc(strlen(aux->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(aux->info.numeSofer) + 1, aux->info.numeSofer);
			return m;
		}
	}
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);

	//afisareMasina(getMasinaById(ht, 3));
	int nrClustere;
	float* medie = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("Pret mediu cluster %d: %.2f\n", i + 1, medie[i]);
	}
	dezalocareTabelaDeMasini(&ht);
	return 0;
}
