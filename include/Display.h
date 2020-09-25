#ifndef DISPLAY_H
#define DISPLAY_H

#include "includes.h"

class Display {

	public:
		Display();
		Display(int width, int height, std::string title);
		void initialize();
		void show();
		void draw_circle(Vec position, float radius, float orientation);
		void draw_line(float x, float y, float x1, float y1);
		void draw_line(Vec v1, Vec v2);
		void render_polygon(Vec position, Vec* vertices, int verts, float orientation);
		~Display();

	private:
		int width;	
		int height;
		std::string title;
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		void circle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
		void close();

};

#endif
