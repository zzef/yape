#ifndef BODY_H
#define BODY_H
#include "includes.h"
#include "Shape.h"
#include <memory>
#include "Manifold.h"
#include "Circle.h"
#include "Polygon.h"

class Body : public Polygon, public Circle {
	
	public:
		Body();
		Body(int type);
		int type;
		void render(Display* d, int options);
		void set_x(float x);
		void set_y(float y);
		void set_type(int type);
		int get_type();
		float get_x();
		float get_y();
		float get_orientation();
		void identify();
		void set_orientation(float angle);
		void set_color(char r, char g, char b);
		void set_color(char color[3]);
		void set_orig_color(char r, char g, char b);
		void set_orig_color(char color[3]);
		void reset_color();
		void mouse_contact(bool val); 
		bool get_mouse_contact(); 
		~Body();

	private:
		float x = 0;
		float y = 0;
		float orientation = 0;
		char color[3] = {(char)150,(char)150,(char)150};
		char orig_color[3] = {(char)150,(char)150,(char)150};
		bool mouse_in = false;

};

#endif
