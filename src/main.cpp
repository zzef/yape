#include "../include/includes.h"

#define W_WIDTH 1280
#define W_HEIGHT 720
#define WINDOW_TITLE "Engine"

float ori = 0;
int mx, my;

Display display = Display(W_WIDTH,W_HEIGHT,WINDOW_TITLE);
World world;

void handle_mouse_motion(SDL_MouseMotionEvent e) {
	world.set_mouse_position(Vec(e.x,e.y));
	switch(e.state) {
		case SDL_BUTTON_LMASK : {
			world.set_mouse_down(true);
			world.set_rel_mouse_position(Vec(e.xrel,e.yrel));
			break;
		}
		case SDL_BUTTON_RMASK : {
			break;
		}
		default: {
			world.set_mouse_down(false);
			world.set_rel_mouse_position(Vec(0,0));
		}
	}		
}

void handle_event(SDL_Event e) {	
	switch(e.type){
		case SDL_MOUSEMOTION : {
			handle_mouse_motion(e.motion);
			break;
		}
	}
}

void update() {
	world.detect_mouse_insidedness();
	world.simulate();
	world.generate_manifolds();
	world.resolve_manifolds();
}

void render() {	
	//world.get_body(0)->set_orientation(ori+=0.05);
	if (ori>360)
		ori = 0;
	world.render(&display);
}

void initialize() {


	world.show_polymids(true);
	world.show_collisions(false);
	world.show_contacts(true);
	world.show_normals(false);
	
	std::shared_ptr<Body> a = std::make_shared<Body>(POLYGON);
	a->generate_polygon();
	a->set_x(900);
	a->set_y(300);
	a->set_orig_color(
		(char) 70,
		(char) 200,
		(char) 70
	);
	a->initialize();

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


	int height = 70;
	int margin = 20;
	int width = W_WIDTH-(margin*2);
	std::shared_ptr<Body> b4 = std::make_shared<Body>(POLYGON,true);
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

	world.add_body(a);
	a->set_orientation(random(0,360)*(M_PI/180.0f));
	world.add_body(b);
	b2->set_orientation(random(0,360)*(M_PI/180.0f));
	world.add_body(b2);
	//world.add_body(b3);
	world.add_body(b4);

}

int main() {
	
	srand (time(NULL));
	bool quit = false;
	SDL_Event e;
	initialize();
	while(!quit) {	
		while (SDL_PollEvent( &e )) {

			if (e.type == SDL_QUIT)
				quit=true;
			else
				handle_event( e );

		}
		update();
		render();
		display.show();
	}

	return 0;
}





