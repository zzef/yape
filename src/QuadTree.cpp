#include "../include/QuadTree.h"

QuadTree::QuadTree() {
	this->capacity=5;
	this->b = Boundary(0,0,500,500);
}

QuadTree::QuadTree(Boundary b, int capacity) {
	this->b = b;
	this->capacity = capacity;
}

void QuadTree::subdivide(Boundary b, std::vector<Boundary>& boundaries) {

	Boundary b1( b.x, b.y, b.w/2, b.h/2 );
	TL = std::make_unique<QuadTree>( b1, capacity );
	boundaries.push_back(b1);

	Boundary b2( b.x + (b.w/2), b.y, b.w/2, b.h/2 );
	TR = std::make_unique<QuadTree>( b2, capacity );
	boundaries.push_back(b2);

	Boundary b3( b.x, b.y + (b.h/2), b.w/2, b.h/2 );
	BL = std::make_unique<QuadTree>( b3, capacity );
	boundaries.push_back(b3);

	Boundary b4( b.x + (b.w/2), b.y + (b.h/2), b.w/2 , b.h/2);
	BR = std::make_unique<QuadTree>( b4, capacity );
	boundaries.push_back(b4);
	
	subdivided=true;

}

bool QuadTree::intersect_sqr(float x, float y, float size) {
	return !(( x - size ) >= (b.x + b.w) ||
		( x + size ) <= b.x ||
		( y - size ) >= (b.y + b.h) ||
		( y + size ) <= b.y);
}

void QuadTree::insert(Body* body, std::vector<Boundary>& boundaries) {
	if (!intersect_sqr(body->position.x,
		body->position.y,body->_rad())) {
		return;
	}

	if (bodies.size() < capacity) {
		bodies.push_back(body);
	}
	else {
		if (!subdivided) {
			subdivide(b,boundaries);
		}

		TL->insert(body,boundaries);
		TR->insert(body,boundaries);
		BL->insert(body,boundaries);
		BR->insert(body,boundaries);
	}
}

void QuadTree::query(Body* b1, std::vector<Body*>& bods) {
	if (!intersect_sqr(b1->position.x, b1->position.y, b1->_rad())) {
		return;
	}

	for (Body* b2 : bodies) {
		if (b2->intersects(b1)) {
			bods.push_back(b2);
		}
	}
	
	if (subdivided) {
		TL->query(b1,bods);
		TR->query(b1,bods);
		BL->query(b1,bods);
		BR->query(b1,bods);
	}
}


