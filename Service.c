
#include "service.h"
#include <string.h>
void generateElems(Service* service);
void saveUndo(Service* service);
void saveUndoOperation(Service* service, Medicine* medicine, char* operationType);


Service* createService(Repository* repository)
{

	if (repository == NULL)
		return NULL;

	Service* service = (Service*)malloc(sizeof(Service));
	if (service == NULL)
		destroyRepository(repository);

	//memento undo redo
	DynamicArray* undo = createDynamicArray(10, &destroyDynamicArray);

	//function pointer undo redo
	//DynamicArray* undo = createDynamicArray(10, &destroyOperation);

	if (undo == NULL)
	{
		destroyRepository(repository);
		free(service);
		return NULL;
	}

	//memento undo redo
	DynamicArray* redo = createDynamicArray(10, &destroyDynamicArray);

	//function pointer undo redo
	//DynamicArray* redo = createDynamicArray(10, &destroyOperation);

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
	service->addFct = &addElemRepository;
	service->removeFct = &deleteElemRepository;
	service->updateFct = &updateElemRepository;
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

	//function pointer undo redo
	//Medicine* medicine = copyMedicine(elem);

	addElemRepository(service->repository, elem);

	//memento undo redo
	saveUndo(service);//saves the new array in the undo array

	//function pointer undo redo
	//saveUndoOperation(service, medicine, "add");
	//destroyMedicine(medicine);

}

int deleteElemService(Service* service, char* name, int concentration, int quantity)
{
	//function pointer undo redo
	//Medicine* medicine = copyMedicine(getElemRepository(service->repository, name, concentration));
	//setQuantity(medicine, quantity);

	int res = deleteElemRepository(service->repository, name, concentration, quantity);
	if (res == 1)
	{
		//memento undo redo
		saveUndo(service);//saves the new array in the undo array

		//function pointer undo redo
		//saveUndoOperation(service, medicine, "remove");
		//destroyMedicine(medicine);
	}
	
	return res;
}

int updateElemService(Service* service, char* name, int concentration, int quantity, int price)
{
	//function pointer undo redo
	//Medicine* medicine = copyMedicine(getElemRepository(service->repository, name, concentration));

	int res = updateElemRepository(service->repository, name, concentration, quantity, price);
	if (res == 1)
	{

		//memento undo redo
		saveUndo(service);//saves the new array in the undo array

		//function pointer undo redo
		//saveUndoOperation(service, medicine, "update");
		//destroyMedicine(medicine);
	}
	return res;

}


//empties the redo list
void emptyRedo(Service* service)
{
	destroyDynamicArray(service->redo);

	//memento undo redo
	service->redo = createDynamicArray(10, &destroyDynamicArray);

	//operation pointer undo redo
	//service->redo = createDynamicArray(10, &destroyOperation);
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



/*
* for function pointer undo redo
* it saves the the undo for the last performed operation and empties the redo list
*/
void saveUndoOperation(Service* service, Medicine* medicine, char* operationType)
{
	Operation* operation = createOperation(medicine, operationType);
	addElemToDynamicArray(service->undo, operation);
	emptyRedo(service);
}



/*
* for function pointer undo redo
* it makes the undo/redo for add and creates a new operation that will be added to the redo/undo stack
*/
Operation* addOperation(Service* service, Medicine* medicine)
{
	service->removeFct(service->repository, getName(medicine), getConcentration(medicine), getQuantity(medicine));
	Operation* newOperation = createOperation(medicine, "remove");
	return newOperation;
}


/*
* for function pointer undo redo
* it makes the undo/redo for remove and creates a new operation that will be added to the redo/undo stack
*/
Operation* removeOperation(Service* service, Medicine* medicine)
{
	service->addFct(service->repository, copyMedicine(medicine));
	Operation* newOperation = createOperation(medicine, "add");
	return newOperation;
}



/*
* for function pointer undo redo 
* it makes the undo/redo for update and creates a new operation that will be added to the redo/undo stack
*/
Operation* updateOperation(Service* service, Medicine* medicine)
{
	Medicine* oldMedicine = getElemRepository(service->repository, getName(medicine), getConcentration(medicine));
	Operation* newOperation = createOperation(oldMedicine, "update");
	service->updateFct(service->repository, getName(medicine), getConcentration(medicine), getQuantity(medicine), getPrice(medicine));
	return newOperation;
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


	//operation pointer undo
	/*if (getSize(service->undo) == 0)
		return 0;

	Operation* lastOperation = getElem(service->undo, getSize(service->undo) - 1);
	Medicine* medicine = getOperationMedicine(lastOperation);
	Operation* newOperation = NULL;
	if (strcmp(getOperationType(lastOperation), "add") == 0)
		newOperation = addOperation(service, medicine);
	if (strcmp(getOperationType(lastOperation), "remove") == 0)
		newOperation = removeOperation(service, medicine);
	if (strcmp(getOperationType(lastOperation), "update") == 0)
		newOperation = updateOperation(service, medicine);
	addElemToDynamicArray(service->redo, newOperation);
	pop(service->undo);
	return 1;
	*/
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


	//operation pointer redo
	/*if (getSize(service->redo) == 0)
		return 0;
	Operation* lastOperation = getElem(service->redo, getSize(service->redo) - 1);
	Medicine* medicine = getOperationMedicine(lastOperation);
	Operation* newOperation = NULL;
	if (strcmp(getOperationType(lastOperation), "add") == 0)
		newOperation = addOperation(service, medicine);
	if (strcmp(getOperationType(lastOperation), "remove") == 0)
	{
		newOperation = removeOperation(service, medicine);
	}
	
	if (strcmp(getOperationType(lastOperation), "update") == 0)
		newOperation = updateOperation(service, medicine);
	addElemToDynamicArray(service->undo, newOperation);
	pop(service->redo);
	return 1;*/
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