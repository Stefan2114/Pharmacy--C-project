#include "DynamicArray.h"
#include <stdlib.h>
#include "Medicine.h"
#include <assert.h>
#include <string.h>

DynamicArray* createDynamicArray(int maxCap, DestroyFunction destroyFct)
{
	DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
	if (arr == NULL)
		return NULL;
	arr->capacity = maxCap;
	arr->size = 0;
	arr->elems = (TElem*)malloc(maxCap * sizeof(TElem));

	if (arr->elems == NULL)
	{
		free(arr);
		return NULL;
	}
	arr->destroyFct = destroyFct;
	return arr;

}


void destroyDynamicArray(DynamicArray* arr)
{
	if (arr == NULL)
		return;
	
	for (int i = 0; i < arr->size; i++)
		arr->destroyFct(arr->elems[i]);
	
	free(arr->elems);
	free(arr);

}



/*
* resizes the array in order to be able to store more elements
* returns NULL in case there isn't enough memory
*/
void resize(DynamicArray* arr)
{
	if (arr == NULL)
		return;

	arr->capacity *= 2;
	TElem* aux = (TElem*)realloc(arr->elems, arr->capacity * sizeof(TElem));
	if (aux == NULL)
		return;
	arr->elems = aux;
}

void addElemToDynamicArray(DynamicArray* arr, TElem elem)
{
	if (arr == NULL)
		return;

	if (arr->size == arr->capacity)
		resize(arr);
	arr->elems[arr->size++] = elem;


}

int removeElemToDynamicArray(DynamicArray* arr, TElem elem)
{
	if (arr == NULL)
		return;
	
	if (elem == NULL)
		return;

	int location = existElem(arr, elem);
	if (location != -1)
	{
		arr->size -= 1;
		arr->destroyFct(arr->elems[location]);
		for (int i = location; i < arr->size; i++)
			arr->elems[i] = arr->elems[i + 1];
		return 1;
	}
	return 0;
	
	

}


int pop(DynamicArray* arr)
{
	if(arr == NULL)
		return;
	if (arr->size > 0)
	{
		arr->destroyFct(arr->elems[arr->size-1]);
		arr->size -= 1;
		return 1;
	}
	return 0;
}


TElem getElem(DynamicArray* arr, int location)
{
	if (arr == NULL)
		return NULL;
	
	if (location >= 0 && location < arr->size)
		return arr->elems[location];
}

int existElem(DynamicArray* arr, TElem elem)
{
	for (int i = 0; i < arr->size; i++)
		if (arr->elems[i] == elem)
			return i;
	return -1;
}

DynamicArray* deepcopy(DynamicArray* arr)
{
	if (arr == NULL)
		return NULL;
	DynamicArray* copiedArr = createDynamicArray(10, arr->destroyFct);
	for (int i = 0; i < arr->size; i++)
	{
		Medicine* newElem = copyMedicine(getElem(arr, i));
		addElemToDynamicArray(copiedArr, newElem);

	}
	return copiedArr;
}

int getSize(DynamicArray* arr)
{
	if (arr == NULL)
		return 0;

	return arr->size;
}


void testAdd()
{
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	addElemToDynamicArray(arr, m1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Paracetamol") == 0);

	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	addElemToDynamicArray(arr, m2);
	assert(getSize(arr) == 2);

	destroyDynamicArray(arr);
}


void testRemove()
{
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	addElemToDynamicArray(arr, m1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Paracetamol") == 0);

	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	addElemToDynamicArray(arr, m2);
	assert(getSize(arr) == 2);

	assert(removeElemToDynamicArray(arr, m1) == 1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Ibuprofen") == 0);
	assert(removeElemToDynamicArray(arr, m1) == 0);

	destroyDynamicArray(arr);
}

void testPop()
{
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	addElemToDynamicArray(arr, m1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Paracetamol") == 0);

	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	addElemToDynamicArray(arr, m2);
	assert(getSize(arr) == 2);

	assert(pop(arr) == 1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Paracetamol") == 0);
	pop(arr);
	assert(pop(arr) == 0);

	destroyDynamicArray(arr);
}

void testExistElem()
{
	DynamicArray* arr = createDynamicArray(10, &destroyMedicine);
	Medicine* m1 = createMedicine("Paracetamol", 20, 5, 10);
	addElemToDynamicArray(arr, m1);
	assert(getSize(arr) == 1);
	assert(strcmp(getName(getElem(arr, 0)), "Paracetamol") == 0);

	Medicine* m2 = createMedicine("Ibuprofen", 200, 3, 15);
	assert(existElem(arr, m1) == 0);
	assert(existElem(arr, m2) == -1);
	destroyMedicine(m2);

	destroyDynamicArray(arr);
}


void testDynamicArray()
{
	testAdd();
	testRemove();
	testPop();
	testExistElem();
}


