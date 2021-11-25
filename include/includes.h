#ifndef INCLUDES_H
#define INCLUDES_H

#define MAX_V 10
#define MIN_V 3
#define DEF_GRAV 3000.0f
#define MAX_BODIES 1000
#define POLYGON 0
#define CIRCLE 1
#define MIN_POLY_RAD 30
#define MAX_POLY_RAD 70
#define MAX_INT 99999999

#define WHITE 255,255,255
#define GREEN 249,130,39
#define DARK_GREY 30,30,30
#define DARK_GREY_2 60,60,60
#define YELLOW 222, 237, 11
#define LIGHT_GREY 210,210,210
#define SHOW_NORMALS 1 << 0
#define SHOW_POLYMIDS 1 << 1
#define SHOW_POLY_OUTLINES 1 << 2
#define resolution_iterations 10
#define FPS 60
const float dt = (float) 1.0f/((float)FPS);

#define RED			250,60,90
#define DARK_NAVY	58,68,110
#define ORANGE		255,190,80
#define CREAM		255,230,205
#define PINK		250,148,131
#define BLUE		64,151,170
#define RED2		193,73,108

#define NAVY_2 			0x6c,0x8a,0x9b
#define LIGHT_GREY_2	0xe6,0xe6,0xe4
#define LIGHT_GREY_3	0xf9,0xf5,0xef
#define RED_2			0xfd,0x65,0x74

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include "Vector.h"
#include "Display.h"
#include "Polygon.h"
#include "Circle.h"
#include "Shape.h"
#include "utils.h"
#include "math.h"
#include "Body.h"
#include "World.h"
#include "Constraints.h"
#include <memory>

#endif
