#ifndef BODY_H
#define BODY_H
#include "includes.h"
#include "Shape.h"
#include <memory>
#include "Manifold.h"
#include "Circle.h"
#include "Polygon.h"
#include "Constraints.h"

class Body : public Polygon, public Circle {
	
	public:
		Body();
		Body(int type);
		Body(int type, bool is_earthed);
		int type;
		void render(Display* d, int options, float ratio);
		void set_type(int type);
		int get_type();
		void identify();
		void set_color(Color color);
		void set_color(Color& color);
		void generate();
		void mouse_contact(bool val); 
		bool get_mouse_contact();
		bool is_earthed();
		bool intersects(Body* b);
		void set_earthed(bool val);
		void reset();
		void apply_impulse(Vec normal,Vec contact);
		void initialize();
		void generate_color();
		void set_density(float density);
		void rect(float height, float width);
		Vec prev_pos;
		float prev_orientation = 0;
		~Body();
		Vec position;
		float w = 0;
		bool visited = false;
		Vec velocity;
		float orientation = 0;
		float im = (float) 1.0f/1000.0f;
		float iI = (float) 1.0f/1000000.0f;

	private:
		bool is_rect = false;
		Color color = {DARK_GREY_2};
		bool mouse_in = false;
		bool is_earth = false;
		float density = 7.2f;

};

#endif
