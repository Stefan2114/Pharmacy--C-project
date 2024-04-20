
#include "service.h"
#include <string.h>
void generateElems(Service* service);
void saveUndo(Service* service);

Service* createService(Repository* repository)
{

	if (repository == NULL)
		return NULL;

	Service* service = (Service*)malloc(sizeof(Service));
	if (service == NULL)
		destroyRepository(repository);

	//memento undo redo
	DynamicArray* undo = createDynamicArray(10, &destroyDynamicArray);

	if (undo == NULL)
	{
		destroyRepository(repository);
		free(service);
		return NULL;
	}

	//memento undo redo
	DynamicArray* redo = createDynamicArray(10, &destroyDynamicArray);

	if (redo == NULL)
	{
		destroyRepository(repository);
		destroyDynamicArray(undo);
		free(service);
	}
	service->repository = repository;
	service->undo = undo;
	service->redo = redo;
	generateElems(service);
	return service;
}

destroyService(Service* service)
{
	destroyDynamicArray(service->undo);
	destroyDynamicArray(service->redo);
	destroyRepository(service->repository);
	free(service);
}

void addElemService(Service* service, char* name, int concentration, int quantity, int price)
{
	
	Medicine* elem = createMedicine(name, concentration, quantity, price);

	addElemRepository(service->repository, elem);

	//memento undo redo
	saveUndo(service);//saves the new array in the undo array


}

int deleteElemService(Service* service, char* name, int concentration, int quantity)
{

	int res = deleteElemRepository(service->repository, name, concentration, quantity);
	if (res == 1)
	{
		//memento undo redo
		saveUndo(service);//saves the new array in the undo array

	}
	
	return res;
}

int updateElemService(Service* service, char* name, int concentration, int quantity, int price)
{

	int res = updateElemRepository(service->repository, name, concentration, quantity, price);
	if (res == 1)
	{

		//memento undo redo
		saveUndo(service);//saves the new array in the undo array
	}
	return res;

}


//empties the redo list
void emptyRedo(Service* service)
{
	destroyDynamicArray(service->redo);

	//memento undo redo
	service->redo = createDynamicArray(10, &destroyDynamicArray);
}



/*
* for memento undo redo
* it saves the the undo for the last performed operation and empties the redo list
*/
void saveUndo(Service* service)
{
	DynamicArray* arr = getElemsRepository(service->repository);
	DynamicArray* savedArr = deepcopy(arr);
	addElemToDynamicArray(service->undo, savedArr);
	emptyRedo(service);
}


int undo(Service* service)
{

	//memento undo
	if (getSize(service->undo) > 1)
	{

		DynamicArray* savedArr = deepcopy(getElem(service->undo, getSize(service->undo) - 1));
		addElemToDynamicArray(service->redo, savedArr);
		pop(service->undo);
		setElemsRepository(service->repository, getElem(service->undo, getSize(service->undo) - 1));
		return 1;
	}
	return 0;
}

int redo(Service* service)
{

	//memento redo
	if (getSize(service->redo) > 0)
	{
		DynamicArray* savedArr = deepcopy(getElem(service->redo, getSize(service->redo) - 1));
		addElemToDynamicArray(service->undo, savedArr);
		setElemsRepository(service->repository, getElem(service->redo, getSize(service->redo) - 1));
		pop(service->redo);
		return 1;
	}
	return 0;

}



DynamicArray* getElemsByNameService(Service* service, char* name)
{
	
	DynamicArray* arr = getElemsRepository(service->repository); // gets the medicines

	DynamicArray* tempArr = createDynamicArray(arr->size, &destroyMedicine); //creates a new array that will be send to the ui
	
	for (int i = 0; i < arr->size; i++)
	{
		if (strstr(getName(arr->elems[i]), name) != NULL) {

			Medicine* newElem = createMedicine(getName(arr->elems[i]), getConcentration(arr->elems[i]), getQuantity(arr->elems[i]), getPrice(arr->elems[i]));
			//makes a copy of the medicine	
			addElemToDynamicArray(tempArr, newElem);
				
		}
				
	}

	//sorts the medicines found in ascending order by their name
	for (int i = 0; i < tempArr->size - 1; i++)
		for (int j = i + 1; j< tempArr->size; j++)
			if (strcmp(getName(tempArr->elems[i]), getName(tempArr->elems[j])) > 0)
			{
				
				Medicine* aux;
				aux = tempArr->elems[i];
				tempArr->elems[i] = tempArr->elems[j];
				tempArr->elems[j] = aux;
			}
	
	return tempArr;
}



DynamicArray* getElemsByQuantityService(Service* service, int quantity, int ascending)
{
	DynamicArray* arr = getElemsRepository(service->repository);
	DynamicArray* tempArr = createDynamicArray(arr->size, &destroyMedicine);
	
	for (int i = 0; i < arr->size; i++)
	{
		if (getQuantity(arr->elems[i]) < quantity) {
			Medicine* newElem = copyMedicine(getElem(arr, i));
			addElemToDynamicArray(tempArr, newElem);
		}
			
	}

	for (int i = 0; i < tempArr->size - 1; i++)
		for (int j = i + 1; j < tempArr->size; j++)
		{
			if ((getQuantity(tempArr->elems[i]) > getQuantity(tempArr->elems[j])) && ascending == 1)
			{
			
				Medicine* aux;
				aux = tempArr->elems[i];
				tempArr->elems[i] = tempArr->elems[j];
				tempArr->elems[j] = aux;
			
			}
			else if (getQuantity(tempArr->elems[i]) < getQuantity(tempArr->elems[j]) && ascending == 2)
			{
				Medicine* aux;
				aux = tempArr->elems[i];
				tempArr->elems[i] = tempArr->elems[j];
				tempArr->elems[j] = aux;
			}
			
		}


	return tempArr;
}



DynamicArray* getElemsByConcentrationService(Service* service, int concentration)
{
	DynamicArray* arr = getElemsRepository(service->repository);
	DynamicArray* tempArr = createDynamicArray(arr->size, &destroyMedicine);
	for (int i = 0; i < arr->size; i++)
	{
		if (getConcentration(arr->elems[i]) < concentration) {
			Medicine* newElem = copyMedicine(getElem(arr, i));
			addElemToDynamicArray(tempArr, newElem);
		}

	}
	for (int i = 0; i < tempArr->size - 1; i++)
		for (int j = i + 1; j < tempArr->size; j++)
			if (getConcentration(tempArr->elems[i]) > getConcentration(tempArr->elems[j]))
			{
				Medicine* aux;
				aux = tempArr->elems[i];
				tempArr->elems[i] = tempArr->elems[j];
				tempArr->elems[j] = aux;
			}

	return tempArr;
}


/*
* generates 10 medicines for the start of the program
*/
void generateElems(Service* service)
{
	Medicine* medicine1 = createMedicine("Paracetamol", 20, 5, 10);
	Medicine* medicine2 = createMedicine("Ibuprofen", 200, 3, 15);
	Medicine* medicine3 = createMedicine("Aspirina", 300, 8, 8);
	Medicine* medicine4 = createMedicine("Loratadina", 10, 2, 12);
	Medicine* medicine5 = createMedicine("Amoxicilina", 500, 12, 20);
	Medicine* medicine6 = createMedicine("Omeprazol", 20, 6, 30);
	Medicine* medicine7 = createMedicine("Ranitidina", 150, 30, 14);
	Medicine* medicine8 = createMedicine("Diclofenac", 50, 9, 12);
	Medicine* medicine9 = createMedicine("Cetirizina", 10, 1, 15);
	Medicine* medicine10 = createMedicine("Clorfeniramina", 4, 2, 16);
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	addElemToDynamicArray(arr, medicine1);
	addElemToDynamicArray(arr, medicine2);
	addElemToDynamicArray(arr, medicine3);
	addElemToDynamicArray(arr, medicine4);
	addElemToDynamicArray(arr, medicine5);
	addElemToDynamicArray(arr, medicine6);
	addElemToDynamicArray(arr, medicine7);
	addElemToDynamicArray(arr, medicine8);
	addElemToDynamicArray(arr, medicine9);
	addElemToDynamicArray(arr, medicine10);
	setElemsRepository(service->repository, arr);
	destroyDynamicArray(arr);
	saveUndo(service);

}