#include "../include/includes.h"

Display::Display() {
	this->initialize();
}

Display::~Display() {
	this->close();
}

void Display::close() {
	window->close();
}

bool Display::is_open() {
	return window->isOpen();
}

bool Display::poll_event(sf::Event& e) {
	return window->pollEvent(e);
}


Display::Display(int width, int height, std::string title) {
	this->width = width;
	this->height = height;
	this->title = title;
	this->initialize();
}
		
void Display::initialize() {
	sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
	window = new sf::RenderWindow(
		sf::VideoMode(this->width, this->height), 
		this->title,
		sf::Style::Default,
		settings);

	char buffer[1024];
	getcwd(buffer,1024);

	std::string abspath(buffer);
	std::string _font = abspath + "/fonts/ZektonRegular.otf";

	bool success = font.loadFromFile(_font);

	if (!success)
		printf("Failed to load font!\n");
}	


void Display::clear() {
	window->clear(sf::Color(DARK_GREY,255));
}

void Display::show() {
	window->display();
}


void Display::fill_polygon(std::vector<Vec>& verts, 
		Vec position, float orientation, 
		Color& color, int options) {

	sf::ConvexShape poly(verts.size()-1);
	for (int i = 0; i < verts.size()-1; i++) {
		poly.setPoint(i,sf::Vector2f(verts[i].get_x(),verts[i].get_y()));	
	}
	if	(options & SHOW_WIREFRAME){
		poly.setOutlineThickness(1);
		poly.setOutlineColor(sf::Color(150,150,150));
		poly.setFillColor(sf::Color(0,0,0,0));
	}
	else {
		poly.setFillColor(sf::Color(color.r,color.g,color.b));
	}

	poly.setPosition(position.get_x(),position.get_y());
	poly.setRotation((180/M_PI)*orientation);
	window->draw(poly);
}

void Display::draw_line(Vec v1, Vec v2, Color& color,float thickness) {

	//stolen from https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
	sf::Vector2f point1(v1.get_x(),v1.get_y());
	sf::Vector2f point2(v2.get_x(),v2.get_y());
	sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

	sf::Vertex vertices[4];

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;

	sf::Color col(color.r,color.g,color.b); 
    for (int i=0; i<4; ++i)
        vertices[i].color = col;

	window->draw(vertices,4,sf::Quads);
	
}

void Display::draw_text(float x, float y, std::string& txt, float size) {
	
	sf::Text text(txt, font);
	text.setCharacterSize(size);
	text.setFillColor(sf::Color(DARK_GREY_3));
	text.setPosition(x,y);
	window->draw(text);

} 

void Display::draw_box(Vec position, float w, float h, float line_thickness,Color& color) {
	sf::RectangleShape box(sf::Vector2f(w,h));
	box.setOutlineThickness(line_thickness);
	box.setOutlineColor(sf::Color(color.r,color.g,color.b));
	box.setFillColor(sf::Color(0,0,0,0));
	box.setPosition(position.x,position.y);
	window->draw(box);	
}

void Display::fill_box(Vec position, float size, Color& color) {
	float trans = size/2.0f;
	sf::RectangleShape box(sf::Vector2f(size,size));
	box.setFillColor(sf::Color(color.r,color.g,color.b));
	box.setPosition(position.get_x()-trans,position.get_y()-trans);
	window->draw(box);	
}

void Display::draw_circle(Vec position, float radius, float line_thickness, Color& line_color) {
	sf::CircleShape circle(radius,15);
	circle.setOutlineColor(sf::Color(line_color.r,line_color.g,line_color.b));
	circle.setOutlineThickness(line_thickness);
	circle.setFillColor(sf::Color(0,0,0,0));
	circle.setPosition(position.get_x()-radius,position.get_y()-radius);
	window->draw(circle);
}

void Display::fill_circle(Vec position, float radius, Color& color, float line_thickness, Color& line_color) {
	sf::CircleShape circle(radius,15);
	circle.setOutlineColor(sf::Color(line_color.r,line_color.g,line_color.b));
	circle.setOutlineThickness(line_thickness);
	circle.setFillColor(sf::Color(color.r,color.g,color.b));
	circle.setPosition(position.get_x()-radius,position.get_y()-radius);
	window->draw(circle);
}	

void Display::fill_circle(Vec position, float radius, Color& color) {
	sf::CircleShape circle(radius,15);
	circle.setFillColor(sf::Color(color.r,color.g,color.b));
	circle.setPosition(position.get_x()-radius,position.get_y()-radius);
	window->draw(circle);
}	

