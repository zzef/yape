#include "../include/includes.h"
#include "../include/Body.h"

#ifndef QUAD_TREE
#define QUAD_TREE

struct Boundary {
	float x;
	float y;
	float w;
	float h;
	Boundary() {
	}
	Boundary(float x, float y, float w, float h) {
		this->x=x;
		this->y=y;
		this->w=w;
		this->h=h;
	}

};


class QuadTree {
	
	private:
		Boundary b;
		int capacity=3;
		bool contains(float x, float y, float size);
		bool intersect_sqr(float x, float y, float size);
		bool intersect_boundary(Boundary b);
		bool do_insert(Body* body);

	public:
		std::unique_ptr<QuadTree> TL;
		std::unique_ptr<QuadTree> TR;
		std::unique_ptr<QuadTree> BL;
		std::unique_ptr<QuadTree> BR;

		std::vector<Body*> bodies;
		bool subdivided=false;
		QuadTree(Boundary b, int capacity);
		QuadTree();
		void subdivide(Boundary b);
		void insert(Body* body);
		void get_bounds(std::vector<Boundary>& boundaries);
		void query(Body* b1, Body** bods, size_t& size);
		void query(Boundary b, Body** bods, size_t& size);
		

	

};

#endif
