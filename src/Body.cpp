#include "../include/includes.h"
#include "../include/utils.h"

Body::Body() {

}

Body::Body(std::shared_ptr<Shape> c) {
	this->shape = c;
}

void Body::render(Display* d) {
	this->shape->render(d,Vec(this->x,this->y),this->orientation);
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

void Body::set_orientation(float angle) {
	this->orientation = angle;
}

void Body::identify() {
	this->shape->print();	
}

Body::~Body() {
	shape.reset();
}

