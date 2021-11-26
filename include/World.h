#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Display.h"
#include <memory>
#include <vector>
#include "Constraints.h"

class World {
	
	private:
		std::vector<std::shared_ptr<Body>> Bodies;
		std::shared_ptr<Body> Bods[MAX_BODIES];
		std::vector<Manifold> contacts;
		std::vector<Vec> contact_points;
		std::vector<Vec> anchor_points;
		std::vector<Edge> dconstraints;
		std::vector<Distance_constraint> distance_constraints;
		int bodies = 0;
		float gravity = 0.5;
		Vec mouse_position;
		int glob_options = 0;
		Vec rel_mouse_position;
		bool show_coll = true;
		bool show_conts = true;
		bool show_conns = true;
		Color contact_color = {GREEN};
		Color anchor_color = {GREEN};
		Color dconstraint_color = {GREEN};
		bool positional_correction = true;
		bool mouse_down;
		Display* display;
		void generate_pp_manifold(Body* a, Body* b);
		bool is_point_inside_polygon(std::shared_ptr<Body> b, Vec point);
		bool is_point_inside_circle(std::shared_ptr<Body> b, Vec point);
		bool point_inside(std::shared_ptr<Body> b, Vec point);
		Edge find_support_edge(Body* body, int index, Vec sep_norm);
		int find_support_point(Body* body, Vec direction);
		void generate_contact_points(Manifold& m);
		std::vector<Vec> clip(Edge incident, Vec ref_norm, float min_ref);
		void generate_manifolds();
		void resolve_manifolds();
		void resolve_constraints();
		void resolve_distance_constraint(std::shared_ptr<Body> a, Vec pp_a, std::shared_ptr<Body> b, Vec pp_b, float dist_const);
		void integrate();
		bool is_joined(std::shared_ptr<Body> a, std::shared_ptr<Body> b);
		void apply_positional_correction();
		void integrate_velocities();
		void integrate_forces();

	public:
		World(Display* display);
		World(float gravity, Display* display);
		void positional_correction_(bool val);
		void clear_bodies();
		void clear_constraints();
		std::shared_ptr<Body> get_body(int i);
		int body_count();
		void add_body(std::shared_ptr<Body> b);
		void remove_body(int i);
		void remove_body(Body b);
		void render(float ratio);
		void set_mouse_position(Vec v);
		void set_mouse_down(bool val);
		void set_rel_mouse_position(Vec v);
		void set_glob_options(int options);
		void detect_mouse_insidedness();
		void show_poly_outlines(bool show);
		void show_normals(bool show);
		void show_polymids(bool show);
		void show_collisions(bool show);
		void show_connections(bool show);
		void show_contacts(bool show);
		void add_distance_constraint(Distance_constraint distance_constraint);
		void simulate();
		void clear_up();

};

#endif
