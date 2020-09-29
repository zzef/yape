#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Display.h"
#include <memory>
#include <vector>

class World {
	
	private:
		std::shared_ptr<Body> Bodies[MAX_BODIES];
		std::vector<Manifold> contacts;
		int bodies = 0;
		float gravity = 0.5;
		Vec mouse_position;
		int glob_options = 0;
		Vec rel_mouse_position;
		void generate_manifold(std::shared_ptr<Body> a, std::shared_ptr<Body> b);
		bool is_point_inside_polygon(std::shared_ptr<Body> b, Vec point);
		bool is_point_inside_circle(std::shared_ptr<Body> b, Vec point);
		bool point_inside(std::shared_ptr<Body> b, Vec point);


	public:
		World();
		World(float gravity);
		std::shared_ptr<Body> get_body(int i);
		int body_count();
		void add_body(std::shared_ptr<Body> b);
		void remove_body(int i);
		void remove_body(Body b);
		void render(Display* d);
		void set_mouse_position(Vec v);
		void set_rel_mouse_position(Vec v);
		void set_glob_options(int options);
		void detect_mouse_insidedness();
		void show_normals(bool show);
		void generate_manifolds();
		void resolve_manifolds();
		void reset_colors();

};

#endif
