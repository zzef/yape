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
		std::vector<Body*> bodies;
		Boundary b;
		bool subdivided=false;
		int capacity=3;
		
		std::unique_ptr<QuadTree> TL;
		std::unique_ptr<QuadTree> TR;
		std::unique_ptr<QuadTree> BL;
		std::unique_ptr<QuadTree> BR;

		bool intersect_sqr(float x, float y, float size);

	public:
		QuadTree(Boundary b, int capacity);
		QuadTree();
		void subdivide(Boundary b, std::vector<Boundary>& boundaries);
		void insert(Body* body, std::vector<Boundary>& boundaries);
		void query(Body* b1, std::vector<Body*>& bods);
		

	

};

#endif
