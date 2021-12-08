#include "../include/QuadTree.h"

QuadTree::QuadTree() {
	this->capacity=5;
	this->b = Boundary(0,0,500,500);
}

QuadTree::QuadTree(Boundary b, int capacity) {
	this->b = b;
	this->capacity = capacity;
}

void QuadTree::subdivide(Boundary b) {

	Boundary b1( b.x, b.y, b.w/2, b.h/2 );
	TL = std::make_unique<QuadTree>( b1, capacity );

	Boundary b2( b.x + (b.w/2), b.y, b.w/2, b.h/2 );
	TR = std::make_unique<QuadTree>( b2, capacity );

	Boundary b3( b.x, b.y + (b.h/2), b.w/2, b.h/2 );
	BL = std::make_unique<QuadTree>( b3, capacity );

	Boundary b4( b.x + (b.w/2), b.y + (b.h/2), b.w/2 , b.h/2);
	BR = std::make_unique<QuadTree>( b4, capacity );
	
	subdivided=true;

}


bool QuadTree::contains(float x, float y, float size) {
	
	return ((x - size > b.x) && (x - size < b.x + b.w) &&
			(x + size > b.x) && (x + size < b.x + b.w) &&
			(y - size > b.y) && (y - size < b.y + b.h) &&
			(y + size > b.y) && (y + size < b.y + b.h) );

}

bool QuadTree::intersect_boundary(Boundary bnd) {
	return !(( bnd.x ) >= (b.x + b.w) ||
			( bnd.w + bnd.x ) <= b.x ||
			( bnd.y ) >= (b.y + b.h) ||
			( bnd.h + bnd.y ) <= b.y);
}

bool QuadTree::intersect_sqr(float x, float y, float size) {
	return !(( x - size ) >= (b.x + b.w) ||
			( x + size ) <= b.x ||
			( y - size ) >= (b.y + b.h) ||
			( y + size ) <= b.y);
}

void QuadTree::get_bounds(std::vector<Boundary>& boundaries) {
	if (subdivided) {
		TL->get_bounds(boundaries);	
		TR->get_bounds(boundaries);	
		BL->get_bounds(boundaries);	
		BR->get_bounds(boundaries);	
	}
	else {
		boundaries.push_back(this->b);
	}
}

void QuadTree::insert(Body* body) { 
	if (!do_insert(body)) {
		bodies.push_back(body);		
	}
}

bool QuadTree::do_insert(Body* body) {
	if (!contains(body->position.x,
		body->position.y,body->_rad())) {
			return false;
	}
	if (bodies.size() < capacity) {
		bodies.push_back(body);
		return true;
	}
	else {
		if (!subdivided) {
			subdivide(b);
		}
		if(TL->do_insert(body)) {return true;}
		if(TR->do_insert(body)) {return true;}
		if(BL->do_insert(body)) {return true;}
		if(BR->do_insert(body)) {return true;}
	}

	bodies.push_back(body);
	return true;
}


void QuadTree::query(Boundary b, Body** bods, size_t& size) {
	if (!intersect_boundary(b)) {
		return;
	}
	for (int i = 0; i<bodies.size(); i++) {
		bods[size] = bodies[i];
		size++;
	}	
	if (subdivided) {
		TL->query(b,bods,size);
		TR->query(b,bods,size);
		BL->query(b,bods,size);
		BR->query(b,bods,size);
	}
}


void QuadTree::query(Body* b1, Body** bods, size_t& size) {
	if (!intersect_sqr(b1->position.x, b1->position.y, b1->_rad())) {
		return;
	}
	for (int i = 0; i<bodies.size(); i++) {
		if (!bodies[i]->visited && bodies[i]!=b1) {
			bods[size] = bodies[i];
			size++;
		}
	}	
	if (subdivided) {
		TL->query(b1,bods,size);
		TR->query(b1,bods,size);
		BL->query(b1,bods,size);
		BR->query(b1,bods,size);
	}
	b1->visited = true;
}


