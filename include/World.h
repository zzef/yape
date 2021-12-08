#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include "Display.h"
#include <memory>
#include <vector>
#include "Constraints.h"
#include "QuadTree.h"

class World {
	
	private:
		std::vector<Body*> Bodies;
		std::vector<Body> Bodies_Model;
		std::vector<Boundary> bounds;
		std::unique_ptr<Body> Bods[MAX_BODIES];
		std::vector<Manifold> contacts;
		std::vector<Vec> contact_points;
		std::vector<Vec> anchor_points;
		std::vector<Edge> dconstraints;
		std::vector<Distance_constraint> distance_constraints;
		std::vector<std::pair<Vec,float>> circles;
		QuadTree quadtree;
		Body* potentials[2000];
		int bodies = 0;
		int p_size = 0;
		size_t comparisons = 0;
		float gravity = 0.5;
		Vec mouse_position;
		int glob_options = 0;
		Vec rel_mouse_position;
		bool show_coll = true;
		bool show_conts = true;
		bool show_wireframes = true;
		bool show_conns = true;
		bool show_bounds = true;
		bool show_bphase = false;
		Color contact_color = {GREEN};
		Color anchor_color = {GREEN};
		Color dconstraint_color = {GREEN};
		bool positional_correction = true;
		bool mouse_down;
		Display* display;
		void do_broadphase(QuadTree* qt);
		void reset_quadtree();
		bool bounds_intersect(Body* A, Body* B);
		void generate_pp_manifold(Body* a, Body* b);
		bool is_point_inside_polygon(Body* b, Vec point);
		bool is_point_inside_circle(Body* b, Vec point);
		bool point_inside(Body*, Vec point);
		Edge find_support_edge(Body* body, int index, Vec sep_norm);
		int find_support_point(Body* body, Vec direction);
		void generate_contact_points(Manifold& m);
		std::vector<Vec> clip(Edge incident, Vec ref_norm, float min_ref);
		void generate_manifolds();
		void resolve_manifolds();
		void resolve_constraints();
		void resolve_distance_constraint(Body* a, Vec pp_a, Body* b, Vec pp_b, float dist_const);
		void integrate();
		void apply_positional_correction();
		void integrate_velocities();
		void integrate_forces();
		void broadphase();
		void update_bodies();

	public:
		World(Display* display);
		World(float gravity, Display* display);
		void show_broadphase(bool show);
		void positional_correction_(bool val);
		void clear_bodies();
		void clear_constraints();
		std::unique_ptr<Body> get_body(int i);
		int body_count();
		void add_body(Body* b);
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
		void show_wireframe(bool show);
		void show_connections(bool show);
		void show_pbounds(bool show);
		void show_contacts(bool show);
		void add_distance_constraint(Distance_constraint distance_constraint);
		size_t get_comparisons();
		void simulate();
		void clear_up();

};

#endif
