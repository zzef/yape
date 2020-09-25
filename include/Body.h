#ifndef BODY_H
#define BODY_H
#include "includes.h"
#include "Shape.h"
#include <memory>

class Body {
	
	public:
		Body();
		Body(std::shared_ptr<Shape> c);
		std::shared_ptr<Shape> shape;
		void render(Display* d);
		void set_x(float x);
		void set_y(float y);
		float get_x();
		float get_y();
		float get_orientation();
		void identify();
		void set_orientation(float angle);
		~Body();

	private:
		float x = 0;
		float y = 0;
		float orientation = 0;

};

#endif
