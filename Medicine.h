#pragma once
#include <stdlib.h>

typedef struct {
	char* name;
	int concentration, quantity, price;
}Medicine;

Medicine* createMedicine(char* name, int concentration, int quantity, int price);
void destroyMedicine(Medicine* medicine);
char* const getName(Medicine* medicine);
int getConcentration(Medicine* medicine);
int getQuantity(Medicine* medicine);
int getPrice(Medicine* medicine);
void setQuantity(Medicine* medicine, int quantity);
void setPrice(Medicine* medicine, int price);
Medicine* copyMedicine(Medicine* medicine);

void printMedicine(Medicine* medicine);


