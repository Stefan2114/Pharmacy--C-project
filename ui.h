#pragma once
#include "service.h"
#include <stdio.h>

typedef struct {
	Service* service;

}UI;

UI* createUI(Service* service);
void run(UI* ui);
void destroyUI(UI* ui);

