#include "../include/includes.h"
#include "SDL_ttf.h"

Display::Display() {
	this->initialize();
}

Display::~Display() {
	this->close();
}

void Display::close() {

	if (this->renderer) {
		SDL_DestroyRenderer(this->renderer);
		this->renderer=NULL;
	}

	if (this->window) {
		SDL_DestroyWindow(this->window);
		this->window=NULL;
	}
	
	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_Quit();
	}

	std::cout<<"remove"<<std::endl;

}

Display::Display(int width, int height, std::string title) {
	this->width = width;
	this->height = height;
	this->title = title;
	this->initialize();
}
		
void Display::initialize() {
	//Start SDL

	char buffer[1024];
	GetCurrentDirectory(1024, buffer);
	std::string str(buffer);
	std::string path = str + "/fonts/BalooTamma2-Medium.ttf";
	
	if (TTF_Init()==-1) 
		printf("TTF_Init: %s\n",TTF_GetError());

	for (int i = this->min_font_size; i<=this->max_font_size; i++) {

		TTF_Font *font = TTF_OpenFont(path.c_str(),i);
		if (!font)
			printf("TTF_OpenFont: %s\n",TTF_GetError());
		else {
			fonts[i]=font;
			//printf("Loaded %ipx font!\n",i);
		}
	}
	
	if(SDL_Init(SDL_INIT_EVERYTHING)<0) {
		std::cout << "Failed to initialize SDL2 - " << SDL_GetError() << std::endl;
		return;
	}
	
	//We start by creating a window
	this->window = SDL_CreateWindow(
		this->title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		this->width,
		this->height,
		SDL_WINDOW_OPENGL
	);

	if (this->window == NULL) {
		std::cout << "Failed to create window - " << SDL_GetError() << std::endl;
		return;
	}

	//We need a renderer to do our rendering
	this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		std::cout << "Failed to create renderer - " << SDL_GetError() << std::endl;
		return;
	}

}	

void Display::show() {
	SDL_RenderPresent(this->renderer);	
	SDL_SetRenderDrawColor( this->renderer, 40,40,40, 255 );
	SDL_RenderClear( this->renderer );
}

//=========================================================================
//https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
void Display::circle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}
//==========================================================================

void Display::draw_line(Vec v1, Vec v2, char color[3]) {	
	SDL_SetRenderDrawColor( this->renderer, color[0], color[1], color[2], 255 );
	SDL_RenderDrawLine(
		this->renderer, v1.get_x(), v1.get_y(), v2.get_x(), v2.get_y()
	);	
}

void Display::draw_line(float x, float y, float x1, float y1, char color[3]) {	
	SDL_SetRenderDrawColor( this->renderer, color[0], color[1], color[2], 255 );
	SDL_RenderDrawLine(
		this->renderer, x, y, x1, y1
	);	
}

void Display::draw_circle(Vec position, float radius, float orientation, char color[3]) {
	SDL_SetRenderDrawColor( this->renderer, color[0], color[1], color[2], 255 );
	this->circle(this->renderer, position.get_x(), position.get_y(), radius);
}	

void Display::draw_text(std::string&& str, int x, int y, char* color, int size) {

	size = std::max(this->min_font_size,std::min(size,this->max_font_size));
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	const char * string = str.c_str();
	int w,h;
	TTF_SizeText(this->fonts[size],string,&w,&h);
	SDL_Color col = {color[0],color[1],color[2]};
	surfaceMessage = TTF_RenderText_Blended(this->fonts[size],string,col);
	Message = SDL_CreateTextureFromSurface(this->renderer,surfaceMessage);
	SDL_Rect Message_rect;
	Message_rect.x = x;
	Message_rect.y = y;
	Message_rect.w = w;
	Message_rect.h = h;
	SDL_RenderCopy(this->renderer,Message,NULL,&Message_rect);	
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);

}



