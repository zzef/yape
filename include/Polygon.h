#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include "includes.h"
#include "Vector.h"

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
		void render(Display* d, Vec position, float orientation);

	private:
		int edges = 0;
		int vertices = 0;
		void generate();

};

#endif
