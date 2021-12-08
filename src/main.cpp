#include "../include/includes.h"

#define W_WIDTH 1600
#define W_HEIGHT 900
#define WINDOW_TITLE "Yape Sandbox"


float t = 0.0f;
auto t_prev = std::chrono::high_resolution_clock::now();
float accumulator = 0.0f;
float y = 100;
float x = 100;
float speed = 0;
float grav = 60.0f;
int ave_fps = 0;
int fps_total = 0;
int draws = 0;
int updates = 0;
double t_time = 0;
double t_time2 = 0;
float e_time = 0;
float sf_time = 0;
char color[3] = {(char)255,(char)0,(char)0};
int _FPS = 0;
int _UPDATES = 0;
int seconds_elapsed = 0;

bool cap_frames = true;
float ori = 0;
int mx, my;
bool interactive = true;
bool physics_interpolation = true;
bool box_mode = false;
bool positional_correction = true;
bool show_connections = true;
bool show_broadphase = false;
bool show_contacts = false;
bool show_bounds = false;
bool show_wireframes = false;
Display display(W_WIDTH,W_HEIGHT,WINDOW_TITLE);
World world(&display);

std::vector<std::unique_ptr<Body>> Bodies;

void initialize();
void setup_demo();

void add_new_box(Vec position){
	std::unique_ptr<Body> rect1 = std::make_unique<Body>(POLYGON);
	rect1->rect(60,60);
	rect1->position = Vec(position);
	rect1->orientation = 0;
	rect1->generate_color();
	rect1->initialize();
	rect1->generate_color();
	world.add_body(rect1.get());
	Bodies.push_back(std::move(rect1));
}

void add_new_polygon(Vec position) {
	std::unique_ptr<Body> a = std::make_unique<Body>(POLYGON);
	a->generate_polygon();
	a->position = position;
	a->orientation = random(0,360)*(M_PI/180.0f);
	a->generate_color();
	a->initialize();
	world.add_body(a.get());
	Bodies.push_back(std::move(a));
}
/*
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
*/
void handle_mouse_up(sf::Event::MouseButtonEvent e) {
	world.set_mouse_down(false);
}

void handle_mouse_down(sf::Event::MouseButtonEvent e) {
	world.set_mouse_down(true);

	if (e.button == sf::Mouse::Button::Left) {
		if (interactive) {
			if (box_mode)
				add_new_box(Vec(e.x,e.y));
			else
				add_new_polygon(Vec(e.x,e.y));
		}
	}
}

void handle_keydown(sf::Keyboard::Key key) {
	switch(key) {
		case sf::Keyboard::I : {
			interactive = !interactive;
			break;
		}
		case sf::Keyboard::P : {
			physics_interpolation = !physics_interpolation;
			break;
		}
		case sf::Keyboard::C : {
			positional_correction = !positional_correction;
			world.positional_correction_(positional_correction);
			break;
		}
		case sf::Keyboard::A : {
			show_connections = !show_connections;
			world.show_connections(show_connections);
			break;
		}
		case sf::Keyboard::T : {
			show_contacts = !show_contacts;
			world.show_contacts(show_contacts);
			break;
		}
		case sf::Keyboard::O : {
			show_bounds = !show_bounds;
			world.show_pbounds(show_bounds);
			break;
		}
		case sf::Keyboard::B : {
			box_mode = !box_mode;
			break;
		}
		case sf::Keyboard::M : {
			show_broadphase = !show_broadphase;
			world.show_broadphase(show_broadphase);
			break;
		}
		case sf::Keyboard::W : {
			show_wireframes = !show_wireframes;
			world.show_wireframe(show_wireframes);
			break;
		}
		case sf::Keyboard::X : {
			world.clear_up();
			world.clear_bodies();
			world.clear_constraints();
			Bodies.clear();
			initialize();
			break;
		}
		case sf::Keyboard::R : {
			world.clear_up();
			world.clear_bodies();
			world.clear_constraints();
			Bodies.clear();
			initialize();
			setup_demo();
			break;
		}
		case sf::Keyboard::Escape : {
			exit(0);
			break;
		}


	}
}

void handle_event(sf::Event e) {	
	switch(e.type){
		case sf::Event::MouseMoved : {
			//handle_mouse_motion(e.mouseMove);
			break;
		}
		case sf::Event::MouseButtonReleased : {
			handle_mouse_down(e.mouseButton);
			break;
		}
		case sf::Event::MouseButtonPressed : {
			handle_mouse_up(e.mouseButton);
			break;
		}
		case sf::Event::KeyPressed : {
			handle_keydown(e.key.code);
		}
	}
}

void update() {
	world.detect_mouse_insidedness();
	world.simulate();
}

void render(float ratio) {	
	//world.get_body(0)->set_orientation(ori+=0.05);
	display.clear();
	world.render(ratio);
	std::string fps = "FPS  " + std::to_string(_FPS);
	std::string ave = "AVE FPS  " + std::to_string(ave_fps);
	std::string phys = "PHYSICS UPDATES  " + std::to_string(_UPDATES);
	std::string bodies = "BODIES  " + std::to_string(world.body_count());
	std::string tests = "COLLISION TESTS  " + std::to_string(world.get_comparisons());
	display.draw_text(20,20,fps,13);
	display.draw_text(95,20,ave,13);
	display.draw_text(195,20,phys,13);
	display.draw_text(365,20,bodies,13);
	display.draw_text(465,20,tests,13);
	display.show();
}

void stress_test(int num) {
	for (int i = 0; i < num; i++)
		add_new_polygon(Vec(random(50,W_WIDTH-50),random(50,W_HEIGHT-50)));	
}

void joints_test() {

	float size = 80;
	float dist = 150;

	std::unique_ptr<Body> rect1 = std::make_unique<Body>(POLYGON);
	rect1->rect(size,size);
	rect1->position = Vec(W_WIDTH/2-200,200);
	rect1->initialize();
	rect1->generate_color();
	rect1->orientation = (random(0,360)*(M_PI/180.0f));

	std::unique_ptr<Body> rect2 = std::make_unique<Body>(POLYGON);
	rect2->rect(size,size);
	rect2->position = Vec(W_WIDTH/2-100,200);
	rect2->initialize();
	rect2->generate_color();
	rect2->orientation = (random(0,360)*(M_PI/180.0f));

	std::unique_ptr<Body> rect3 = std::make_unique<Body>(POLYGON);
	rect3->rect(size,size);
	rect3->position = Vec(W_WIDTH/2,100);
	rect3->initialize();
	rect3->generate_color();
	rect3->orientation = (random(0,360)*(M_PI/180.0f));
	
	std::unique_ptr<Body> rect4 = std::make_unique<Body>(POLYGON);
	rect4->rect(size,size);
	rect4->position = Vec(W_WIDTH/2+100,200);
	rect4->initialize();
	rect4->generate_color();
	rect4->orientation = (random(0,360)*(M_PI/180.0f));

	std::unique_ptr<Body> rect5 = std::make_unique<Body>(POLYGON);
	rect5->rect(size,size);
	rect5->position = Vec(W_WIDTH/2+200,150);
	rect5->initialize();
	rect5->generate_color();
	rect5->orientation = (random(0,360)*(M_PI/180.0f));

	Distance_constraint distance_constraint(rect1.get(),Vec(0,0),rect2.get(),Vec(0,0),dist);	
	Distance_constraint distance_constraint2(rect2.get(),Vec(0,0),rect3.get(),Vec(0,0),dist);	
	Distance_constraint distance_constraint3(rect3.get(),Vec(0,0),rect4.get(),Vec(-size/2.0f,0),dist);	
	Distance_constraint distance_constraint4(rect4.get(),Vec(size/2.0f,0),rect5.get(),Vec(0,0),dist);	

	world.add_distance_constraint(distance_constraint);
	world.add_distance_constraint(distance_constraint2);
	world.add_distance_constraint(distance_constraint3);
	world.add_distance_constraint(distance_constraint4);
	
	world.add_body(rect1.get());
	Bodies.push_back(std::move(rect1));

	world.add_body(rect2.get());
	Bodies.push_back(std::move(rect2));

	world.add_body(rect3.get());
	Bodies.push_back(std::move(rect3));

	world.add_body(rect4.get());
	Bodies.push_back(std::move(rect4));

	world.add_body(rect5.get());
	Bodies.push_back(std::move(rect5));

}

void add_world_surfaces() {
	int overlap = 10;
	float thickness = 1.5f;	
	int height = thickness;
	int margin = 60;
	int width = W_WIDTH-(margin*2)+overlap;
	std::unique_ptr<Body> floor = std::make_unique<Body>(POLYGON);
	floor->rect(thickness,W_WIDTH-(margin*2)+(overlap*2));
	floor->position = Vec(W_WIDTH/2,W_HEIGHT-((height/2)+margin));
	floor->initialize();
	floor->iI = 0;
	floor->im = 0;

	float wh = W_HEIGHT-(margin*2)+(overlap);

	std::unique_ptr<Body> wall1 = std::make_unique<Body>(POLYGON);
	wall1->rect(wh,thickness);
	wall1->position = Vec((thickness/2.0f) + W_WIDTH - margin - thickness, margin + (thickness/2.0f)  + (wh/2));
	wall1->initialize();	
	wall1->iI = 0;
	wall1->im = 0;

	std::unique_ptr<Body> wall2 = std::make_unique<Body>(POLYGON);
	wall2->rect(wh,thickness);
	wall2->position = Vec((thickness/2.0f) + margin, margin + (thickness/2.0f) + (wh/2));
	wall2->initialize();	
	wall2->iI = 0;
	wall2->im = 0;

	world.add_body(wall1.get());	
	Bodies.push_back(std::move(wall1));	
	world.add_body(wall2.get());
	Bodies.push_back(std::move(wall2));
	world.add_body(floor.get());
	Bodies.push_back(std::move(floor));
}


void stacking_test() {

	int levels = 8;
	Vec orig(850,900- (levels*(60+60)));
	for (int i = 0; i < levels; i++) {
		Vec start = Vec(-((i*65/2)),(i*65)) + Vec(0,30*i);
		for (int j = 0; j<i+1; j++) {
			Vec pos = (orig + start) + Vec(j*65,0);
			add_new_box(pos);
		}
	}
	
}

void setup_demo() {
	joints_test();
	//stress_test(200);
	//stacking_test();
}

void initialize() {
	add_world_surfaces();
}

int main() {

	world.show_polymids(false);
	world.show_broadphase(false);
	world.show_collisions(false);
	world.show_contacts(show_contacts);
	world.show_connections(show_connections);
	world.show_pbounds(false);
	world.show_normals(false);
	world.show_poly_outlines(false);
	world.show_wireframe(show_wireframes);
	world.positional_correction_(positional_correction);


	srand (time(NULL));	
	bool quit = false;
	initialize();
	setup_demo();	
	
	while ( display.is_open() ) {
	
		auto t_now = std::chrono::high_resolution_clock::now();
		double frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t_now - t_prev).count();
		frame_time *= 1e-9;
		accumulator += frame_time;
		t_time += frame_time;
		t_time2 += frame_time;
		sf_time += frame_time;
		t_prev = t_now;
	
		accumulator = std::min(0.1f,accumulator); //preventing spiral of death
		
        // Process events
        sf::Event event;
        while (display.poll_event(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                display.close();
			handle_event(event);
				
        }

		float times = 0;
		float draw_now=false;
		if ( accumulator >= dt) {
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
		
		float ratio = std::min(1.0f,(float) sf_time / e_time);
		if (!physics_interpolation)
			ratio = 1;
	
		if (t_time > 1){
			_FPS = draws;
			fps_total+=_FPS;
			seconds_elapsed++;
			ave_fps = (fps_total/seconds_elapsed);
			_UPDATES = updates;
			t_time = 0;
			draws = 0;
			updates = 0;
		
			if(seconds_elapsed>10) {
				seconds_elapsed=0;
				fps_total=0;
			}
		}

		if (t_time2>=(1.0f/FPS)) { //fps is different to rate of physics
			if (cap_frames) {
				render(ratio);
				draws++;
			}
			t_time2=0;
		}

	}

	return 0;
}





