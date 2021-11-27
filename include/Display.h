#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/utils.h"
#include "includes.h"

class Display {

	public:
		Display();
		Display(int width, int height, std::string title);
		void initialize();
		void show();
		void clear();

		void draw_text(float x, float y, std::string& text, float size);
		void fill_box(Vec position, float size, Color& color);
		void fill_polygon(std::vector<Vec>& verts, Vec position, float orientation, Color& color, int options);
		void fill_circle(Vec position, float radius, Color& color);
		void fill_circle(Vec position, float radius, Color& color, float line_thickness, Color& line_color);
		void draw_circle(Vec position, float radius, float line_thickness, Color& line_color);
		void draw_line(Vec v1, Vec v2, Color& color,float thickness);
		void draw_box(Vec position, float w, float h, float line_thickness,Color& color);
		bool is_open();
		void close();
		bool poll_event(sf::Event& e);
		~Display();
		int width;	
		int height;

	private:
		sf::Font font;
		std::string title;
		void initialize_shapes();
		sf::RenderWindow* window;

};

#endif
