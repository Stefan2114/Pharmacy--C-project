#pragma once


typedef void* TElem;
typedef void(*DestroyFunction)(TElem);

typedef struct {
	int capacity, size;
	TElem* elems;
	DestroyFunction destroyFct;
}DynamicArray;



/*creates a dynamic array
	* if there isn't enough space for the array it will return NULL
	* input: the initial capacity of the array and the destroy function of the elems that the array will contain
	* output: a pointer to this array
*/
DynamicArray* createDynamicArray(int maxCap, DestroyFunction destroyFct);
void testDynamicArray();

//frees the space ocupied by the array from the memory by freing each element from it first then the array
void destroyDynamicArray(DynamicArray* arr);

//adds the element from the input to the array and it resize it if it's needed
void addElemToDynamicArray(DynamicArray* arr, TElem elem);

//returns the position of the element from the array or -1 if it doesn't exists
int existElem(DynamicArray* arr, TElem elem);

//returns the size of the element or 0 if it doesn't exist
int getSize(DynamicArray* arr);

//removes the elem from the array if it exists
int removeElemToDynamicArray(DynamicArray* arr, TElem elem);

//returns the element from a given location
TElem getElem(DynamicArray* arr, int location);

//removes the last element from the array if the array is not empty
int pop(DynamicArray* arr);

//returns a deepcopy of the array
DynamicArray* deepcopy(DynamicArray* arr);


