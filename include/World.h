#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Display.h"
#include <memory>

class World {
	
	private:
		std::shared_ptr<Body> Bodies[MAX_BODIES];
		int bodies = 0;
		float gravity = 0.5;
	
	public:
		World();
		World(float gravity);
		std::shared_ptr<Body> get_body(int i);
		int body_count();
		void add_body(std::shared_ptr<Body> b);
		void remove_body(int i);
		void remove_body(Body b);
		void render(Display* d);

};

#endif
