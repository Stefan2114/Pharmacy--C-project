#pragma once

#include "DynamicArray.h"
#include "Medicine.h"


typedef struct
{
	DynamicArray* arr;
	DynamicArray* undo;
	DynamicArray* redo;
} Repository;


/*
	* creates a repository structure that is dynamic allocated
	* arr: the array that will contain the medicines
	* undo: the array that will contain the previous arrays of medicines
	* redo: the array that will contain the previous arrays from the undo
	* returns the pointer to the repository created or NULL if there wasn't enough space
*/
Repository* createRepository();
void destroyRepository(Repository* repository);

//if the elem is not in the array it will be added and if it already exist it will just update the qunatity of the elem
void addElemRepository(Repository* repository, Medicine* elem);


//if will subtact from the quantity of the elem from array the quantity given and if it gets to 0 it will remove the elem from the array
//return 0 if we try to remove a quantity biger than the one found in the array or the elem doesn't exist
int deleteElemRepository(Repository* repository, char* name, int concentration, int quantity);


//if will update the quantity and the price of the medicine with the given ones
//return 0 the elem doesn't exist
int updateElemRepository(Repository* repository, char* name, int concentration, int quantity, int price);

//returns the array from the repository
DynamicArray* getElemsRepository(Repository* repository);

//returns an elem from repository array if the matches the name and concentration
Medicine* getElemRepository(Repository* repository, char* name, int concentration);

//returns the length of the array from repository
int getSizeRepo(Repository* repository);

//sets a new array for repository
void setElemsRepository(Repository* repository, DynamicArray* newArr);
void testRepo();