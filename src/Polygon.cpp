#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Vector.h"
#include "../include/Polygon.h"

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

void Polygon::render(Display* d, Vec position, float orientation, char color[3], int options) {

	for (int i = 0; i < this->get_vertices()-1; i++) {	
		
		Vec v1 = this->verts[i].rotate(orientation) + position;
		Vec v2 = this->verts[i+1].rotate(orientation) + position;
		d->draw_line( v1, v2 , color );	
	
		if (options & SHOW_NORMALS) {
			
			Vec line = v2 - v1;
			Vec ortho = line.cross(1).normalize();	
			Vec mid = v1.mid(v2);
			Vec to = mid + (ortho * 12);
			d->draw_line(mid, to, color);	

		}

	}
	//this->draw_line(position,vertices[0].rotate(orientation) + position);

}

void Polygon::init() {
	
	Vec v1 = this->verts[0];
	Vec v2 = this->verts[1];

	//reverse polygon winding order so that normals are facing outwards
	if (v1.cross(v2)>0)	{
		for (int i = 0; i<this->vertices/2; i++) {
			Vec tmp = this->verts[this->vertices-1-i];
			this->verts[this->vertices-1-i] = verts[i];
			verts[i] = tmp;
		}
	}
		
}

Polygon::~Polygon() {

}

