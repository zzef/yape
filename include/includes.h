#ifndef INCLUDES_H
#define INCLUDES_H

#define GRAVITY_SCALE 1000
#define MAX_V 8
#define MIN_V 3
#define GRAVITY 1.5f
#define DEF_GRAV (GRAVITY_SCALE * GRAVITY)
#define MAX_BODIES 2000
#define POLYGON 0
#define CIRCLE 1
#define MIN_POLY_RAD 40
#define MAX_POLY_RAD 80
#define MAX_INT 99999999
#define QUAD_TREE_CAPACITY 4

#define WHITE 255,255,255
#define GREEN 249,130,39
#define DARK_GREY 15,15,15
#define DARK_GREY_2 40,40,40
#define DARK_GREY_3 65,65,65
#define YELLOW 222, 237, 11
#define LIGHT_GREY 210,210,210
#define SHOW_NORMALS 1 << 0
#define SHOW_POLYMIDS 1 << 1
#define SHOW_WIREFRAME 1 << 2
#define resolution_iterations 10
#define FPS 2000
#define PPS 60
const float dt = (float) 1.0f/((float)PPS);

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

#define O_RED			0xff,0x4a,0x5a	
#define O_NAVY			0x3f,0x3c,0x5a
#define O_ORANGE		0xff,0xa5,0x64
#define O_CREAM			0xeb,0xe8,0xcd

#define rot_damp_constant 0.999f

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
#include <unistd.h>

#endif
