#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Body.h"
#include "../include/World.h"

World::World() {
 	this->gravity = DEF_GRAV;
}

World::World(float gravity) {
	this->gravity = gravity;
}

int World::body_count() {
	return this->bodies;
}

std::shared_ptr<Body> World::get_body(int i) {
	return this->Bodies[i];
}

void World::add_body(std::shared_ptr<Body> b) {
	if (this->bodies>=MAX_BODIES)
		return;
	this->Bodies[this->bodies]=b;
	this->bodies++;
}

void World::render(Display* d) {
	for (int i = 0; i<this->bodies; i++) {
		this->Bodies[i]->render(d);
	}
}

void World::remove_body(Body b) {
}

void World::remove_body(int i) {

}
