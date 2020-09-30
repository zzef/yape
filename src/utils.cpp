#include "../include/utils.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"

int random(int a, int b) {
	if (a==b)
		return a;
	return rand() % (b-a) + a;
}
