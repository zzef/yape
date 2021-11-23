#include "../include/includes.h"

#define W_WIDTH 1700
#define W_HEIGHT 900
#define WINDOW_TITLE "Engine"

float ori = 0;
int mx, my;
bool interactive = true;
bool physics_interpolation = true;
bool box_mode = false;
bool positional_correction = true;
bool show_contacts = false;
Display display = Display(W_WIDTH,W_HEIGHT,WINDOW_TITLE);
World world;

void initialize();

void add_new_box(Vec position){
	std::shared_ptr<Body> rect1 = std::make_shared<Body>(POLYGON);
	rect1->rect(40,40);
	rect1->set_pos(position);
	rect1->set_orientation(0);
	rect1->generate_color();
	rect1->initialize();
	world.add_body(rect1);
}

void add_new_polygon(Vec position) {
	std::shared_ptr<Body> a = std::make_shared<Body>(POLYGON);
	a->generate_polygon();
	a->set_x(position.get_x());
	a->set_y(position.get_y());
	a->set_orientation(random(0,360)*(M_PI/180.0f));
	a->generate_color();
	a->initialize();
	world.add_body(a);
}

void handle_mouse_motion(SDL_MouseMotionEvent e) {
	world.set_mouse_position(Vec(e.x,e.y));
	switch(e.state) {
		case SDL_BUTTON_LMASK : {
			world.set_rel_mouse_position(Vec(e.xrel,e.yrel));
			break;
		}
		case SDL_BUTTON_RMASK : {
			
		}
		default: {
			world.set_rel_mouse_position(Vec(0,0));
		}
	}		
}

void handle_mouse_up(SDL_MouseButtonEvent e) {
	world.set_mouse_down(false);
}

void handle_mouse_down(SDL_MouseButtonEvent e) {
	world.set_mouse_down(true);

	if (e.button == SDL_BUTTON_LEFT) {
		if (!interactive) {
			if (box_mode)
				add_new_box(Vec(e.x,e.y));
			else
				add_new_polygon(Vec(e.x,e.y));
		}
	}
}

void handle_keydown(SDL_KeyboardEvent e) {
	switch(e.keysym.sym) {
		case SDLK_i : {
			interactive = !interactive;
			break;
		}
		case SDLK_p : {
			physics_interpolation = !physics_interpolation;
			break;
		}
		case SDLK_c : {
			positional_correction = !positional_correction;
			world.positional_correction_(positional_correction);
			break;
		}
		case SDLK_t : {
			show_contacts = !show_contacts;
			world.show_contacts(show_contacts);
			break;
		}
		case SDLK_b : {
			box_mode = !box_mode;
			break;
		}
		case SDLK_x : {
			world.clear_up();
			world.clear_bodies();
			break;
		}
		case SDLK_r : {
			world.clear_bodies();
			world.clear_up();
			initialize();
			break;
		}
		case SDLK_ESCAPE : {
			exit(0);
			break;
		}


	}
}

void handle_event(SDL_Event e) {	
	switch(e.type){
		case SDL_MOUSEMOTION : {
			handle_mouse_motion(e.motion);
			break;
		}
		case SDL_MOUSEBUTTONDOWN : {
			handle_mouse_down(e.button);
			break;
		}
		case SDL_MOUSEBUTTONUP : {
			handle_mouse_up(e.button);
			break;
		}
		case SDL_KEYDOWN : {
			handle_keydown(e.key);
		}
	}
}

void update() {
	world.detect_mouse_insidedness();
	world.simulate();
}

void render(float ratio) {	
	//world.get_body(0)->set_orientation(ori+=0.05);
	if (ori>360)
		ori = 0;
	world.render(&display, ratio);
	display.show();
}

void test() {

	std::shared_ptr<Body> a = std::make_shared<Body>(POLYGON);
	a->generate_polygon();
	a->set_x(1300);
	a->set_y(300);
	a->set_orig_color(
		(char) 70,
		(char) 200,
		(char) 70
	);
	a->initialize();

	std::shared_ptr<Body> a2 = std::make_shared<Body>(POLYGON);
	a2->generate_polygon();
	a2->set_x(200);
	a2->set_y(200);
	a2->set_orig_color(
		(char) 70,
		(char) 200,
		(char) 70
	);
	a2->initialize();


	std::shared_ptr<Body> b = std::make_shared<Body>(POLYGON);
	b->add_vertex(Vec(140,120));
	b->add_vertex(Vec(-120,170));
	b->add_vertex(Vec(-160,140));
	b->add_vertex(Vec(-140,-60));
	b->add_vertex(Vec(50,-80));
	b->add_vertex(Vec(140,120));
	b->set_x(400);
	b->set_y(300);	
	b->set_orig_color(
		(char) 70,
		(char) 200,
		(char) 70
	);
	b->initialize();

	std::shared_ptr<Body> b2 = std::make_shared<Body>(POLYGON);
	b2->add_vertex(Vec(80,60));
	b2->add_vertex(Vec(20,-90));
	b2->add_vertex(Vec(-80,-80));
	b2->add_vertex(Vec(-120,140));
	b2->add_vertex(Vec(0,170));
	b2->add_vertex(Vec(80,60));
	b2->set_x(800);
	b2->set_y(400);
	b2->set_orig_color(
		(char) 180,
		(char) 30,
		(char) 180
	);
	b2->initialize();

	std::shared_ptr<Body> b3 = std::make_shared<Body>(CIRCLE);
	b3->set_x(700);
	b3->set_y(200);
	b3->set_radius(50);
	b3->set_orig_color(
		(char) 80,
		(char) 120,
		(char) 210
	);

	//world.add_body(a2);
	a2->set_orientation(random(0,360)*(M_PI/180.0f));	
	//world.add_body(a);
	a->set_orientation(random(0,360)*(M_PI/180.0f));
	
	//world.add_body(b);
	//b2->set_orientation(random(0,360)*(M_PI/180.0f));
	//world.add_body(b2);

	std::shared_ptr<Body> rect1 = std::make_shared<Body>(POLYGON);
	rect1->rect(100,100);
	rect1->set_pos(600,500);
	rect1->initialize();
	std::shared_ptr<Body> rect2 = std::make_shared<Body>(POLYGON);
	rect2->rect(100,100);
	rect2->set_pos(600,300);
	rect2->initialize();
	rect1->set_orientation(random(0,360)*(M_PI/180.0f));	
	rect2->set_orientation(random(0,360)*(M_PI/180.0f));	

	std::shared_ptr<Body> rect3 = std::make_shared<Body>(POLYGON);
	rect3->rect(100,100);
	rect3->set_pos(1000,300);
	rect3->initialize();
	rect3->set_orientation(random(0,360)*(M_PI/180.0f));	

	
	std::shared_ptr<Body> rect4 = std::make_shared<Body>(POLYGON);
	rect4->rect(100,100);
	rect4->set_pos(900,300);
	rect4->set_orientation(random(0,360)*(M_PI/180.0f));	
	rect4->initialize();
	/*
	std::shared_ptr<Body> rect5 = std::make_shared<Body>(POLYGON);
	rect5->rect(150,150);
	rect5->set_pos(1500,300);
	*/
	Distance_constraint distance_constraint(rect1,Vec(50,50),rect2,Vec(-50,50),0);	
	Distance_constraint distance_constraint2(rect2,Vec(50,50),rect3,Vec(-50,50),0);	
	Distance_constraint distance_constraint3(rect3,Vec(50,50),rect4,Vec(-50,50),0);	
	//Distance_constraint distance_constraint4(rect4,Vec(40,0),rect5,Vec(0,0),300);	

	world.add_distance_constraint(distance_constraint);
	world.add_distance_constraint(distance_constraint2);
	world.add_distance_constraint(distance_constraint3);
	//world.add_distance_constraint(distance_constraint4);
	

	world.add_body(rect1);
	world.add_body(rect2);
	world.add_body(rect3);
	world.add_body(rect4);
	//world.add_body(rect5);



}

void add_world_surfaces() {
	world.show_polymids(false);
	world.show_collisions(false);
	world.show_contacts(show_contacts);
	world.show_connections(true);
	world.show_normals(false);
	world.positional_correction_(positional_correction);
	
	int thickness = 40;	
	int height = thickness;
	int margin = 40;
	int width = W_WIDTH-(margin*2);
	std::shared_ptr<Body> b4 = std::make_shared<Body>(POLYGON);
	b4->add_vertex(Vec(width/2,height/2));
	b4->add_vertex(Vec(-width/2,height/2));
	b4->add_vertex(Vec(-width/2,-height/2));
	b4->add_vertex(Vec(width/2,-height/2));
	b4->add_vertex(Vec(width/2,height/2));
	b4->set_x(W_WIDTH/2);
	b4->set_y(W_HEIGHT-((height/2)+margin));
	b4->initialize();
	b4->set_iI(0);
	b4->set_im(0);


	float wh = 700;

	std::shared_ptr<Body> wall1 = std::make_shared<Body>(POLYGON);
	wall1->rect(wh,thickness);
	wall1->set_pos((thickness/2.0f) + W_WIDTH - margin - thickness,100 + (wh/2));
	wall1->initialize();	
	wall1->set_iI(0);
	wall1->set_im(0);

	std::shared_ptr<Body> wall2 = std::make_shared<Body>(POLYGON);
	wall2->rect(wh,thickness);
	wall2->set_pos((thickness/2.0f) + margin,100 + (wh/2));
	wall2->initialize();	
	wall2->set_iI(0);
	wall2->set_im(0);

	world.add_body(wall1);	
	world.add_body(wall2);
	world.add_body(b4);
}


void stacking_test() {

	int levels = 10;
	Vec orig(850,920 - (levels*(40+40)));
	for (int i = 0; i < levels; i++) {
		Vec start = Vec(-((i*45/2)),(i*45)) + Vec(0,30*i);
		for (int j = 0; j<i+1; j++) {
			Vec pos = (orig + start) + Vec(j*45,0);
			add_new_box(pos);
		}
	}
	
}

void initialize() {


	add_world_surfaces();
	test();
	//stacking_test();

}

float t = 0.0f;
auto t_prev = std::chrono::high_resolution_clock::now();
float accumulator = 0.0f;
float y = 100;
float x = 100;
float speed = 0;
float grav = 60.0f;
int draws = 0;
int updates = 0;
double t_time;
float e_time = 0;
float sf_time = 0;
char color[3] = {(char)255,(char)0,(char)0};

int main() {

	srand (time(NULL));	
	bool quit = false;
	SDL_Event e;
	initialize();

	while ( !quit ) {
	
		auto t_now = std::chrono::high_resolution_clock::now();
		double frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t_now - t_prev).count();
		frame_time *= 1e-9;
		accumulator += frame_time;
		t_time += frame_time;
		sf_time += frame_time;
		t_prev = t_now;

		if (t_time > 1){
			std::cout << "updates " << updates << ", renders " << draws << std::endl;
			t_time = 0;
			draws = 0;
			updates = 0;
		}

		accumulator = std::min(0.1f,accumulator); //preventing spiral of death
		
		while (SDL_PollEvent( &e )) {

			if (e.type == SDL_QUIT)
				quit=true;
			else
				handle_event( e );

		}

		float times = 0;
		if ( accumulator >= dt) {
			//std::cout << "end" << std::endl;
			e_time = 0;
			sf_time = frame_time;	
		}
	
		while ( accumulator >= dt ) {
			update();	
			accumulator -= dt ;
			updates++;
			e_time += dt;
			times++;
		}
	
		
		//std::cout << "percentage = " << ((float) sf_time / (float) e_time) << std::endl;
		float ratio = std::min(1.0f,(float) sf_time / e_time);
		if (!physics_interpolation)
			ratio = 1;
		render(ratio);
		draws++;

	}

	return 0;
}





