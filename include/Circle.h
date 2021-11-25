#ifndef CIRCLE_H
#define CIRCLE_H

#include "includes.h"
#include "Shape.h"

class Circle : public Shape {

	public:
		Circle();
		Circle(int radius);
		~Circle();
		void set_radius(int radius);
		int get_radius();
		void print();
		void render(Display* d, Vec position, float orientation, Color& color, int options);

	private:
		int radius = 0;
		void generate();

};

#endif
