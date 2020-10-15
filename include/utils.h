#ifndef UTILS_H
#define UTILS_H

struct Color {
	float r;
	float g;
	float b;
};

int random(int a, int b);
struct Color hsv_to_rgb(float h, float s, float v);

#endif
