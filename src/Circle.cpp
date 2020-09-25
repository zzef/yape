#include "../include/Circle.h"
#include "../include/includes.h"
#include "../include/utils.h"

Circle::Circle() {
	this->generate();
}

Circle::Circle(int radius) {
	this->radius = radius;
}

void Circle::print() {
	std::cout<<"HI IM A CIRCLE"<<std::endl;
}

void Circle::set_radius(int radius) {
	this->radius = radius;
}

int Circle::get_radius() {
	return this->radius;
}

void Circle::generate() {
	if (!this->radius)
		radius = random(10,40);
}

void Circle::render(Display* d, Vec position, float orientation) {
	d->draw_circle(position, this->radius, orientation);
}

Circle::~Circle() {

}
