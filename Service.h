#pragma once
#include "repository.h"


typedef struct {
	Repository* repository;
	DynamicArray* undo;
	DynamicArray* redo;

}Service;

Service* createService(Repository* repository);
destroyService(Service* service);


/*
	* creates a strcuture medicine and adds it to the repository
	* input: the medicine data
	* saves the new array in the undo array
*/
void addElemService(Service* service, char* name, int concentration, int quantity, int price);

/*
	* deletes one qunatity of the medicine that has the given name and concentration from
	* the storage
	* input: the medicine name and concentration
	* saves the new array in the undo array
*/
int deleteElemService(Service* service, char* name, int concentration, int quantity);

// gets all the medicines from the repository and selects only the ones that match the given name
DynamicArray* getElemsByNameService(Service* service, char* name);

// gets all the medicines from the repository and selects only the ones that has quantity lower than the one given
DynamicArray* getElemsByQuantityService(Service* service, int quantity, int ascending);


/*
	* creates a strcuture medicine and updates the one from the repository if it exists
	* input: the medicine data
*/
int updateElemService(Service* service, char* name, int concentration, int quantity, int price);

// gets all the medicines from the repository and selects only the ones that has concentration lower than the one given
DynamicArray* getElemsByConcentrationService(Service* service, int concentration);


int undo(Service* service);
int redo(Service* service);