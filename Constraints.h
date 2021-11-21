#ifndef JOINTS_H
#define JOINTS_H

#include "includes.h"
#include "Body.h"

struct Distance_constraint {
	std::shared_ptr<Body> a;
	Vec ra;
	std::shared_ptr<Body> b;
	Vec rb;
	float d;
	Distance_constraint(std::shared_ptr<Body> a, Vec ra, std::shared_ptr<Body> b, Vec rb, float d) {
		this->a = a;
		this->b = b;
		this->ra = ra;
		this->rb = rb;
		this->d = d;
	}
};

#endif
