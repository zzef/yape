#include "../include/includes.h"
#include "../include/utils.h"

Vec::Vec() {
	
}

float Vec::get_x() const {
	return this->x;
} 

float Vec::get_y() const {

	return this->y;
}
		
void Vec::set_x(float x) {
	this->x = x;
}

void Vec::set_y(float y) {
	this->y = y;
}

Vec::Vec(float x, float y) {
	this->x = x;
	this->y = y;
}

Vec::Vec(float x, float y, float x1, float y1) {
	this->x = x1-x;
	this->y = y1-y;
}

float Vec::cross(Vec& v) {
	return (this->x*v.y) - (this->y*v.x);
}

Vec Vec::mid(Vec& v) {
	Vec vec((v.x+this->x)/2,(v.y+this->y)/2);
	return vec;
}

Vec Vec::absolute() {
	Vec vec(abs(this->x),abs(this->y));
	return vec;
}

Vec Vec::cross(float value) {
	Vec vec(-value*this->y,value*this->x);
	return vec;
}

float Vec::scalar_proj(Vec& v) {
	return this->dot(v)/this->mag();
}

float Vec::dot(Vec& v) {
	return (this->x*v.x) + (this->y*v.y);
}

float Vec::mag() {
	return sqrt((this->x*this->x) + (this->y*this->y));
}

Vec Vec::normalize() {
	float m = this->mag();
	Vec vec(this->x/m,this->y/m);
	return vec;
}

Vec Vec::rotate(float w) {
		
	float r = ((w*M_PI)/180);
    float csn = cos(r);
    float sn = sin(r);
    Vec vec(this->x * csn - this->y * sn, this->x * sn + this->y * csn);
	return vec;

}

Vec Vec::operator+(const Vec& v) {
	Vec vec(v.x+this->x,v.y+this->y);
	return vec;
}

Vec Vec::operator-(const Vec& v) {
	Vec vec(-v.x+this->x,-v.y+this->y);
	return vec;
}
		
Vec Vec::operator*(float s) {
	Vec vec(this->x*s,this->y*s);
	return vec;
}

Vec Vec::operator/(float s) {
	Vec vec(this->x/s,this->y/s);
	return vec;
}

void Vec::print() {
	std::cout << this->x << "," << this->y << std::endl;
}
