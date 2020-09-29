#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <memory>
#include "Body.h"

class Body;

struct Manifold {
	std::shared_ptr<Body> A;
	std::shared_ptr<Body> B;
	Vec mtv;
	Manifold(std::shared_ptr<Body> A, std::shared_ptr<Body> B, Vec mtv) {
		this->A = A;
		this->B = B;
		this->mtv = mtv;	
	}
};

#endif
