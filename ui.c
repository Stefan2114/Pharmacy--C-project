#include "ui.h"

UI* createUI(Service* service)
{
	UI* ui = (UI*)malloc(sizeof(UI));
	if (ui == NULL)
		return NULL;
	ui->service = service;
	return ui;
}

void destroyUI(UI* ui)
{
	destroyService(ui->service);
	free(ui);
}

void readString(char* message, int maxSize, char* string)
{
	/*
	Reads a string with spaces from the standard input.
	Input:	message - string, message to be shown to the user.
			maxStrSize - the maximum size for the string to be read
	Output: the string that was read.
	*/
	int res = 0;
	getchar(); // read the newline, to prevent it from going further to fgets
	printf("%s", message);
	fgets(string, maxSize, stdin);
	printf("\n");
	string[strlen(string) - 1] = '\0'; // the newline is also read so we must eliminate it from the string
}


int readInteger(char* message)
{
	/*
	Reads an integer number from the keyboard. Asks for number while read errors encoutered.
	Input: the message to be displayed when asking the user for input.
	Returns the number.
	*/
	char* string[16];
	int ok = 0;
	int result;
	while (ok == 0)
	{
		printf("%s", message);
		scanf("%s", string);
		ok = sscanf(string, "%u", &result);// reads data from s and stores them as integer, if possible; returns 1 if successful
		if (ok == 0)
			printf("Error reading the number\n");

	}
	printf("\n");
	return result;
}

int addElemUI(UI* ui)
{
	//reads the medicine's data and it adds it to the storage
	char name[30];
	int concentration, quantity, price;
	readString("The name of the medicine is: ", 30, name);
	concentration = readInteger("The concentration of the medicine is: ");
	quantity = readInteger("The qunatity of the medicine is: ");
	if (quantity < 1)
		return 0;
	price = readInteger("The price of the medicine is: ");
	addElemService(ui->service, name, concentration, quantity, price);
	return 1;
}

int deleteElemUI(UI* ui)
{
	//reads the medicine's name and concentration and it deletes one qunatity
	//of it from the storage if it's found
	char name[30];
	int concentration, quantity;
	readString("The name of the medicine is: ", 30, name);
	concentration = readInteger("The concentration of the medicine is: ");
	quantity = readInteger("The qunatity of the medicine is: ");

	return deleteElemService(ui->service, name, concentration, quantity);
}


int updateElemUI(UI* ui)
{
	//reads the medicine's data and it updates it if it's found
	char name[30];
	int concentration, quantity, price;
	readString("The name of the medicine is: ", 30, name);
	concentration = readInteger("The concentration of the medicine is: ");
	quantity = readInteger("The qunatity of the medicine is: ");
	if (quantity < 1)
		return 2;
	price = readInteger("The price of the medicine is: ");
	return updateElemService(ui->service, name, concentration, quantity, price);
}

void printElemsUI(DynamicArray* arr)
{
	//prints all the medicines from the given array

	for (int i = 0; i < getSize(arr); i++)
	{
		printMedicine(getElem(arr, i));
		printf("\n");
	}
}


void printElemsByName(UI* ui)
{
	//prints all the medicines that match the given string
	char name[30];
	readString("The name of the medicine is: ", 30, name);
	DynamicArray* arr = getElemsByNameService(ui->service, name);
	printElemsUI(arr);
	destroyDynamicArray(arr);
}

void printElemsByConcentration(UI* ui)
{
	//prints all the medicines that have less than the concentration given
	int concentration;
	concentration = readInteger("The concentration of the medicine is: ");
	DynamicArray* (*ElemsByConcentrationFunction)(Service*, int) = &getElemsByConcentrationService;
	DynamicArray* arr = (*ElemsByConcentrationFunction)(ui->service, concentration);
	printElemsUI(arr);
	destroyDynamicArray(arr);
}



void printElemsOnShortSuply(UI* ui)
{
	//prints all the medicines that have less than the qunatity given
	//in ascending or descending order
	int quantity;
	quantity = readInteger("The quantity of the medicine is: ");
	printf("The Medicines should be sorted ascending by quantity?\n");
	printf("1. YES\n");
	printf("2. NO\n");

	int ascending = ascending = readInteger("Your choice is: ");
	while (ascending != 1 && ascending != 2)
	{
		printf("Please enter a valid command");
		ascending = readInteger("Your choice is: ");
	}

	DynamicArray* (*ElemsByQuantity)(Service*, int, int) = &getElemsByQuantityService;

	DynamicArray* arr = (*ElemsByQuantity)(ui->service, quantity, ascending);
	printElemsUI(arr);
	destroyDynamicArray(arr);
}


int undoUI(UI* ui)
{
	//undos the last performed instruction
	return undo(ui->service);
}

int redoUI(UI* ui)
{
	//redos the last performed undo
	return redo(ui->service);
}

void print_menu_UI()
{
	//prints the menu for the user
	printf("1. Add Medicine\n");
	printf("2. Remove Medicine\n");
	printf("3. Update Medicine\n");
	printf("4. Print Medicine by Name\n");
	printf("5. See medicine on short concentration\n");
	printf("6. See medicine on short suply\n");
	printf("7. Undo\n");
	printf("8. Redo\n");
	printf("9. Exit\n");
	printf("What is your choice: ");

}

void run(UI* ui)
{
	while (1)
	{
		print_menu_UI();
	
		int	option = readInteger("Your option is: ");
		while (option > 9)
		{
			printf("Please enter a valid command\n");
			option = readInteger("Your option is: ");
		}
		printf("\n");

		if (option == 9)
			break;
		switch (option)
		{
		case 1:
		{
			
			int res = addElemUI(ui);
			if (res == 1)
				printf("The medicine was succesfully added to the storage\n");
			else
				printf("The quantity must be > 0\n");
			break;
		}
		case 2:
		{
			int res = deleteElemUI(ui);
			if (res == 1)
				printf("One quantity of the medicine was succesfully removed from the storage\n");
			else
				printf("There is no medicine with that name and concentration\n");
			break;
		}
		case 3:
		{
			int res = updateElemUI(ui);
			if (res == 1)
				printf("The medicine with that name and concentration was succesfully updated\n");
			else if(res == 0)
				printf("There is no medicine with that name and concentration\n");
			else
				printf("The quantity must be > 0\n");
			break;
		}
		case 4:
		{
			printElemsByName(ui);
			break;
		}
		case 5:
		{
			printElemsByConcentration(ui);
			break;
		}
		case 6:
		{
			printElemsOnShortSuply(ui);
			break;
		}
		case 7:
		{
			int res = undoUI(ui);
			if (res == 1)
				printf("The undo was succesfully made\n");
			else
				printf("There are no more undos possible\n");
			break;
		}
		case 8:
		{
			int res = redoUI(ui);
			if (res == 1)
				printf("The redo was succesfully made\n");
			else
				printf("There are no more redos possible\n");
			break;
		}
		default:
			break;
		}
		
		printf("\n");

	}
}






