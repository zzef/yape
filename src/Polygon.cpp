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

Vec* Polygon::get_vertex(int index) {
	if (index<0 || index>this->vertices-1)
		return NULL;

	return &verts[index];
}

void Polygon::generate_polygon() {
	
	clear_verts();
	
	float edges = random(MIN_V,MAX_V);
	std::cout << "verts " << edges << std::endl;
	int min = 10;
	int max = 180;
	int radius = random(MIN_POLY_RAD,MAX_POLY_RAD);
	int sum_of_differences = 0;
	std::vector<float> differences;
	
	int no_differences = edges-1;
	for (int i = 0; i<no_differences; i++) {
		
		if (i==no_differences-1){
			if (sum_of_differences<180){
				min = 180-sum_of_differences;
				std::cout << "here min " << min << std::endl;
			}
		}
		int diff = random(min,max);
		std::cout << "diff " << diff << std::endl; 
		differences.push_back(diff);
		sum_of_differences += diff;
	}
	
	int last_seg = random(10,120);
	if (sum_of_differences > 360 - last_seg) {
		std::cout << "herer";
		for (int i = 0; i<differences.size(); i++) {
			differences[i]*=((float)(360-last_seg)/sum_of_differences);
		}
	}

	float sum = 0;
	std::cout << "new polygon" << std::endl;
	for (int i = 0; i<differences.size(); i++) {
		std::cout << differences[i] << std::endl;
		sum+=differences[i];
	}	
	std::cout << "final sum " << sum << std::endl;
	std::cout << "\n";

	float disp = 0;
	Vec rad = Vec(0,1) * radius;
	verts[0] = rad;
	int i = 0;
	for (i = 0; i < differences.size() ; i++) {
		disp+=differences[i];
		Vec vertex = rad.rotate(disp);
		verts[i+1] = vertex;
	}
	verts[i+1]=verts[0];
	this->edges=edges;
	this->vertices=i+2;
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

	d->draw_line(position,verts[0].rotate(orientation) + position, color);

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

