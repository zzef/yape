#ifndef SHAPE_H
#define SHAPE_H

#include "includes.h"

class Shape {
	
	public:
		Shape();
		virtual ~Shape() = 0;
		virtual void print() = 0;
		virtual void render(Display* d, Vec position, float orientation, Color& color, int options) = 0;

	private:
		virtual void generate() = 0;

};

#endif
