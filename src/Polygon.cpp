#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Vector.h"
#include "../include/Polygon.h"

Polygon::Polygon() {

}

Polygon::Polygon(int edges) {
	this->edges = edges;	
	this->vertices = edges;
}

void Polygon::print() {
	std::cout<<"HI IM A POLYGON"<<std::endl;
}

float Polygon::get_radius() {
	return this->radius;
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

float Polygon::_rad() {
	return rad;
}

void Polygon::add_vertex(Vec v) {
	this->verts.push_back(v);
	this->vertices++;
	float sz = v.mag();
	if (sz>rad){
		rad = sz;
	}
}

Vec* Polygon::prev_vertex(int index) {

	int previ;
	if (index-1 < 0)
		previ = this->get_vertices()-2;
	else 
		previ = index-1;

	return this->get_vertex(previ);

}

Vec* Polygon::next_vertex(int index) {

	int nexti;
	if (index+1 >= this->get_vertices()-1)
		nexti = 0;
	else
		nexti = index+1;

	return this->get_vertex(nexti);

}

inline Vec* Polygon::get_vertex(int index) {
	if (index<0 || index>this->vertices-1)
		return NULL;

	return &verts[index];
}

void Polygon::set_vertex(int index, Vec v) {
	verts[index] = v;
}

void Polygon::generate_polygon() {
	
	clear_verts();
	
	float edges = random(MIN_V,MAX_V);
	int min = 10;
	int max = 180;
	int radius = random(MIN_POLY_RAD,MAX_POLY_RAD);
	this->radius = radius;
	int sum_of_differences = 0;
	std::vector<float> differences;
	
	int no_differences = edges-1;
	for (int i = 0; i<no_differences; i++) {
		
		if (i==no_differences-1){
			if (sum_of_differences<180){
				min = 180-sum_of_differences;
			}
		}
		int diff = random(min,max);
		differences.push_back(diff);
		sum_of_differences += diff;
	}
	
	int last_seg = random(10,120);
	if (sum_of_differences > 360 - last_seg) {
		for (int i = 0; i<differences.size(); i++) {
			differences[i]*=((float)(360-last_seg)/sum_of_differences);
		}
	}

	float sum = 0;
	for (int i = 0; i<differences.size(); i++) {
		sum+=differences[i];
	}	

	float disp = 0;
	Vec rad = Vec(0,1) * radius;
	verts.push_back(rad);
	int i = 0;
	for (i = 0; i < differences.size() ; i++) {
		disp+=differences[i];
		Vec vertex = rad.rotate(disp*(M_PI/180.0f));
		verts.push_back(vertex);
	}
	add_vertex(verts[0]);
	this->edges=edges;
	this->vertices=i+2;
}

void Polygon::render(Display* d, Vec position, float orientation, Color& color, int options) {

	d->fill_polygon(this->verts, position, orientation, color, options);
	if (options & SHOW_NORMALS) {
		for (int i = 0; i < this->get_vertices()-1; i++) {	
			
			Vec v1 = this->verts[i].rotate(orientation) + position;
			Vec v2 = this->verts[i+1].rotate(orientation) + position;		
			Vec line = v2 - v1;
			Vec ortho = line.cross(1).normalize();	
			Vec mid = v1.mid(v2);
			Vec to = mid + (ortho * 12);
			d->draw_line(mid, to, color,1);	
	
		}
	}

	if (options & SHOW_POLYMIDS)
		d->draw_line(position,verts[0].rotate(orientation) + position, color,1);

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

