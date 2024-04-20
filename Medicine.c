#include "Medicine.h"
#include <string.h>

Medicine* createMedicine(char* name, int concentration, int quantity, int price)
{
    /*
    * creates a dynamic allocated structure Medicine
    * if there isn't enough space for the medicine it will return NULL
    * input: the medicine's data
    * output: a pointer to the new medicine created
    */
    Medicine* medicine = (Medicine*)malloc(sizeof(Medicine));
    if (medicine == NULL)
        return NULL;
    medicine->name = (char*)malloc((strlen(name)+1) * sizeof(char));
    if (medicine->name == NULL)
    {
        free(medicine);
        return NULL;
    }
    strcpy(medicine->name, name);

    medicine->concentration = concentration;
    medicine->quantity = quantity;
    medicine->price = price;
    return medicine;
}


void destroyMedicine(Medicine* medicine)
{
    //frees the space allocated for the medicine
    if (medicine == NULL)
        return;
    free(medicine->name);
    free(medicine);

}

char* const getName(Medicine* medicine)
{
   //return the string that cointains the name of the medicine
    if (medicine == NULL || medicine->name == NULL)
        return NULL;


 
    return medicine->name;
}

int getConcentration(Medicine* medicine)
{
    //return the concentraion of the medicine
    if (medicine == NULL)
        return NULL;
    return medicine->concentration;
}

int getQuantity(Medicine* medicine)
{
    //return the qunatity of the medicine
    if (medicine == NULL)
        return NULL;
    return medicine->quantity;
}

int getPrice(Medicine* medicine)
{
    //return the price of the medicine
    if (medicine == NULL)
        return NULL;
    return medicine->price;
}


void setQuantity(Medicine* medicine, int quantity)
{
    //sets a new qunatity to the medicine
    if (medicine == NULL)
        return NULL;
    medicine->quantity= quantity;
}

void setPrice(Medicine* medicine, int price)
{
    //sets a new price to the medicine
    if (medicine == NULL)
        return NULL;
    medicine->price = price;


}

Medicine* copyMedicine(Medicine* medicine)
{
    //creates a new medicine with the exact data as the one from the input and it returns a pointer to this new medicine
    if (medicine == NULL)
        return NULL;
    Medicine* copiedMedicine = createMedicine(getName(medicine), getConcentration(medicine), getQuantity(medicine), getPrice(medicine));
    return copiedMedicine;
}

void printMedicine(Medicine* medicine)
{
    //the output data of the medicine that will be seen by the user
    printf("Name: %s\n", medicine->name);
    printf("Concentration: %u\n", medicine->concentration);
    printf("Quantity: %u\n", medicine->quantity);
    printf("Price: %u\n", medicine->price);
}

