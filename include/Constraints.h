#ifndef JOINTS_H
#define JOINTS_H

#include "includes.h"
#include "Body.h"

struct Distance_constraint {
	Body* a;
	Vec pp_a;
	Body* b;
	Vec pp_b;
	float d;
	Distance_constraint(Body* a, Vec pp_a, Body* b, Vec pp_b, float d) {
		this->a = a;
		this->b = b;
		this->pp_a = pp_a;
		this->pp_b = pp_b;
		this->d = d;
	}
};

#endif
