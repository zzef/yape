#ifndef DISPLAY_H
#define DISPLAY_H

#include "includes.h"

class Display {

	public:
		Display();
		Display(int width, int height, std::string title);
		void initialize();
		void show();
		void draw_circle(Vec position, float radius, float orientation, char color[3]);
		void draw_line(float x, float y, float x1, float y1, char color[3]);
		void draw_line(Vec v1, Vec v2, char color[3]);
		void draw_text(std::string&& str, int x, int y, char* color, int size);
		~Display();

	private:
		int width;	
		int height;
		int min_font_size = 5;
		int max_font_size = 50;
		std::string title;
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		std::map<int,TTF_Font*> fonts;
		void circle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
		void close();

};

#endif
