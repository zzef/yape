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
		inline Vec* get_vertex(int index);
		void set_vertex(int index, Vec v);
		Vec* prev_vertex(int index);	
		Vec* next_vertex(int index);	
		std::vector<Vec> verts;	
		~Polygon();
		void print();
		void init();
		void render(Display* d, Vec position, float orientation, Color& color, int option);
		void generate_polygon();
		int vertices = 0;
		int edges = 0;
		float _rad();
		float rad = 0.0f;

	private:
		float radius = 200;

};

#endif
