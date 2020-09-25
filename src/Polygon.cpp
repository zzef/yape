#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Vector.h"

Polygon::Polygon() {

}

Polygon::Polygon(int vertices) {
	this->vertices=vertices;
	this->edges = vertices;	
}

void Polygon::print() {
	std::cout<<"HI IM A POLYGON"<<std::endl;
}

int Polygon::get_vertices() {
	return this->vertices;
}

int Polygon::get_edges() {
	return this->edges;
}

void Polygon::clear_verts() {
	this->vertices=0;
}

void Polygon::add_vertex(Vec v) {
	this->verts[this->vertices] = v;
	this->vertices++;
}

Vec* Polygon::get_vertex(int index) {
	if (index<0 || index>this->vertices-1)
		return NULL;

	return &verts[index];
}

void Polygon::generate() {
	
	clear_verts();
	if (this->vertices == 0) {
		this->vertices = random(MIN_V,MAX_V);
		this->edges = this->vertices;
	}	

}


void Polygon::render(Display* d, Vec position, float orientation) {
	d->render_polygon(position,verts,this->vertices,orientation);
}

Polygon::~Polygon() {

}

