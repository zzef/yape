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
		if(this->Bodies[i]->get_mouse_contact()) {
			//this->Bodies[i]->set_color(RED);
		}
		this->Bodies[i]->render(d,this->glob_options);
	}
	char color[3] = {
		(char) 255,
		(char) 0,
		(char) 0
	};

	if (show_conts) {
		for (int i = this->contact_points.size()-1; i >= 0; i--) {
			d->draw_circle(this->contact_points[i],7.5,0,color);
			//this->contact_points[i].print();
			this->contact_points.pop_back();
		}
	}

	for (int i = this->edges.size()-1; i >= 0; i--) {
		d->draw_line(edges[i].v1,edges[i].v2,color);
		this->edges.pop_back();
	}

	this->reset_colors();
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

void World::reset_colors() {
	for (int i = 0; i<this->bodies; i++) {
		this->Bodies[i]->reset_color();
	}
}

void World::simulate() {
	
	for (int i = 0; i < this->bodies; i++) {

		std::shared_ptr<Body> b = this->Bodies[i];	
		if (b->is_earthed())
			continue;

		if(this->mouse_down && b->get_mouse_contact()) {
			b->reset();
			continue;
		}
			
		b->set_vel_y(b->get_vel_y()+this->gravity);
		b->set_y(b->get_y()+b->get_vel_y());
		b->set_x(b->get_x()+b->get_vel_x());
		b->set_orientation(b->get_orientation()+b->get_ang_vel());
	}

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

void World::resolve_manifolds() {

	for (int i = contacts.size()-1; i>=0; i--) {
		Vec sep_norm = this->contacts[i].mtv;
		std::shared_ptr<Body> A = this->contacts[i].A;	
		std::shared_ptr<Body> B = this->contacts[i].B;
		
		if (this->show_coll) {
			A->set_color(YELLOW);
			B->set_color(YELLOW);
		}

		Vec position_a(A->get_x(),A->get_y());
		Vec position_b(B->get_x(),B->get_y());
		
		Vec velocity_a(A->get_vel_x(),A->get_vel_y());
		Vec velocity_b(B->get_vel_x(),B->get_vel_y());

		float ang_vel_a = A->get_ang_vel();
		float ang_vel_b = B->get_ang_vel();

		float e = 0.6f;
		Vec mv = this->contacts[i].mtv * this->contacts[i].mtvm;
		Vec mtv = this->contacts[i].mtv;
	
		for (int j = 0; j<this->contacts[i].no_contacts; j++) {

				this->contact_points.push_back(this->contacts[i].contacts[j]);

				Vec ra = this->contacts[i].contacts[j] - position_a;	
				Vec rb = this->contacts[i].contacts[j] - position_b;
				
				Vec rv = velocity_a + ra.cross(ang_vel_a) - (velocity_b + rb.cross(ang_vel_b)); 
	
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
		
				float inv_mass_sum = (float) (A->get_im() +  ((racrossn*racrossn) * A->get_iI())) + (B->get_im() +  ((racrossn*racrossn) * B->get_iI()));

				//std::cout << "inv_mass_sum " << inv_mass_sum << std::endl;
			
				float ji = -(1.0f + e) * contact_vel;
				ji /= inv_mass_sum;
				//std::cout << "ji " << ji << std::endl;
				ji /= this->contacts[i].no_contacts; 
				//std::cout << "ji " << ji << std::endl;
	
				Vec impulse = sep_norm * ji;
				if (A->get_im() > 0)
					A->apply_impulse(impulse,ra);

				Vec nimpulse = sep_norm * ji * -1;
				if (B->get_im() > 0)
					B->apply_impulse(nimpulse, rb);		


				float df = 0.35;
				float mu = 0.45;
				
				velocity_a = Vec(A->get_vel_x(),A->get_vel_y());
				velocity_b = Vec(B->get_vel_x(),B->get_vel_y());
				
				rv = velocity_a + ra.cross(ang_vel_a) - (velocity_b + rb.cross(ang_vel_b)); 
			
				Vec t = rv - (sep_norm * (rv.dot(sep_norm)));
				
				//std::cout << "t ---" << std::endl;
				//rv.print();
				//sep_norm.print();
				//t.print(); 

				t = t.normalize();
				//std::cout << "== norms" << std::endl;
				//t.print();

				float jt = -rv.dot(t);
				jt /= inv_mass_sum;

				if (abs(jt)<0.001)
					break;
				
				Vec fimpulse;
				if (abs(jt) < ji*mu) {
					fimpulse = t * jt;
				}
				else {
					fimpulse = t * (-ji * df);
				}

				if (A->get_im() > 0)
					A->apply_impulse(fimpulse,ra);

				Vec nfimpulse = fimpulse * -1;			
				if (B->get_im() > 0)
					B->apply_impulse(nfimpulse, rb);		

		}


		if (A->get_im() > 0) {
			A->set_x(A->get_x()+(mv.get_x()*0.25f));
			A->set_y(A->get_y()+(mv.get_y()*0.25f));
		}
		
		if (B->get_im() > 0) {
			B->set_x(B->get_x()-(mv.get_x()*0.25f));
			B->set_y(B->get_y()-(mv.get_y()*0.25f));
		}

		contacts.pop_back();

	}

}

void World::generate_manifolds() {
	
	for(int i = 0; i<this->bodies; i++) {
		for(int j = i + 1; j<this->bodies; j++) {
			
			std::shared_ptr<Body> A = this->Bodies[i];
			std::shared_ptr<Body> B = this->Bodies[j];
			
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
		edges.push_back(Edge(vt,best_vertex1));
	
	}
	else {
		reference = s_edge_2;
		incident = s_edge_1;

		Vec vt = best_vertex - sep_vec;
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
		}
		else {
			this->Bodies[i]->mouse_contact(false);
		}
	}
	this->rel_mouse_position = Vec(0,0);
}
