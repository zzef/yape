#include "../include/includes.h"
#include "../include/utils.h"
#include "../include/Body.h"
#include "../include/World.h"
#include "../include/Joints.h"

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
	std::cout << "added" << std::endl;
}

void World::render(Display* d, float ratio) {
	for (int i = 0; i<this->bodies; i++) {
		if(this->Bodies[i]->get_mouse_contact()) {
			//this->Bodies[i]->set_color(RED);
		}
		this->Bodies[i]->render(d,this->glob_options,ratio);
	}
	char color[3] = {
		(char) 255,
		(char) 0,
		(char) 0
	};

	char blue[3] = {
		(char) 120,
		(char) 140,
		(char) 230
	};

	if (show_conns) {
		for (int i = this->connections.size()-1; i >= 0; i--) {
			d->draw_circle(this->connections[i],7.5,0,blue);
		}
	}


	if (show_conts) {
		for (int i = this->contact_points.size()-1; i >= 0; i--) {
			d->draw_circle(this->contact_points[i],5,0,color);
			//this->contact_points[i].print();
		}
	}

	for (int i = this->edges.size()-1; i >= 0; i--) {
		d->draw_line(edges[i].v1,edges[i].v2,color);
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

void World::show_normals(bool show) {
	this->glob_options = show ? this->glob_options | SHOW_NORMALS : this->glob_options & ~SHOW_NORMALS;
}

void World::set_glob_options(int options) {
	this->glob_options = options;
}

void World::show_connections(bool show) {
	this->show_conns=show;
}

void World::reset_colors() {
	for (int i = 0; i<this->bodies; i++) {
		this->Bodies[i]->reset_color();
	}
}

void World::add_joint(Joint joint) {
	this->joints.push_back(joint);
}

void World::keep_distance(std::shared_ptr<Body> a, Vec pp_a, std::shared_ptr<Body> b, Vec pp_b, float dist_const) {

	float damping = 80;
	if (dist_const == 0)
		damping = 130;

	Vec b_pos(b->get_x(),b->get_y());
	Vec a_pos(a->get_x(),a->get_y());

	Vec contact_b = pp_b.rotate(b->get_orientation());
	Vec contact_a = pp_a.rotate(a->get_orientation());

	Vec pivot_b = b_pos + contact_b;
	Vec pivot_a = a_pos + contact_a;

	//std::cout << "contact" << std::endl;
	Vec position(b->get_x(),b->get_y());

	Vec ra = pp_a * -1;
	Vec rb = pp_b * -1;

	//std::cout << "(" << b->get_x() << "," << b->get_y() << ")" << std::endl;

	//std::cout << "first pos ";
	//position.print(); 
	//position = position + contact;
	//std::cout << "piv" << std::endl;
	//pivot.print();
	Vec d = pivot_a - pivot_b;
	//std::cout << "d" << std::endl;
	//d.print();
	float distance = d.mag();
	float vel = distance - dist_const;
	//std::cout << distance << " dist " << std::endl;
	//std::cout << dist_const << " distcon " << std::endl;
	Vec dn = d.normalize();
	Vec dn2 = d.normalize();
	Vec f = dn * vel * damping;
	Vec fn = f * -1;
	Vec velocity_a = Vec(a->get_vel_x(),a->get_vel_y()) + ra.cross(a->get_ang_vel());
	Vec velocity_b = Vec(b->get_vel_x(),b->get_vel_y()) + rb.cross(b->get_ang_vel());	
	Vec vrel = velocity_b - velocity_a;
	Vec vrel2 =  velocity_a - velocity_b;
	//vrel.print();
	float ta = dn.cross(1.0f).dot(vrel2);
	float tb = dn.cross(1.0f).dot(vrel);

	Vec tva = dn.cross(1.0f) * ta;
	Vec tvb = dn.cross(1.0f) * tb;

	float const_vel = dn.dot(vrel);
	float const_vel2 = dn2.dot(vrel2);
	//std::cout << vel << " vel" << std::endl;
	f = f - (dn*const_vel) - (tvb * 0.0025); // - tvb for rotational damping
	fn = fn - (dn*const_vel2) - (tva * 0.0025);
	f = f/(b->get_im() * 200);
	fn = fn/(a->get_im() * 200);
	b->apply_impulse(f,contact_b);
	a->apply_impulse(fn,contact_a);

	if (dist_const == 0) {	
		//a->set_x(a->get_x()-(d.get_x()/2.0f));
		//a->set_y(a->get_y()-(d.get_y()/2.0f));
		//b->set_x(b->get_x()+(d.get_x()/2.0f));
		//b->set_y(b->get_y()+(d.get_y()/2.0f));
	}

	connections.push_back(pivot_a);
	connections.push_back(pivot_b);
	edges.push_back(Edge(pivot_a,pivot_b));	

}

void World::resolve_constraints() {


	for (int i = 0; i < joints.size(); i++ ) {
		//std::cout << "yooo" << std::endl;
		std::shared_ptr<Body> a = joints[i].a;
		std::shared_ptr<Body> b = joints[i].b;
		this->keep_distance( a, joints[i].pp_a, b, joints[i].pp_b, joints[i].d );
		this->keep_distance( b, joints[i].pp_b, a, joints[i].pp_a, joints[i].d );

	} 
	

}

void World::integrate_forces() {
	for (int i = 0; i < this->bodies; i++) {

		std::shared_ptr<Body> b = this->Bodies[i];	
		if (b->get_im()==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}
			
		b->set_vel_y(b->get_vel_y()+(this->gravity * (dt / 2.0f)));
	}

}

void World::integrate_velocities() {
	for (int i = 0; i < this->bodies; i++) {

		std::shared_ptr<Body> b = this->Bodies[i];	
		if (b->get_im()==0)
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}

		b->prev_pos = Vec(b->get_x(),b->get_y());
		b->prev_orientation = b->get_orientation();
		b->set_orientation(b->get_orientation()+(b->get_ang_vel()*dt));
		b->set_x(b->get_x()+(b->get_vel_x()*dt));
		b->set_y(b->get_y()+(b->get_vel_y()*dt));
	}

}
void World::clear_up() {

	this->contact_points.clear();
	this->connections.clear();
	this->edges.clear();

}

void World::simulate() {

		this->reset_colors();
		this->clear_up();
		this->integrate_forces();
		this->generate_manifolds();
		
		for (int i = 0; i < resolution_iterations; i++) {
			this->resolve_manifolds();
			this->resolve_constraints();
		}

		this->integrate_velocities();
		//this->apply_positional_correction();
		this->contacts.clear();
		
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

void World::apply_positional_correction() {
  const float k_slop = 0.05f; // Penetration allowance
  const float percent = 0.65f; // Penetration percentage to correct

	for (int i = contacts.size()-1; i>=0; i--) {
		std::shared_ptr<Body> A = this->contacts[i].A;	
		std::shared_ptr<Body> B = this->contacts[i].B;
		float penetration = this->contacts[i].mtvm;
		Vec normal = this->contacts[i].mtv;
		Vec correction = normal * (std::max( penetration - k_slop, 0.0f ) / (A->get_im() + B->get_im())) * percent;
	
		A->set_x(A->get_x() + (correction.get_x() * A->get_im()));
		A->set_y(A->get_y() + (correction.get_y() * A->get_im()));

		B->set_x(B->get_x() - (correction.get_x() * B->get_im()));
		B->set_y(B->get_y() - (correction.get_y() * B->get_im()));
		
	}
}
void World::positional_correction_(bool val) {
	this->positional_correction = val;
}

void World::resolve_manifolds() {

	for (int i = contacts.size()-1; i>=0; i--) {
	
		std::shared_ptr<Body> A = this->contacts[i].A;	
		std::shared_ptr<Body> B = this->contacts[i].B;
		
		if (this->show_coll) {
			A->set_color(YELLOW);
			B->set_color(YELLOW);
		}

		Vec position_a(A->get_x(),A->get_y());
		Vec position_b(B->get_x(),B->get_y());
			
		float e = 0.5f;
		float mtvm = this->contacts[i].mtvm;
		Vec mv = this->contacts[i].mtv * mtvm;
		Vec mtv = this->contacts[i].mtv;
		float contacts_ = this->contacts[i].no_contacts;	
		Vec sep_norm = mtv;
		//std::cout << "contacts " << contacts_ << std::endl;
			
		float bias = this->positional_correction ? 0.3f : 0.0f;
		float penetration_allowance = 0.15f;
		float totji = 0.0f;

		Vec velocity_a(A->get_vel_x(),A->get_vel_y());
		Vec velocity_b(B->get_vel_x(),B->get_vel_y());		

		float ang_vel_a = A->get_ang_vel();
		float ang_vel_b = B->get_ang_vel();

		for (int j = 0; j<contacts_; j++) {
	
			//std::cout << "contact " << j << std::endl;
			//std::cout << "==========" << std::endl;	
			//velocity_a.print();
			//velocity_b.print();

			if (show_conts) 
				this->contact_points.push_back(this->contacts[i].contacts[j]);

			Vec ra = this->contacts[i].contacts[j] - position_a;	
			Vec rb = this->contacts[i].contacts[j] - position_b;
			//ra.print();
			//rb.print();
				
			Vec rv = velocity_a + ra.cross(ang_vel_a) - (velocity_b + rb.cross(ang_vel_b)); 
			//Vec rv = velocity_a - velocity_b;
	
			if (rv.mag() < this->gravity * dt ) // if collision is weaker than gravity then cause bodies to lose energy fast and come to rest 
				e = 0.0f;


			//std::cout << "---------" << std::endl;	
			//rv.print();		
		
			float contact_vel = rv.dot(sep_norm);	
	
			//std::cout << "contact vel " << contact_vel << std::endl;

			if (contact_vel > 0) {
				//std::cout << "reyurning" << std::endl;
				break;
			}
			//std::cout << "ra " <<std::endl;
			//ra.print();
			//sep_norm.print();
			float racrossn = ra.cross(sep_norm);
			float rbcrossn = rb.cross(sep_norm);
			//std::cout << "racrossn " << racrossn << std::endl;
			//std::cout << "rbcrossn " << rbcrossn << std::endl;

			float inv_mass_sum = (float) (A->get_im() + B->get_im());
			inv_mass_sum += (float)  (racrossn * racrossn * A->get_iI()) +  (rbcrossn * rbcrossn * B->get_iI());
			//inv_mass_sum += (A->get_iI() * (rad - (rna*rna))) +  (B->get_iI() * (rbd - (rnb*rnb))); 
			//float inv_mass_sum = (float) A->get_im() + B->get_im();

			//std::cout << "inv_mass_sum " << inv_mass_sum << std::endl;
		
			float ji = -(1.0f + e) * contact_vel;
			//std::cout << "mtvm " << mtvm << std::endl;
			ji += -bias * (1.0f/dt) * std::min(0.0f, penetration_allowance - mtvm);

			if (contact_vel > 0) {
				Vec impulse = sep_norm * ji;
				A->apply_impulse(impulse,ra);
				Vec nimpulse = impulse * -1;			
				B->apply_impulse(nimpulse, rb);			
				break;
			}

			ji /= inv_mass_sum;
			
			//std::cout << "ji " << ji << std::endl;
			ji /= (float) contacts_; 
			//std::cout << "jii " << ji << std::endl;
	
			//float tempji = totji;
			//totji = std::max(totji+ji,0.0f);
			//ji = totji - tempji;	

			float df = 0.35f;
			float mu = 0.40f;
			
			//std::cout << "==========" << std::endl;	
			//velocity_a.print();
			//velocity_b.print();
		
			//std::cout << "velocities" << std::endl;
			//velocity_a.print();
			//velocity_b.print();
	
			Vec t = rv - (sep_norm * (rv.dot(sep_norm)));
			//std::cout << "t ---" << std::endl;
			//rv.print();
			//sep_norm.print();
			//t.print(); 
			//std::cout << "done--" << std::endl;
			t = t.normalize();
				
			//std::cout << "== norms" << std::endl;
			//t.print();

			float jt = -rv.dot(t);
			jt /= inv_mass_sum;
			jt /= (float) contacts_;

			//if (abs(jt)<1e+6)
			//	break;
			
			//std::cout << "jt " << jt << std::endl;
	
			Vec impulse = sep_norm * ji;
			if (abs(jt) < ji*mu) {
				impulse = impulse + ( t * jt );
			}
			else {
				impulse = impulse + ( t * (-ji * df) );
			}

			//fimpulse.print();	
			A->apply_impulse(impulse,ra);
			Vec nimpulse = impulse * -1;			
			B->apply_impulse(nimpulse, rb);			

		}

	}

}

bool World::is_joined(std::shared_ptr<Body> a, std::shared_ptr<Body> b)  {

	for (int i = 0; i<this->joints.size(); i++) {
		if ((joints[i].a == a && joints[i].b == b) || (joints[i].a == b && joints[i].b == a))
			return true;
	}
	return false;
}

void World::generate_manifolds() {
	
	for(int i = 0; i<this->bodies; i++) {
		for(int j = i + 1; j<this->bodies; j++) {
			
			std::shared_ptr<Body> A = this->Bodies[i];
			std::shared_ptr<Body> B = this->Bodies[j];
			
			if (A->get_type()==POLYGON && B->get_type()==POLYGON) {
				if (this->is_joined(A,B)) {
					continue;
				}
				this->generate_pp_manifold(A,B);
				//std::cout << "checking " << A->get_x() << " against " << B->get_x() << std::endl;
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

int World::find_support_point(std::shared_ptr<Body> body, Vec direction) {
	
	float best_proj = -9999999;
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

Edge World::find_support_edge(std::shared_ptr<Body> body, int index, Vec sep_norm) {
	
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

	std::shared_ptr<Body> A = m.A;
	std::shared_ptr<Body> B = m.B;
	Vec sep_norm = m.mtv;
	Vec sep_vec = sep_norm * m.mtvm;
	
	int index = this->find_support_point(B,sep_norm);
	Vec best_vertex = B->get_vertex(index)->rotate(B->get_orientation()) + Vec(B->get_x(),B->get_y());	
	//contact_points.push_back(best_vertex);
	Edge s_edge_1 = this->find_support_edge(B,index,sep_norm);
	Vec e1(s_edge_1);
			
	sep_norm = sep_norm * -1;
	index = this->find_support_point(A,sep_norm);
	Vec best_vertex1 = A->get_vertex(index)->rotate(A->get_orientation()) + Vec(A->get_x(),A->get_y());
	//contact_points.push_back(best_vertex1);
	Edge s_edge_2 = this->find_support_edge(A,index,sep_norm);
	Vec e2(s_edge_2);
	
	Edge incident;
	Edge reference;

	if (abs(e1.dot(sep_norm)) <= abs(e2.dot(sep_norm))) {
		reference = s_edge_1;
		incident = s_edge_2;

		Vec vt = best_vertex1 + sep_vec;
		if (show_conts)
			edges.push_back(Edge(vt,best_vertex1));
	
	}
	else {
		reference = s_edge_2;
		incident = s_edge_1;

		Vec vt = best_vertex - sep_vec;
		if (show_conts)
			edges.push_back(Edge(vt,best_vertex));
		
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

void World::generate_pp_manifold(std::shared_ptr<Body> a, std::shared_ptr<Body> b) {

	Vec position_a(a->get_x(),a->get_y());
	Vec position_b(b->get_x(),b->get_y());
	std::shared_ptr<Body> ref_poly = a;
	Vec mtv_axis;
	float mt = 999999999;

	for (int s = 0; s < 2; s++) {	
		
		if (s==1)
			ref_poly = b;
			
		Vec position_r(ref_poly->get_x(),ref_poly->get_y());
		
		for (int i = 0; i < ref_poly->get_vertices()-1; i++) {	
			
			Vec v1 = ref_poly->get_vertex(i)->rotate(ref_poly->get_orientation()) + position_r;
			Vec v2 = ref_poly->get_vertex(i+1)->rotate(ref_poly->get_orientation()) + position_r;
			Vec line = v2 - v1;	
			Vec ortho = line.cross(1).normalize();	
			
			float max_a = -999999999;
			float min_a = 999999999;
			float max_b = -999999999;
			float min_b = 999999999;
	
			for (int j = 0; j<a->get_vertices(); j++) {
				Vec v = a->get_vertex(j)->rotate(a->get_orientation()) + position_a;
				float vp = ortho.dot(v);
				if (vp<min_a)	
					min_a=vp;
				if (vp>max_a)
					max_a=vp;
			}
				
			for (int j = 0; j<b->get_vertices(); j++) {
				Vec v = b->get_vertex(j)->rotate(b->get_orientation()) + position_b;
				float vp = ortho.dot(v);
				if (vp<min_b)	
					min_b=vp;
				if (vp>max_b)
					max_b=vp;
			}
		
			float overlap;
			if (min_a < min_b)
				overlap = min_b - max_a;
			else 
				overlap = min_a - max_b;	
	
			if ( overlap >= 0)
				return;
		
			overlap = abs(overlap);
			if (overlap < mt) {
				mt = overlap;
				mtv_axis = ortho;
			
				Vec v = Vec(a->get_x(),a->get_y()) - Vec(b->get_x(),b->get_y());
				if (v.dot(mtv_axis) < 0)
					mtv_axis = mtv_axis * -1;	
		
			}
		}
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
	for (int i = 0; i<this->bodies; i++) {
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
