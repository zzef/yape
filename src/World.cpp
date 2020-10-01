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
	for (int i = this->contact_points.size()-1; i >= 0; i--) {
		d->draw_circle(this->contact_points[i],10,0,color);
		this->contact_points[i].print();
		this->contact_points.pop_back();
	}

	for (int i = this->collision_normals.size()-1; i >= 0; i--) {
		Vec norm = this->collision_normals[i].normalize() * 50;
		d->draw_line(Vec(200,200),norm+Vec(200,200),color);
		this->collision_normals.pop_back();
	}


	for (int i = this->edges.size()-1; i >= 0; i--) {
		d->draw_line(edges[i].first,edges[i].second,color);
		this->edges.pop_back();
	}


	std::cout << "" << std::endl;
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

void World::resolve_manifolds() {

	for (int i = contacts.size()-1; i>=0; i--) {
		collision_normals.push_back(this->contacts[i].mtv);
		std::shared_ptr<Body> A = this->contacts[i].A;	
		std::shared_ptr<Body> B = this->contacts[i].B;
		A->set_color(YELLOW);
		B->set_color(YELLOW);
		//A->set_x(A->get_x()+this->contacts[i].mtv.get_x());
		//A->set_y(A->get_y()+this->contacts[i].mtv.get_y());

		float best_proj = -9999999;
		Vec best_vertex;
		Vec best_vertex1;
		int index = 0;
		for (int j = 0; j < B->get_vertices()-1; j++) {
			Vec v = B->get_vertex(j)->rotate(B->get_orientation()) + Vec(B->get_x(),B->get_y());
			float proj = v.dot(this->contacts[i].mtv);
	
			if (proj > best_proj) {
				best_vertex = v;
				best_proj = proj;
				index = j;
			}
			
		}

		int nexti;
		int previ;
	
		
		if (index+1 >= B->get_vertices()-1)
			nexti = 0;
		else
			nexti = index+1;

		if (index-1 < 0)
			previ = B->get_vertices()-2;
		else 
			previ = index-1;

		Vec prev = B->get_vertex(previ)->rotate(B->get_orientation()) + Vec(B->get_x(),B->get_y()); 
		Vec next = B->get_vertex(nexti)->rotate(B->get_orientation()) + Vec(B->get_x(),B->get_y()); 

		Vec left = (best_vertex - prev).normalize();
		Vec right = (best_vertex - next).normalize();
			
	
		Vec contact1;	
		Vec contact2;	
		if (right.dot(this->contacts[i].mtv) <= left.dot(this->contacts[i].mtv)) {			
			contact_points.push_back(next);
			contact1 = next;
			
		}
		else {
			contact_points.push_back(prev);
			contact1 = prev;
		}

		Vec e1 = best_vertex-contact1;
		contact_points.push_back(best_vertex);

		this->contacts[i].mtv = this->contacts[i].mtv * -1;

		best_proj = -9999999;
		index = 0;
		for (int j = 0; j < A->get_vertices()-1; j++) {
			Vec v = A->get_vertex(j)->rotate(A->get_orientation()) + Vec(A->get_x(),A->get_y());
			float proj = v.dot(this->contacts[i].mtv);
	
			if (proj > best_proj) {
				best_vertex1 = v;
				best_proj = proj;
				index = j;
			}
			
		}

		if (index+1 >= A->get_vertices()-1)
			nexti = 0;
		else
			nexti = index+1;

		if (index-1 < 0)
			previ = A->get_vertices()-2;
		else 
			previ = index-1;

		prev = A->get_vertex(previ)->rotate(A->get_orientation()) + Vec(A->get_x(),A->get_y()); 
		next = A->get_vertex(nexti)->rotate(A->get_orientation()) + Vec(A->get_x(),A->get_y()); 

		left = (best_vertex1 - prev).normalize();
		right = (best_vertex1 - next).normalize();
		
		if (right.dot(this->contacts[i].mtv) <= left.dot(this->contacts[i].mtv)) {			
			contact_points.push_back(next);
			contact2 = next;
			
		}
		else {
			contact_points.push_back(prev);
			contact2 = prev;
		}


		contact_points.push_back(best_vertex1);
		this->contacts.pop_back();


		Vec e2 = best_vertex1-contact2;
	
		if (abs(e1.dot(this->contacts[i].mtv)) <= abs(e2.dot(this->contacts[i].mtv))) {
			edges.push_back(std::make_pair(best_vertex,contact1));
		}
		else {
			edges.push_back(std::make_pair(best_vertex1,contact2));
		}	

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
	return (point - position).mag() <= b->get_radius();
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
	
			if ( overlap > 0)
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
	contacts.push_back(Manifold(a,b,mtv_axis*mt));
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
