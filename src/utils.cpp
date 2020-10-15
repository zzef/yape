#include "../include/utils.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"
<<<<<<< HEAD
#include <math.h>
=======
>>>>>>> ff48d433afb346640b1cdb3f3018f41592b056a4

int random(int a, int b) {
	if (a==b)
		return a;
	return rand() % (b-a) + a;
}

<<<<<<< HEAD
struct Color hsv_to_rgb(float h, float s, float v) { 
=======
struct Color hsv_to_rgb(int h, int s, int v) {
>>>>>>> ff48d433afb346640b1cdb3f3018f41592b056a4

	// function to convert from hsv color space to rgb color space
	// At the moment this function is just returning the same redish pinkish color.
	// You'll need to replace this code with code that converts the
	// color specified by th h, s, v parameters to rgb.

<<<<<<< HEAD
	std::cout << "h : " <<h<< std::endl;
	std::cout << "s : " <<s<< std::endl;
	std::cout << "v : " <<v<< std::endl;

	float r, g, b;
	float x, c, m;

	c = (float) (v/100.0f) * (s/100.0f);
	//h = (h / 60% 6);
	x = (float) c*(1.0 - abs(fmod(h/60.0f, 2) -1.0));
	m = (float) (v/100.0f) - c;

	if(h >= 0 && h < 60){
		r = c, g = x, b =0;
	}
	else if(h >=60 && h <120){
		r = x, g = c, b=0;
	}
	else if(h >=120 && h < 180){
		r = 0, g=c, b=x;
	}
	else if(h >= 180 && h <240){
		r = 0, g=x, b=c;
	}
	else if(h >= 240 && h <300){
		r=x, g=0, b=c;
	}
	else if(h >= 300 && h <360){
		r=c, g=0, b=x;
	}
	else{
		r = 0, g = 0, b = 0;
	}

	float R = (r+m)*255;
	float G = (g+m)*255;
	float B = (b+m)*255;

	std::cout << "r : " <<r<< std::endl;
	std::cout << "g : " <<g<< std::endl;
	std::cout << "b : " <<b<< std::endl;


	std::cout << "R : " <<R<< std::endl;
	std::cout << "G : " <<G<< std::endl;
	std::cout << "B : " <<B<< std::endl;


	Color col = { (int) R, (int) G, (int) B };
=======
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
>>>>>>> ff48d433afb346640b1cdb3f3018f41592b056a4
	return col;

}
