#ifndef UTILS_H
#define UTILS_H

struct Color {
	int r;
	int g;
	int b;
};

int random(int a, int b);
struct Color hsv_to_rgb(int h, int s, int v);

#endif
