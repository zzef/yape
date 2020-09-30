#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Manifold.h"

Body::Body() {

}

Body::Body(int type) {
	this->type = type;
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

void Body::generate() {
	if (this->get_type()==POLYGON)
		this->generate_polygon();
}

void Body::set_orientation(float angle) {
	this->orientation = angle;
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

