#ifndef UTILS_H
#define UTILS_H
#include "Vector.h"

struct Color {
	float r;
	float g;
	float b;
};


struct Color gen_col();
int random(int a, int b);
struct Color hsv_to_rgb(int h, int s, int v);
float interpolate(float a, float b, float ratio);
Vec interpolate(Vec a, Vec b, float ratio);

#endif
