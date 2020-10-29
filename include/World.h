#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Display.h"
#include <memory>
#include <vector>
#include "Joints.h"

class World {
	
	private:
		std::shared_ptr<Body> Bodies[MAX_BODIES];
		std::shared_ptr<Body> Bods[MAX_BODIES];
		std::vector<Manifold> contacts;
		std::vector<Vec> contact_points;
		std::vector<Vec> connections;
		std::vector<Vec> collision_normals;
		std::vector<Edge> edges;
		std::vector<Joint> joints;
		int bodies = 0;
		float gravity = 0.5;
		Vec mouse_position;
		int glob_options = 0;
		Vec rel_mouse_position;
		bool show_coll = true;
		bool show_conts = true;
		bool show_conns = true;
		bool positional_correction = true;
		bool mouse_down;
		void generate_pp_manifold(std::shared_ptr<Body> a, std::shared_ptr<Body> b);
		bool is_point_inside_polygon(std::shared_ptr<Body> b, Vec point);
		bool is_point_inside_circle(std::shared_ptr<Body> b, Vec point);
		bool point_inside(std::shared_ptr<Body> b, Vec point);
		Edge find_support_edge(std::shared_ptr<Body> body, int index, Vec sep_norm);
		int find_support_point(std::shared_ptr<Body> body, Vec direction);
		void generate_contact_points(Manifold& m);
		std::vector<Vec> clip(Edge incident, Vec ref_norm, float min_ref);
		void generate_manifolds();
		void resolve_manifolds();
		void resolve_constraints();
		void keep_distance(std::shared_ptr<Body> a, Vec pp_a, std::shared_ptr<Body> b, Vec pp_b, float dist_const);
		void integrate();
		bool is_joined(std::shared_ptr<Body> a, std::shared_ptr<Body> b);
		void apply_positional_correction();
		void integrate_velocities();
		void integrate_forces();
		void clear_up();

	public:
		World();
		World(float gravity);
		void positional_correction_(bool val);
		void clear_bodies();
		std::shared_ptr<Body> get_body(int i);
		int body_count();
		void add_body(std::shared_ptr<Body> b);
		void remove_body(int i);
		void remove_body(Body b);
		void render(Display* d, float ratio);
		void set_mouse_position(Vec v);
		void set_mouse_down(bool val);
		void set_rel_mouse_position(Vec v);
		void set_glob_options(int options);
		void detect_mouse_insidedness();
		void show_normals(bool show);
		void show_polymids(bool show);
		void show_collisions(bool show);
		void show_connections(bool show);
		void show_contacts(bool show);
		void reset_colors();
		void add_joint(Joint joint);
		void simulate();

};

#endif
