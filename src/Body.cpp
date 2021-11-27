#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Manifold.h"

Body::Body() {

}

Body::Body(int type) {
	this->type = type;
}

Body::Body(int type, bool is_earthed) {
	this->type = type;
	this->is_earth = is_earthed;
}

void Body::generate_color() {
	//struct Color c = hsv_to_rgb(random(0,360),60,100);
	this->color = gen_col();
}

void Body::rect(float height, float width) {
	this->is_rect=true;
	this->add_vertex(Vec(width/2,height/2));
	this->add_vertex(Vec(-width/2,height/2));
	this->add_vertex(Vec(-width/2,-height/2));
	this->add_vertex(Vec(width/2,-height/2));
	this->add_vertex(Vec(width/2,height/2));
	float area = height * width;
	float m = area * this->density;
	this->im = 1.0f/m;
	this->iI = 1.0f/(area*m);	

}

void Body::set_density(float density) {
	this->density = density;
	this->initialize();
}

void Body::initialize() {

	if (this->type == POLYGON) {
		this->init();
		if(!is_rect) {	
			float m = pow(this->Polygon::get_radius(),2) * M_PI * this->density;
			this->im = 1.0f/m;
			this->iI = 1.0f/(this->Polygon::get_radius() * this->Polygon::get_radius() * m );
		}
	}
	this->prev_pos = position;
	this->prev_orientation = this->orientation;
}

void Body::set_type(int type) {
	this->type = type;
}

int Body::get_type() {
	return this->type;
}

bool Body::intersects(Body* b) {
	return !( ( b->position.x - b->_rad() >= position.x + _rad() ) ||
			  ( b->position.x + b->_rad() <= position.x - _rad() ) ||
			  ( b->position.y - b->_rad() >= position.y + _rad() ) ||
			  ( b->position.y + b->_rad() <= position.y - _rad() ) );
}

void Body::render(Display* d, int options, float ratio) {

	if (this->type == CIRCLE)
		this->Circle::render(d,position,this->orientation,color,options);
	
	if (this->type == POLYGON) {
		
		Vec s_p = this->prev_pos;
		Vec e_p = position;
		float s_o = prev_orientation;
		float e_o = this->orientation;
		
		Vec c_p = interpolate(s_p,e_p,ratio);
		float c_o = interpolate(s_o,e_o,ratio);

		this->Polygon::render(d,c_p,c_o,color,options);
	}

}

void Body::mouse_contact(bool val) {
	this->mouse_in = val;
}

bool Body::get_mouse_contact() {
	return this->mouse_in;
}

void Body::generate() {
	if (this->get_type()==POLYGON)
		this->generate_polygon();
}

bool Body::is_earthed() {
	return this->is_earth;
}

void Body::set_earthed(bool val) {
	this->is_earth=val;
}

void Body::reset() {
	this->w = 0;
	velocity = Vec(0,0);	
}

void Body::apply_impulse(Vec normal, Vec contact) {

	if (this->im==0)
		return;

    Vec vel = normal * this->im;
    float ang_vel = contact.cross(normal) * this->iI;
	//std::cout << "angular velocity " << ang_vel << std::endl;

	velocity = velocity + vel;
	this->w += ang_vel;

	//fake air resistance
	//this->vel_x *= 0.9999;
	//this->vel_y *= 0.9999;
	//this->ang_vel *= 0.9999;
}


void Body::set_color(Color color) {
	this->color = color;
}


void Body::set_color(Color& color) {
	this->color = color;
}

void Body::identify() {
}

Body::~Body() {
}

