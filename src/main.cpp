#include "../include/includes.h"

#define W_WIDTH 1700
#define W_HEIGHT 900
#define WINDOW_TITLE "Yape Sandbox"


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
int _FPS = 0;
int _UPDATES = 0;

float ori = 0;
int mx, my;
bool interactive = true;
bool physics_interpolation = true;
bool box_mode = false;
bool positional_correction = true;
bool show_connections = true;
bool show_contacts = false;
Display display(W_WIDTH,W_HEIGHT,WINDOW_TITLE);
World world(&display);

void initialize();
void setup_demo();

void add_new_box(Vec position){
	std::shared_ptr<Body> rect1 = std::make_shared<Body>(POLYGON);
	rect1->rect(60,60);
	rect1->set_pos(position);
	rect1->set_orientation(0);
	rect1->generate_color();
	rect1->initialize();
	rect1->generate_color();
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
		case sf::Keyboard::B : {
			box_mode = !box_mode;
			break;
		}
		case sf::Keyboard::X : {
			world.clear_up();
			world.clear_bodies();
			world.clear_constraints();
			initialize();
			break;
		}
		case sf::Keyboard::R : {
			world.clear_up();
			world.clear_bodies();
			world.clear_constraints();
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
	if (ori>360)
		ori = 0;
	display.clear();
	world.render(ratio);
	std::string fps = "FPS   " + std::to_string(_FPS);
	std::string phys = "PHYSICS UPDATES   " + std::to_string(_UPDATES);
	display.draw_text(20,20,fps,13);
	display.draw_text(100,20,phys,13);
	display.show();
}

void test() {

	std::shared_ptr<Body> a = std::make_shared<Body>(POLYGON);
	a->generate_polygon();
	a->set_x(1300);
	a->set_y(300);
	a->initialize();

	std::shared_ptr<Body> a2 = std::make_shared<Body>(POLYGON);
	a2->generate_polygon();
	a2->set_x(200);
	a2->set_y(200);
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
	b2->initialize();

	std::shared_ptr<Body> b3 = std::make_shared<Body>(CIRCLE);
	b3->set_x(700);
	b3->set_y(200);
	b3->set_radius(50);

	//world.add_body(a2);
	a2->set_orientation(random(0,360)*(M_PI/180.0f));	
	//world.add_body(a);
	a->set_orientation(random(0,360)*(M_PI/180.0f));
	
	//world.add_body(b);
	//world.add_body(b2);

	std::shared_ptr<Body> rect1 = std::make_shared<Body>(POLYGON);
	rect1->rect(60,60);
	rect1->set_pos(250,500);
	rect1->initialize();
	rect1->generate_color();
	rect1->set_orientation(random(0,360)*(M_PI/180.0f));

	std::shared_ptr<Body> rect2 = std::make_shared<Body>(POLYGON);
	rect2->rect(60,60);
	rect2->set_pos(350,500);
	rect2->initialize();
	rect2->generate_color();

	std::shared_ptr<Body> rect3 = std::make_shared<Body>(POLYGON);
	rect3->rect(60,60);
	rect3->set_pos(450,500);
	rect3->initialize();
	rect3->generate_color();
	
	std::shared_ptr<Body> rect4 = std::make_shared<Body>(POLYGON);
	rect4->rect(60,60);
	rect4->set_pos(550,500);
	rect4->initialize();
	rect4->generate_color();

	/*
	std::shared_ptr<Body> rect5 = std::make_shared<Body>(POLYGON);
	rect5->rect(150,150);
	rect5->set_pos(1500,300);
	*/
	Distance_constraint distance_constraint(rect1,Vec(0,0),rect2,Vec(0,0),100);	
	Distance_constraint distance_constraint2(rect2,Vec(0,0),rect3,Vec(0,0),100);	
	Distance_constraint distance_constraint3(rect3,Vec(0,0),rect4,Vec(0,0),100);	
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
	world.show_connections(show_connections);
	world.show_normals(false);
	world.show_poly_outlines(false);
	world.positional_correction_(positional_correction);
	int overlap = 10;
	float thickness = 1.5f;	
	int height = thickness;
	int margin = 60;
	int width = W_WIDTH-(margin*2)+overlap;
	std::shared_ptr<Body> floor = std::make_shared<Body>(POLYGON);
	floor->rect(thickness,W_WIDTH-(margin*2)+(overlap*2));
	floor->set_x(W_WIDTH/2);
	floor->set_y(W_HEIGHT-((height/2)+margin));
	floor->initialize();
	floor->set_iI(0);
	floor->set_im(0);


	float wh = W_HEIGHT-(margin*2)+(overlap);

	std::shared_ptr<Body> wall1 = std::make_shared<Body>(POLYGON);
	wall1->rect(wh,thickness);
	wall1->set_pos((thickness/2.0f) + W_WIDTH - margin - thickness, margin + (thickness/2.0f)  + (wh/2));
	wall1->initialize();	
	wall1->set_iI(0);
	wall1->set_im(0);

	std::shared_ptr<Body> wall2 = std::make_shared<Body>(POLYGON);
	wall2->rect(wh,thickness);
	wall2->set_pos((thickness/2.0f) + margin, margin + (thickness/2.0f) + (wh/2));
	wall2->initialize();	
	wall2->set_iI(0);
	wall2->set_im(0);

	world.add_body(wall1);	
	world.add_body(wall2);
	world.add_body(floor);
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
	test();
	//stacking_test();
}

void initialize() {
	add_world_surfaces();
}

int main() {

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
		sf_time += frame_time;
		t_prev = t_now;

		if (t_time > 1){
			_FPS = draws;
			_UPDATES = updates;
			t_time = 0;
			draws = 0;
			updates = 0;
		}

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





