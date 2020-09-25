#include "../include/includes.h"

#define W_WIDTH 1280
#define W_HEIGHT 720
#define WINDOW_TITLE "Engine"

float ori = 0;

Display display = Display(W_WIDTH,W_HEIGHT,WINDOW_TITLE);
World world;

void handle_mouse_motion(SDL_MouseMotionEvent e) {

	switch(e.state) {
		case SDL_BUTTON_LMASK : {
			break;
		}
		case SDL_BUTTON_RMASK : {
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
	}
}

void update() {

}

void render() {
	
	world.get_body(0)->set_orientation(ori+=0.05);
	world.render(&display);
}

void init() {

	std::shared_ptr<Shape> shape;
	std::shared_ptr<Polygon> p = std::make_shared<Polygon>();
	p->add_vertex(Vec(140,120));
	p->add_vertex(Vec(-120,170));
	p->add_vertex(Vec(-160,140));
	p->add_vertex(Vec(-140,-60));
	p->add_vertex(Vec(50,-80));
	p->add_vertex(Vec(140,120));
	shape = p;
	std::shared_ptr<Body> b = std::make_shared<Body>(shape);
	b->set_x(200);
	b->set_y(300);

	std::shared_ptr<Polygon> p2 = std::make_shared<Polygon>();
	shape = p2;
	p2->add_vertex(Vec(80,60));
	p2->add_vertex(Vec(0,170));
	p2->add_vertex(Vec(-120,140));
	p2->add_vertex(Vec(-80,-80));
	p2->add_vertex(Vec(20,-90));
	p2->add_vertex(Vec(80,60));
	std::shared_ptr<Body> b2 = std::make_shared<Body>(shape);
	b2->set_x(800);
	b2->set_y(400);

	std::shared_ptr<Circle> c = std::make_shared<Circle>(50);
	shape = c;
	std::shared_ptr<Body> b3 = std::make_shared<Body>(shape);
	b3->set_x(700);
	b3->set_y(200);

	std::shared_ptr<Polygon> p4 = std::make_shared<Polygon>();
	shape = p4;
	int height = 30;
	int margin = 20;
	int width = W_WIDTH-(margin*2);
	
	p4->add_vertex(Vec(width/2,height/2));
	p4->add_vertex(Vec(-width/2,height/2));
	p4->add_vertex(Vec(-width/2,-height/2));
	p4->add_vertex(Vec(width/2,-height/2));
	p4->add_vertex(Vec(width/2,height/2));

	std::shared_ptr<Body> b4 = std::make_shared<Body>(shape);
	b4->set_x(W_WIDTH/2);
	b4->set_y(W_HEIGHT-((height/2)+margin));

	world.add_body(b);
	world.add_body(b2);
	world.add_body(b3);
	world.add_body(b4);

}

int main() {
	
	bool quit = false;
	SDL_Event e;
	init();
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





