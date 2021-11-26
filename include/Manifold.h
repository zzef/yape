#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <memory>
#include "Body.h"

class Body;

struct Manifold {
	Body* A;
	Body* B;
	Vec mtv;
	float mtvm;
	Vec contacts[2];
	int no_contacts = 0;
	Manifold(Body* A, Body* B, Vec mtv, float mtvm) {
		this->A = A;
		this->B = B;
		this->mtv = mtv;	
		this->mtvm = mtvm;
	}
};

#endif
