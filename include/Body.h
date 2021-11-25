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
		void set_x(float x);
		Vec* get_vel();
		void set_y(float y);
		void set_type(int type);
		int get_type();
		float get_x();
		float get_y();
		float get_orientation();
		void identify();
		void set_orientation(float angle);
		void set_color(Color color);
		void set_color(Color& color);
		void generate();
		void mouse_contact(bool val); 
		bool get_mouse_contact();
		void set_vel(Vec vel);
		void set_vel_x(float val);
		void set_vel_y(float val);
		float get_vel_x();
		float get_vel_y();
		bool is_earthed();
		void set_earthed(bool val);
		void set_ang_vel(float val);
		float get_ang_vel();
		void reset();
		void apply_impulse(Vec normal,Vec contact);
		void initialize();
		void set_im(float val);
		float get_im();	
		void generate_color();
		void set_iI(float val);
		float get_iI();
		void set_density(float density);
		void set_pos(Vec p);
		void set_pos(float x, float y);
		void rect(float height, float width);
		Vec prev_pos;
		float prev_orientation;
		~Body();

	private:
		bool is_rect = false;
		float im = (float) 1.0f/1000.0f;
		float iI = (float) 1.0f/1000000.0f;
		float x = 0;
		float y = 0;
		float ang_vel = 0;
		float orientation = 0;
		Color color = {DARK_GREY_2};
		bool mouse_in = false;
		float vel_x = 0;
		float vel_y = 0;
		bool is_earth = false;
		float density = 7.2f;

};

#endif
