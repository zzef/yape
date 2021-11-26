#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Body.h"
#include "../include/World.h"
#include "../include/Constraints.h"

World::World(Display* display) {
 	this->gravity = DEF_GRAV;
	this->display = display;
}

World::World(float gravity, Display* display) {
	this->gravity = gravity;
	this->display = display;
}

int World::body_count() {
	return this->Bodies.size();
}

std::shared_ptr<Body> World::get_body(int i) {
	return this->Bodies[i];
}

void World::add_body(std::shared_ptr<Body> b) {
	if (this->Bodies.size()>=MAX_BODIES)
		return;
	Bodies.push_back(b);
	std::cout << "added" << std::endl;
}

void World::render(float ratio) {
	Color white = {WHITE};
	for (int i = 0; i<this->Bodies.size(); i++) {
		if(this->Bodies[i]->get_mouse_contact()) {
		}
		this->Bodies[i]->render(display,this->glob_options,ratio);
	}

	if (show_conts) {
		for (Vec v : contact_points)
			display->fill_box(v,5,contact_color);
	}

	if (show_conns) {
		for (Vec v : anchor_points) {
			display->fill_circle(v,3,anchor_color,1.5f,white);
		}

		for (Edge e : dconstraints)	
			display->draw_line(e.v1,e.v2,dconstraint_color,3);
	}

}

void World::remove_body(Body b) {
}

void World::remove_body(int i) {

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
	this->glob_options = show ? this->glob_options | SHOW_POLY_OUTLINES : this->glob_options & ~SHOW_POLY_OUTLINES;
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

void World::resolve_distance_constraint(std::shared_ptr<Body> a, Vec ra, std::shared_ptr<Body> b, Vec rb, float l) {


	float time = dt / resolution_iterations;
	
	Vec b_pos(b->get_x(),b->get_y());
	Vec a_pos(a->get_x(),a->get_y());

	rb = rb.rotate(b->get_orientation());
	ra = ra.rotate(a->get_orientation());

	Vec pb = b_pos + rb;
	Vec pa = a_pos + ra;
	
	//computing jacobian

	Vec d = pb - pa;
	d=d.normalize();
	Vec j0 = d * -1;
	float j1 = ((ra*-1).cross(d));
	Vec j2 = d;
	float j3 = (rb.cross(d));

	float offset_length = (pb-pa).mag() - l;
	float bias = (1.0f/time) * offset_length * 0.25f;
	float wa = a->get_ang_vel(); 
	float wb = b->get_ang_vel(); 
	Vec va = *(a->get_vel());
	Vec vb = *(b->get_vel());
	
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

	a->set_vel(va + ((j0 * lambda) * a->im));	
	b->set_vel(vb + ((j2 * lambda) * b->im));
	
	//rotational damping about the anchor point
	a->set_ang_vel((wa + ((j1 * lambda) * a->iI)) * rot_damp_constant);	
	b->set_ang_vel((wb + ((j3 * lambda) * b->iI)) * rot_damp_constant);	
	
	
	if (show_conns) {
		anchor_points.push_back(pa);
		anchor_points.push_back(pb);
		dconstraints.push_back(Edge(pa,pb));
	}
	
}

void World::resolve_constraints() {


	for (int i = 0; i < distance_constraints.size(); i++ ) {
		//std::cout << "yooo" << std::endl;
		std::shared_ptr<Body> a = distance_constraints[i].a;
		std::shared_ptr<Body> b = distance_constraints[i].b;
		if (a != NULL && b!=NULL)
			this->resolve_distance_constraint( a, distance_constraints[i].pp_a, b, distance_constraints[i].pp_b, distance_constraints[i].d );

	} 
	

}

void World::integrate_forces() {
	float time = dt / resolution_iterations;
	for (int i = 0; i < this->Bodies.size(); i++) {

		std::shared_ptr<Body> b = this->Bodies[i];	
		if (b->im==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}
			
		b->set_vel_y(b->get_vel_y()+(this->gravity * time));
		
		//global damping
		//b->set_ang_vel(b->get_ang_vel() * 0.99905f);
		//b->set_vel(*b->get_vel() * 0.99905f);
	}

}

void World::integrate_velocities() {
	float time = dt / resolution_iterations;
	for (int i = 0; i < this->Bodies.size(); i++) {

		std::shared_ptr<Body> b = this->Bodies[i];	
		if (b->im==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}

		b->prev_pos = Vec(b->get_x(),b->get_y());
		b->prev_orientation = b->get_orientation();
		b->set_orientation(b->get_orientation()+(b->get_ang_vel()*time));
		b->set_x(b->get_x()+(b->get_vel_x()*time));
		b->set_y(b->get_y()+(b->get_vel_y()*time));
	}

}
void World::clear_up() {
	this->contacts.clear();
	this->contact_points.clear();
	this->anchor_points.clear();
	this->dconstraints.clear();
}

void World::simulate() {

		
		for (int i = 0; i < resolution_iterations; i++) {
			this->clear_up();
			this->integrate_forces();
			this->resolve_constraints();
			this->generate_manifolds();
			this->resolve_manifolds();
			this->integrate_velocities();
			this->contacts.clear();
		}

		//this->apply_positional_correction();
		
}

void World::set_mouse_down(bool val) {
	this->mouse_down = val;
}

void World::show_collisions(bool show) {
	this->show_coll = show;
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
		
		Vec position_a(A->x,A->y);
		Vec position_b(B->x,B->y);
			
		float e = 0.25f;
		float mtvm = this->contacts[i].mtvm;
		Vec mv = this->contacts[i].mtv * mtvm;
		Vec mtv = this->contacts[i].mtv;
		float contacts_ = this->contacts[i].no_contacts;	
		Vec sep_norm = mtv;
			
		float bias = this->positional_correction ? 0.5f : 0.0f;
		float penetration_allowance = 0.05f;
		float totji = 0.0f;

		Vec velocity_a(A->vel_x,A->vel_y);
		Vec velocity_b(B->vel_x,B->vel_y);		

		float ang_vel_a = A->ang_vel;
		float ang_vel_b = B->ang_vel;

		for (int j = 0; j<contacts_; j++) {

			if (show_conts)
				contact_points.push_back(this->contacts[i].contacts[j]);

			Vec ra = this->contacts[i].contacts[j] - position_a;	
			Vec rb = this->contacts[i].contacts[j] - position_b;
				
			Vec rv = velocity_a + ra.cross(ang_vel_a) - (velocity_b + rb.cross(ang_vel_b)); 
	
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

bool World::is_joined(std::shared_ptr<Body> a, std::shared_ptr<Body> b)  {

	for (int i = 0; i<this->distance_constraints.size(); i++) {
		if ((distance_constraints[i].a == a && distance_constraints[i].b == b) || (distance_constraints[i].a == b && distance_constraints[i].b == a))
			return true;
	}
	return false;
}

void World::generate_manifolds() {
	
	for(int i = 0; i<this->Bodies.size(); i++) {
		for(int j = i + 1; j<this->Bodies.size(); j++) {	
				
			Body* A = this->Bodies[i].get();
			Body* B = this->Bodies[j].get();

			if (A->get_type()==POLYGON && B->get_type()==POLYGON) {
				this->generate_pp_manifold(A,B);
			}

		}	
	}
}

bool World::is_point_inside_circle(std::shared_ptr<Body> b, Vec point) {
	Vec position(b->get_x(),b->get_y());
	return (point - position).mag() <= b->Circle::get_radius();
}


bool World::is_point_inside_polygon(std::shared_ptr<Body> b, Vec point) {

	Vec position(b->get_x(),b->get_y());
	for (int i = 0; i < b->get_vertices()-1; i++) {	
	
		Vec v1 = b->get_vertex(i)->rotate(b->get_orientation()) + position;
		Vec v2 = b->get_vertex(i+1)->rotate(b->get_orientation()) + position;
		Vec line = v2 - v1;	
		Vec ortho = line.cross(1).normalize();
		
		float max = ortho.dot(v1);
		float min = ortho.dot(v1);

		float pp = ortho.dot(point);

		for (int j = 0; j<b->get_vertices(); j++) {
			Vec v = b->get_vertex(j)->rotate(b->get_orientation()) + position;
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
		Vec v = body->get_vertex(j)->rotate(body->get_orientation()) + Vec(body->get_x(),body->get_y());
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
	Vec best_vertex = body->get_vertex(index)->rotate(body->get_orientation()) + Vec(body->get_x(),body->get_y());	
	Vec prev = body->prev_vertex(index)->rotate(body->get_orientation()) + Vec(body->get_x(),body->get_y()); 
	Vec next = body->next_vertex(index)->rotate(body->get_orientation()) + Vec(body->get_x(),body->get_y()); 
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
	Vec best_vertex = m.B->get_vertex(index)->rotate(m.B->orientation) + Vec(m.B->x,m.B->y);	
	//contact_points.push_back(best_vertex);
	Edge s_edge_1 = this->find_support_edge(m.B,index,sep_norm);
	Vec e1(s_edge_1);
			
	sep_norm = sep_norm * -1;
	index = this->find_support_point(m.A,sep_norm);
	Vec best_vertex1 = m.A->get_vertex(index)->rotate(m.A->orientation) + Vec(m.A->x,m.A->y);
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
	Vec position_a(a->x,a->y);
	Vec position_b(b->x,b->y);
	Body* ref_poly = a;
	Vec mtv_axis;
	float mt = MAX_INT;

	for (int s = 0; s < 2; s++) {	
	
		Vec position_r(ref_poly->x,ref_poly->y);
		
		for (int i = 0; i < ref_poly->vertices-1; i++) {	
			
			Vec v1 = ref_poly->get_vertex(i)->rotate(ref_poly->orientation) + position_r;
			Vec v2 = ref_poly->get_vertex(i+1)->rotate(ref_poly->orientation) + position_r;
			Vec line = v2 - v1;	
			Vec ortho = line.cross(1).normalize();	
			
			float max_a = -MAX_INT;
			float min_a = MAX_INT;
			float max_b = -MAX_INT;
			float min_b = MAX_INT;
	
			for (int j = 0; j<a->vertices; j++) {
				Vec v = a->get_vertex(j)->rotate(a->orientation) + position_a;
				float vp = ortho.dot(v);
				if (vp<min_a)	
					min_a=vp;
				if (vp>max_a)
					max_a=vp;
			}
				
			for (int j = 0; j<b->vertices; j++) {
				Vec v = b->get_vertex(j)->rotate(b->orientation) + position_b;
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
			
				Vec v = position_a - position_b;
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

bool World::point_inside(std::shared_ptr<Body> b, Vec point) {
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
			this->Bodies[i]->set_x(this->Bodies[i]->get_x()+this->rel_mouse_position.get_x());
			this->Bodies[i]->set_y(this->Bodies[i]->get_y()+this->rel_mouse_position.get_y());
			this->Bodies[i]->prev_pos = Vec(this->Bodies[i]->get_x(),this->Bodies[i]->get_y());
		}
		else {
			this->Bodies[i]->mouse_contact(false);
		}
	}
	this->rel_mouse_position = Vec(0,0);
}
