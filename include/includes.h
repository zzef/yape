#ifndef INCLUDES_H
#define INCLUDES_H

#define MAX_V 10
#define MIN_V 3
#define DEF_GRAV 2000.0f
#define MAX_BODIES 1000
#define POLYGON 0
#define CIRCLE 1
#define MIN_POLY_RAD 20
#define MAX_POLY_RAD 90

#define RED 240, 50, 80
#define YELLOW 222, 237, 11
#define SHOW_NORMALS 1 << 0
#define SHOW_POLYMIDS 1 << 1
#define resolution_iterations 20
#define FPS 60
const float dt = (float) 1.0f/((float)FPS);

#include "SDL2/SDL.h"
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
#include "Joints.h"
#include <memory>

#endif
