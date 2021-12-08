#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Body.h"
#include "../include/World.h"
#include "../include/Constraints.h"
#include "../include/QuadTree.h"

World::World(Display* display) {
 	this->gravity = DEF_GRAV;
	this->display = display;
	reset_quadtree();
}

World::World(float gravity, Display* display) {
	this->gravity = gravity;
	this->display = display;
	reset_quadtree();
}

void World::reset_quadtree() {
	Boundary b(0,0,display->width,display->height);
	this->quadtree = QuadTree(b,QUAD_TREE_CAPACITY);
}

int World::body_count() {
	return this->Bodies.size();
}

void World::add_body(Body* b) {
	if (this->Bodies.size()>=MAX_BODIES)
		return;
	Bodies.push_back(b);
}

size_t World::get_comparisons() {
	return comparisons;
}

void World::render(float ratio) {
	Color white = {WHITE};
	for (int i = 0; i<this->Bodies.size(); i++) {

		Body* b = Bodies[i];
		if(b->get_mouse_contact()) {
		}
		//printf("ratio %f\n",ratio);
		b->render(display,this->glob_options,ratio);
	}

	if (show_conts) {
		for (Vec v : contact_points)
			display->fill_box(v,5,contact_color);
	}

	if (show_conns) {
		for (Vec v : anchor_points) {
			display->fill_circle(v,2,anchor_color,1.0f,white);
		}

		for (Edge e : dconstraints)	
			display->draw_line(e.v1,e.v2,dconstraint_color,2);
	}

	if (show_bounds) {
		for (int i = 0; i<this->Bodies.size(); i++) {
			Body* b = Bodies[i];
			Vec segment(b->_rad(),b->_rad());
			Vec position = b->position - segment;
			display->draw_box(position,segment.x*2,segment.y*2,0.5f,contact_color);
		}
	}

	if (show_bphase) {
		for (Boundary b : bounds) {
			display->draw_box(Vec(b.x,b.y),b.w,b.h,0.25f,contact_color);
		}
	}


}


void World::set_mouse_position(Vec v) {
	this->mouse_position = v;
}

void World::set_rel_mouse_position(Vec v) {
	this->rel_mouse_position = v;
}

void World::show_polymids(bool show) {
	this->glob_options = show ? this->glob_options | SHOW_POLYMIDS : this->glob_options & ~SHOW_POLYMIDS;
}

void World::show_poly_outlines(bool show) {
}

void World::show_wireframe(bool show) {
	this->show_wireframes=show;
	this->glob_options = show ? this->glob_options | SHOW_WIREFRAME : this->glob_options & ~SHOW_WIREFRAME;
}

void World::show_pbounds(bool show) {
	this->show_bounds=show;
}

void World::show_normals(bool show) {
	this->glob_options = show ? this->glob_options | SHOW_NORMALS : this->glob_options & ~SHOW_NORMALS;
}

void World::set_glob_options(int options) {
	this->glob_options = options;
}

void World::show_connections(bool show) {
	this->show_conns=show;
}

void World::add_distance_constraint(Distance_constraint distance_constraint) {
	this->distance_constraints.push_back(distance_constraint);
}

void World::resolve_distance_constraint(Body* a, Vec ra, Body* b, Vec rb, float l) {


	float time = dt / resolution_iterations;
	
	rb = rb.rotate(b->orientation);
	ra = ra.rotate(a->orientation);

	Vec pb = b->position + rb;
	Vec pa = a->position + ra;
	
	//computing jacobian

	Vec d = pb - pa;
	d=d.normalize();
	Vec j0 = d * -1;
	float j1 = ((ra*-1).cross(d));
	Vec j2 = d;
	float j3 = (rb.cross(d));

	float offset_length = (pb-pa).mag() - l;
	float bias = (1.0f/time) * offset_length * 0.25f;
	float wa = a->w; 
	float wb = b->w; 
	Vec va = a->velocity;
	Vec vb = b->velocity;
	
	//calculate effective mass

	float effective_mass = (j0 * a->im).dot(j0)
							+ (j1 * a->iI * j1)
							+ (j2 * b->im).dot(j2)
							+ (j3 * b->iI * j3);

	//calculate JV + b

	float JVpb = j0.dot(va)
			+ (j1 * wa)
			+ j2.dot(vb)
			+ (j3 * wb)
			+ bias;

	//calculate lambda	

	float lambda = -JVpb / effective_mass;

	//Apply impulses

	a->velocity = va + ((j0 * lambda) * a->im);	
	b->velocity = vb + ((j2 * lambda) * b->im);
	
	//rotational damping about the anchor point
	a->w = (wa + ((j1 * lambda) * a->iI)) * rot_damp_constant;	
	b->w = (wb + ((j3 * lambda) * b->iI)) * rot_damp_constant;	
	
	
	if (show_conns) {
		anchor_points.push_back(pa);
		anchor_points.push_back(pb);
		dconstraints.push_back(Edge(pa,pb));
	}
	
}

void World::resolve_constraints() {


	for (int i = 0; i < distance_constraints.size(); i++ ) {
		Body* a = distance_constraints[i].a;
		Body* b = distance_constraints[i].b;
		if (a != NULL && b!=NULL)
			this->resolve_distance_constraint( a, distance_constraints[i].pp_a, b, distance_constraints[i].pp_b, distance_constraints[i].d );

	} 
	

}

void World::integrate_forces() {
	float time = dt / resolution_iterations;
	for (int i = 0; i < this->Bodies.size(); i++) {

		Body* b = this->Bodies[i];	
		if (b->im==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}
			
		b->velocity.y += this->gravity * time;
	}

}

void World::integrate_velocities() {
	float time = dt / resolution_iterations;
	for (int i = 0; i < this->Bodies.size(); i++) {

		Body* b = this->Bodies[i];	
		if (b->im==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}

		b->orientation += b->w*time;
		b->position = b->position + (b->velocity * time);
	}

}
void World::clear_up() {
	this->contacts.clear();
	this->contact_points.clear();
	this->anchor_points.clear();
	this->dconstraints.clear();
	this->circles.clear();
	bounds.clear();
	reset_quadtree();
	comparisons = 0;
}

void World::simulate() {

		for (int i = 0; i < this->Bodies.size(); i++) {
			Body* b = this->Bodies[i];	
			b->prev_pos = b->position;
			b->prev_orientation = b->orientation;
		}

		for (int i = 0; i < resolution_iterations; i++) {
			this->clear_up();
			this->integrate_forces();
			this->resolve_constraints();
			this->broadphase();
			this->resolve_manifolds();
			this->integrate_velocities();
		}

}

void World::set_mouse_down(bool val) {
	this->mouse_down = val;
}

void World::show_collisions(bool show) {
	//this->show_coll = show;
}

void World::show_broadphase(bool show) {
	show_bphase = show;
}

void World::show_contacts(bool show) {
	this->show_conts = show;
}

void World::positional_correction_(bool val) {
	this->positional_correction = val;
}

void World::resolve_manifolds() {

	float time = dt / resolution_iterations;
	for (int i = contacts.size()-1; i>=0; i--) {
	
		Body* A = this->contacts[i].A;	
		Body* B = this->contacts[i].B;
		
		float e = 0.25f;
		float mtvm = this->contacts[i].mtvm;
		Vec mv = this->contacts[i].mtv * mtvm;
		Vec mtv = this->contacts[i].mtv;
		int contacts_ = this->contacts[i].no_contacts;	
		Vec sep_norm = mtv;
			
		float bias = this->positional_correction ? 0.5f : 0.0f;
		float penetration_allowance = 0.05f;
		float totji = 0.0f;

		float wa = A->w;
		float wb = B->w;
		Vec velocity_a = A->velocity;
		Vec velocity_b = B->velocity;

		for (int j = 0; j<contacts_; j++) {
			if (show_conts)
				contact_points.push_back(this->contacts[i].contacts[j]);

			Vec ra = this->contacts[i].contacts[j] - A->position;	
			Vec rb = this->contacts[i].contacts[j] - B->position;
			
				
			Vec rv = velocity_a + ra.cross(wa) - (velocity_b + rb.cross(wb)); 
	
			if (rv.mag() < this->gravity * time ) // if collision is weaker than gravity then cause bodies to lose energy fast and come to rest 
				e = 0.0f;
		
			float contact_vel = rv.dot(sep_norm);	
	

			if (contact_vel > 0) {
				break;
			}
			float racrossn = ra.cross(sep_norm);
			float rbcrossn = rb.cross(sep_norm);

			float inv_mass_sum = (float) (A->im + B->im);
			inv_mass_sum += (float)  (racrossn * racrossn * A->iI) +  (rbcrossn * rbcrossn * B->iI);
		
			float ji = -(1.0f + e) * contact_vel;
			ji += -bias * (1.0f/time) * std::min(0.0f, penetration_allowance - mtvm);

			if (contact_vel > 0) {
				Vec impulse = sep_norm * ji;
				impulse = impulse / (float) contacts_;
				A->apply_impulse(impulse,ra);
				Vec nimpulse = impulse * -1;			
				B->apply_impulse(nimpulse, rb);			
				break;
			}

			ji /= inv_mass_sum;
			float df = 0.45f;
			float mu = 0.55f;
	
			Vec t = rv - (sep_norm * (rv.dot(sep_norm)));
			t = t.normalize();
			float jt = -rv.dot(t);
			jt /= inv_mass_sum;
	
			Vec impulse = sep_norm * ji;
			if (abs(jt) < ji*mu) {
				impulse = impulse + ( t * jt );
			}
			else {
				impulse = impulse + ( t * (-ji * df) );
			}

			impulse = impulse / (float) contacts_;

			A->apply_impulse(impulse,ra);
			Vec nimpulse = impulse * -1;			
			B->apply_impulse(nimpulse, rb);			

		}

	}

}

bool World::bounds_intersect(Body* A, Body* B) {
	
	float s1 = A->_rad();
	float s2 = B->_rad();

	if ((A->position - B->position)._mag() <= (s1+s2) * (s1+s2))
		return true;
	return false;
}

void World::do_broadphase(QuadTree* qt) {

	int l_size = 0;
	for (Body* A : qt->bodies) {
		for (int i = 0; i<p_size; i++) {
			Body* B = potentials[i];
			if (A->get_type()==POLYGON && B->get_type()==POLYGON && A!=B ) {
				if (A->intersects(B)) {
					comparisons++;
					this->generate_pp_manifold(A,B);
				}
			}
		}
		potentials[p_size] = A;
		p_size++;
		l_size++;
	}

	if (qt->subdivided) {
		do_broadphase(qt->TL.get());
		do_broadphase(qt->TR.get());
		do_broadphase(qt->BL.get());
		do_broadphase(qt->BR.get());
	}

	p_size -= l_size;
	

}
void World::broadphase() {

	for (int i = 0; i<Bodies.size(); i++) {
		quadtree.insert(Bodies[i]);
	}
	quadtree.get_bounds(bounds);	
	do_broadphase(&quadtree);
}

bool World::is_point_inside_circle(Body* b, Vec point) {
	return (point - b->position).mag() <= b->Circle::get_radius();
}

bool World::is_point_inside_polygon(Body* b, Vec point) {

	for (int i = 0; i < b->get_vertices()-1; i++) {	
	
		Vec v1 = b->get_vertex(i)->rotate(b->orientation) + b->position;
		Vec v2 = b->get_vertex(i+1)->rotate(b->orientation) + b->position;
		Vec line = v2 - v1;	
		Vec ortho = line.cross(1).normalize();
		
		float max = ortho.dot(v1);
		float min = ortho.dot(v1);

		float pp = ortho.dot(point);

		for (int j = 0; j<b->get_vertices(); j++) {
			Vec v = b->get_vertex(j)->rotate(b->orientation) + b->position;
			float vp = ortho.dot(v);
			if (vp<min)	
				min=vp;
			if (vp>max)
				max=vp;
		}
		
		if (!(pp>=min && pp<=max))
			return false;
	}

	return true;
}

int World::find_support_point(Body* body, Vec direction) {
	
	float best_proj = -MAX_INT;
	int index = 0;
	for (int j = 0; j < body->get_vertices()-1; j++) {
		Vec v = body->get_vertex(j)->rotate(body->orientation) + body->position;
		float proj = v.dot(direction);
		if (proj > best_proj) {
			best_proj = proj;
			index = j;	
		}		
	}
	return index;

}

Edge World::find_support_edge(Body* body, int index, Vec sep_norm) {
	
	Vec contact;	
	Vec best_vertex = body->get_vertex(index)->rotate(body->orientation) + body->position;	
	Vec prev = body->prev_vertex(index)->rotate(body->orientation) + body->position; 
	Vec next = body->next_vertex(index)->rotate(body->orientation) + body->position; 
	Vec left = (best_vertex - prev).normalize();
	Vec right = (best_vertex - next).normalize();
	
	if (right.dot(sep_norm) <= left.dot(sep_norm)) {			
		//contact_points.push_back(next);
		return Edge(next,best_vertex);
	}
	else {
		//contact_points.push_back(prev);
		return Edge(best_vertex,prev);;
	};

}

std::vector<Vec> World::clip(Edge incident, Vec ref_norm, float min_ref) {

	float p1 = incident.v1.dot(ref_norm) - min_ref;
	float p2 = incident.v2.dot(ref_norm) - min_ref;

	std::vector<Vec> contacts;

	if (p1 >= 0)
		contacts.push_back(incident.v1);
	if (p2 >= 0)
		contacts.push_back(incident.v2);

	if (p1 * p2 < 0) {

		Vec v = incident.v2 - incident.v1;
		float u = (float) p1/(p1 - p2);
		v = v * u;
		v = v + incident.v1;
		contacts.push_back(v);	
	}

	return contacts;
}

void World::generate_contact_points(Manifold& m)  {

	Vec sep_norm = m.mtv;
	Vec sep_vec = sep_norm * m.mtvm;
	
	int index = this->find_support_point(m.B,sep_norm);
	Vec best_vertex = m.B->get_vertex(index)->rotate(m.B->orientation) + m.B->position;	
	//contact_points.push_back(best_vertex);
	Edge s_edge_1 = this->find_support_edge(m.B,index,sep_norm);
	Vec e1(s_edge_1);
			
	sep_norm = sep_norm * -1;
	index = this->find_support_point(m.A,sep_norm);
	Vec best_vertex1 = m.A->get_vertex(index)->rotate(m.A->orientation) + m.A->position;
	//contact_points.push_back(best_vertex1);
	Edge s_edge_2 = this->find_support_edge(m.A,index,sep_norm);
	Vec e2(s_edge_2);
	
	Edge incident;
	Edge reference;

	if (abs(e1.dot(sep_norm)) <= abs(e2.dot(sep_norm))) {
		reference = s_edge_1;
		incident = s_edge_2;

		Vec vt = best_vertex1 + sep_vec;
		//if (show_conts)
		//	display->draw_line(vt,best_vertex,contact_color,1);
	
	}
	else {
		reference = s_edge_2;
		incident = s_edge_1;

		Vec vt = best_vertex - sep_vec;
		//if (show_conts)
		//	display->draw_line(vt,best_vertex,contact_color,1);
		
	}	
	
	//edges.push_back(reference);
	//edges.push_back(incident);
	
	Vec ref_norm = Vec(reference).normalize();

	float min_ref = ref_norm.dot(reference.v1);
	std::vector<Vec> clip_1 = this->clip(incident,ref_norm,min_ref);
	if (clip_1.size()<2)
		return;

	Edge incident_2(clip_1[0],clip_1[1]);
	ref_norm = ref_norm * -1;
	float min_ref2 = ref_norm.dot(reference.v2);
	std::vector<Vec> clip_2 = this->clip(incident_2,ref_norm,min_ref2);
	if (clip_2.size()<2)
		return;


	Vec refnorm = ref_norm.cross(-1);
	float max = refnorm.dot(reference.v1);

	int in = 0;
	for (int i = 0; i < clip_2.size(); i++) {
		if (refnorm.dot(clip_2[i]) - max >= 0) {
			m.contacts[in]=clip_2[i];
			in++;
			m.no_contacts++;
		}
	}

}

void World::clear_bodies() {
	this->Bodies.clear();
}

void World::clear_constraints() {
	this->distance_constraints.clear();
}


void World::generate_pp_manifold(Body* a, Body* b) {
	Body* ref_poly = a;
	Vec mtv_axis;
	float mt = MAX_INT;
	for (int s = 0; s < 2; s++) {	
		
		for (int i = 0; i < ref_poly->vertices-1; i++) {	
			
			Vec v1 = ref_poly->get_vertex(i)->rotate(ref_poly->orientation) + ref_poly->position;
			Vec v2 = ref_poly->get_vertex(i+1)->rotate(ref_poly->orientation) + ref_poly->position;
			Vec line = v2 - v1;	
			Vec ortho = line.cross(1).normalize();	
			
			float max_a = -MAX_INT;
			float min_a = MAX_INT;
			float max_b = -MAX_INT;
			float min_b = MAX_INT;
	
			for (int j = 0; j<a->vertices; j++) {
				Vec v = a->get_vertex(j)->rotate(a->orientation) + a->position;
				float vp = ortho.dot(v);
				if (vp<min_a)	
					min_a=vp;
				if (vp>max_a)
					max_a=vp;
			}
				
			for (int j = 0; j<b->vertices; j++) {
				Vec v = b->get_vertex(j)->rotate(b->orientation) + b->position;
				float vp = ortho.dot(v);
				if (vp<min_b)	
					min_b=vp;
				if (vp>max_b)
					max_b=vp;
			}
		
			float overlap = min_a < min_b ? min_b - max_a : min_a - max_b;
	
			if ( overlap >= 0)
				return;
		
			overlap = abs(overlap);
			if (overlap < mt) {
				mt = overlap;
				mtv_axis = ortho;
			
				Vec v = a->position - b->position;
				if (v.dot(mtv_axis) < 0)
					mtv_axis = mtv_axis * -1;	
		
			}
		}
		ref_poly = b;
	}

	Manifold m(a,b,mtv_axis,mt);
	this->generate_contact_points(m);
	contacts.push_back(m);
	
}

bool World::point_inside(Body* b, Vec point) {
	bool inside;
	if (b->type == POLYGON) {
		inside = this->is_point_inside_polygon(b, point);
	}
	if (b->type == CIRCLE) {
		inside = this->is_point_inside_circle(b, point);
	}
	return inside;
}

void World::detect_mouse_insidedness() {
	for (int i = 0; i<this->Bodies.size(); i++) {
		if(this->point_inside(this->Bodies[i],this->mouse_position)){
			this->Bodies[i]->mouse_contact(true);
			this->Bodies[i]->prev_pos = Bodies[i]->position;
			this->Bodies[i]->position = this->Bodies[i]->position + rel_mouse_position;
		}
		else {
			this->Bodies[i]->mouse_contact(false);
		}
	}
	this->rel_mouse_position = Vec(0,0);
}
