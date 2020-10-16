#include "../include/utils.h"
#include "../include/Vector.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"

int random(int a, int b) {
	if (a==b)
		return a;
	return rand() % (b-a) + a;
}


float interpolate(float a, float b, float ratio) {
	return a + (( b - a ) * ratio);

}

Vec interpolate(Vec a, Vec b, float ratio) {
	return a + (( b - a ) * ratio);
}

struct Color hsv_to_rgb(int h, int s, int v) {

	// function to convert from hsv color space to rgb color space
	// At the moment this function is just returning the same redish pinkish color.
	// You'll need to replace this code with code that converts the
	// color specified by th h, s, v parameters to rgb.

	int r;
	int g;
	int b;

	//------------

	r = 255;
	g = 43;
	b = 142;

	//Your code should replace these 3 lines setting r, g and b to their correct values.
	
 	//------------


	Color col = { r, g, b };
	return col;

}
