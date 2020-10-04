#ifndef POLYGON_H
#define POLYGON_H

#include "includes.h"
#include "Shape.h"

class Polygon : public Shape {

	public:
		Polygon();
		Polygon(int edges);
		int get_vertices();
		float get_radius();
		int get_edges();
		void clear_verts();
		void add_vertex(Vec v);
		Vec* get_vertex(int index);
		Vec* prev_vertex(int index);	
		Vec* next_vertex(int index);	
		Vec verts[MAX_V];	
		~Polygon();
		void print();
		void init();
		void render(Display* d, Vec position, float orientation, char color[3], int option);
		void generate_polygon();

	private:
		int edges = 0;
		int vertices = 0;
		float radius = 50;

};

#endif
