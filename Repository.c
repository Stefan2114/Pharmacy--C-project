#include "repository.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
void generateElems(Repository* repository);


Repository* createRepository()
{
	
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	if (arr == NULL)
		return NULL;
	Repository* repository = (Repository*)malloc(sizeof(Repository));
	if (repository == NULL)
	{
		destroyDynamicArray(arr);
		return NULL;
	}
	repository->arr = arr;
	return repository;
}



void addElemRepository(Repository* repository, Medicine* elem)
{
	Medicine* medicine = getElemRepository(repository, getName(elem), getConcentration(elem));
	if (medicine == NULL)
		addElemToDynamicArray(repository->arr, elem);
	else {
		setQuantity(medicine, getQuantity(medicine) + getQuantity(elem));
		destroyMedicine(elem);
	}
		
}

int deleteElemRepository(Repository* repository, char* name, int concentration, int quantity)
{

	Medicine* medicine = getElemRepository(repository, name, concentration);

	if (medicine == NULL)
		return 0;
	if (getQuantity(medicine) - quantity < 0)
		return 0;
	setQuantity(medicine, getQuantity(medicine) - quantity);
	if(getQuantity(medicine) == 0)
		removeElemToDynamicArray(repository->arr, medicine);
	return 1;

	
}

int updateElemRepository(Repository* repository, char* name, int concentration, int quantity, int price)
{
	Medicine* medicine = getElemRepository(repository, name, concentration);
	if (medicine == NULL)
		return 0;

	setQuantity(medicine, quantity);
	setPrice(medicine, price);
	return 1;

}

int getSizeRepo(Repository* repository)
{
	return getSize(repository->arr);
}

void destroyRepository(Repository* repository)
{
	destroyDynamicArray(repository->arr);
	free(repository);
}


DynamicArray* getElemsRepository(Repository* repository)
{
	return repository->arr;
	
}
void setElemsRepository(Repository* repository, DynamicArray* newArr)
{
	destroyDynamicArray(repository->arr);
	repository->arr = deepcopy(newArr);
}

Medicine* getElemRepository(Repository* repository, char* name, int concentration)
{
	int location = -1;
	for (int i = 0; i < getSize(repository->arr); i++)
	{
		Medicine* medicine = getElem(repository->arr, i);
		if (strcmp(getName(medicine), name) == 0 && getConcentration(medicine) == concentration)
		{
			location = i;
			break;
		}
	}
	if (location == -1)
		return NULL;
	return getElem(repository->arr, location);
}



void testAddRepo()
{
	Repository* repository = createRepository();
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	addElemRepository(repository, m1);
	assert(getSizeRepo(repository) == 1);
	assert(strcmp(getName(getElem(repository->arr, 0)), "Paracetamol") == 0);

	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	addElemRepository(repository, m2);
	assert(getSizeRepo(repository) == 2);

	Medicine* m3 = createMedicine("Paracetamol", 20, 10, 10);
	addElemRepository(repository, m3);
	assert(getSizeRepo(repository) == 2);
	assert(getQuantity(getElem(repository->arr, 0)) == 15);


	// destroy the test repository
	destroyRepository(repository);
}

void testRemoveRepo()
{
	Repository* repository = createRepository();
	Medicine* m1 = createMedicine("Paracetamol", 20, 2, 10);
	addElemRepository(repository, m1);
	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	addElemRepository(repository, m2);
	assert(getSizeRepo(repository) == 2);
	assert(deleteElemRepository(repository, m1->name, m1->concentration, 1) == 1);
	assert(strcmp(getName(getElem(repository->arr, 0)), "Paracetamol") == 0);
	assert(getQuantity(getElem(repository->arr, 0)) == 1);
	assert(getSizeRepo(repository) == 2);
	deleteElemRepository(repository, m1->name, m1->concentration, 1);
	assert(strcmp(getName(getElem(repository->arr, 0)), "Ibuprofen") == 0);
	assert(getQuantity(getElem(repository->arr, 0)) == 3);
	assert(getSizeRepo(repository) == 1);

	Medicine* m3 = createMedicine("Paracetamol", 20, 2, 10);
	assert(deleteElemRepository(repository, m3->name, m3->concentration, 1)==0);
	destroyMedicine(m3);


	// destroy the test repository
	destroyRepository(repository);
}


void testUpdateRepo()
{
	Repository* repository = createRepository();
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	
	addElemRepository(repository, m1);
	assert(getQuantity(getElem(repository->arr, 0)) == 5);
	assert(getPrice(getElem(repository->arr, 0)) == 10);

	Medicine* m2 = createMedicine("Paracetamol", 20, 3, 15);
	addElemRepository(repository, m2);

	Medicine* m3 = createMedicine("Paracetamol", 20, 5, 10);

	assert(updateElemRepository(repository, getName(m3), getConcentration(m3), getQuantity(m3), getPrice(m3)) == 1);
	assert(getQuantity(getElem(repository->arr, 0)) == 5);
	assert(getPrice(getElem(repository->arr, 0)) == 10);

	destroyMedicine(m3);


	Medicine* m4 = createMedicine("Paracetamol", 10, 3, 15);
	assert(updateElemRepository(repository, getName(m4), getConcentration(m4), getQuantity(m4), getPrice(m4)) == 0);

	destroyMedicine(m4);



	// destroy the test repository
	destroyRepository(repository);
}

void testRepo()
{

	testAddRepo();
	testRemoveRepo();
	testUpdateRepo();
}
