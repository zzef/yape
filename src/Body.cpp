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

void Body::set_pos(Vec p) {
	this->x = p.get_x();
	this->y = p.get_y();
}

void Body::set_pos(float x, float y) {
	this->x = x;
	this->y = y;
}

void Body::set_vel(Vec vel) {
	this->set_vel_x(vel.get_x());
	this->set_vel_y(vel.get_y());
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
	this->prev_pos = Vec(this->x,this->y);
	this->prev_orientation = this->orientation;
}

void Body::set_type(int type) {
	this->type = type;
}

int Body::get_type() {
	return this->type;
}

void Body::render(Display* d, int options, float ratio) {

	if (this->type == CIRCLE)
		this->Circle::render(d,Vec(this->x,this->y),this->orientation,color,options);
	
	if (this->type == POLYGON) {
		
		Vec s_p = this->prev_pos;
		Vec e_p(this->x,this->y);
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

void Body::set_x(float x) {
	this->x=x;
}

void Body::set_y(float y) {
	this->y=y;
}

float Body::get_x() {
	return this->x;
}

float Body::get_y() {
	return this->y;
}

float Body::get_orientation() {
	return this->orientation;
}

void Body::set_vel_x(float val) {
	this->vel_x=val;
}

void Body::set_vel_y(float val) {
	this->vel_y=val;
}

float Body::get_vel_x() {
	return this->vel_x;
}

void Body::set_ang_vel(float val) {
	this->ang_vel=val;
}

float Body::get_ang_vel() {
	return this->ang_vel;
}

Vec* Body::get_vel() {
	return new Vec(this->get_vel_x(),this->get_vel_y());
}

float Body::get_vel_y() {
	return this->vel_y;
}

void Body::generate() {
	if (this->get_type()==POLYGON)
		this->generate_polygon();
}

void Body::set_orientation(float angle) {
	this->orientation = angle;
}

bool Body::is_earthed() {
	return this->is_earth;
}

void Body::set_earthed(bool val) {
	this->is_earth=val;
}

void Body::reset() {
	this->ang_vel = 0;
	this->vel_x = 0;
	this->vel_y = 0;		
}

void Body::set_im(float val) {
	this->im = val;
}

float Body::get_im() {
	return this->im;
}

void Body::set_iI(float val) {
	this->iI = val;
}

float Body::get_iI() {
	return this->iI;
}

void Body::apply_impulse(Vec normal, Vec contact) {

	if (this->get_im()==0)
		return;

    Vec velocity = normal * this->im;
    float ang_vel = contact.cross(normal) * this->iI;
	//std::cout << "angular velocity " << ang_vel << std::endl;

	this->vel_x += velocity.get_x();
	this->vel_y += velocity.get_y();
	this->ang_vel += ang_vel;

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

