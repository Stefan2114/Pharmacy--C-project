#include "ui.h"
#include <crtdbg.h>
#include <assert.h>
#include <stdio.h>



int main()
{
	testDynamicArray();
	testRepo();
	Repository* repository = createRepository();
	Service* service = createService(repository);
	UI* ui = createUI(service);
	run(ui);

	destroyUI(ui);
	if (_CrtDumpMemoryLeaks() == 0)
	{
		printf("There are no memory leaks\n");
	}
	return 0;
}