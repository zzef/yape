#ifndef JOINTS_H
#define JOINTS_H

#include "includes.h"
#include "Body.h"

struct Distance_constraint {
	std::shared_ptr<Body> a;
	Vec pp_a;
	std::shared_ptr<Body> b;
	Vec pp_b;
	float d;
	Distance_constraint(std::shared_ptr<Body> a, Vec pp_a, std::shared_ptr<Body> b, Vec pp_b, float d) {
		this->a = a;
		this->b = b;
		this->pp_a = pp_a;
		this->pp_b = pp_b;
		this->d = d;
	}
};

#endif
