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

void Body::initialize() {

	if (this->type == POLYGON) {
		this->init();
		float m = this->Polygon::get_radius() * this->Polygon::get_radius() * M_PI * 7.2f;
		this->im = 1.0f/m;
		this->iI = 1.0f/(this->Polygon::get_radius() * this->Polygon::get_radius() * 250.0f/m * 600000000);
	}
}

void Body::set_type(int type) {
	this->type = type;
}

int Body::get_type() {
	return this->type;
}

void Body::render(Display* d, int options) {

	if (this->type == CIRCLE)
		this->Circle::render(d,Vec(this->x,this->y),this->orientation,color,options);
	if (this->type == POLYGON)
		this->Polygon::render(d,Vec(this->x,this->y),this->orientation,color,options);

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

    Vec velocity = normal * this->im;
    float ang_vel = contact.cross(normal) * this->iI;
	//std::cout << "angular velocity " << ang_vel << std::endl;

	this->vel_x += velocity.get_x();
	this->vel_y += velocity.get_y();
	this->ang_vel += ang_vel;
}

void Body::set_orig_color(char r, char g, char b) {
	this->orig_color[0] = r;
	this->orig_color[1] = g;
	this->orig_color[2] = b;
	this->reset_color();
}

void Body::set_orig_color(char color[3]) {
	this->orig_color[0] = color[0];
	this->orig_color[1] = color[1];
	this->orig_color[2] = color[2];
	this->reset_color();
}


void Body::set_color(char r, char g, char b) {
	this->color[0] = r;
	this->color[1] = g;
	this->color[2] = b;
}

void Body::set_color(char color[3]) {
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}

void Body::reset_color() {
	this->color[0] = this->orig_color[0];
	this->color[1] = this->orig_color[1];
	this->color[2] = this->orig_color[2];
}

void Body::identify() {
}

Body::~Body() {
}

