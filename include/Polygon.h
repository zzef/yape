#ifndef POLYGON_H
#define POLYGON_H

#include "includes.h"
#include "Shape.h"

class Polygon : public Shape {

	public:
		Polygon();
		Polygon(int vertices);
		int get_vertices();
		int get_edges();
		void clear_verts();
		void add_vertex(Vec v);
		Vec* get_vertex(int index);
		Vec verts[MAX_V];	
		~Polygon();
		void print();
		void init();
		void render(Display* d, Vec position, float orientation, char color[3], int option);

	private:
		int edges = 0;
		int vertices = 0;
		void generate();

};

#endif
