#ifndef INCLUDES_H
#define INCLUDES_H

#define MAX_V 10
#define MIN_V 3
#define DEF_GRAV 0.002
#define MAX_BODIES 100
#define POLYGON 0
#define CIRCLE 1
#define MIN_POLY_RAD 20
#define MAX_POLY_RAD 140

#define RED 240, 50, 80
#define YELLOW 222, 237, 11
#define SHOW_NORMALS 1 << 0
#define SHOW_POLYMIDS 1 << 1

#include <unistd.h>
#include <windows.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>
#include "Vector.h"
#include <map>
#include "Display.h"
#include "Polygon.h"
#include "Circle.h"
#include "Shape.h"
#include "utils.h"
#include "math.h"
#include "Body.h"
#include "World.h"
#include "Joints.h"
#include <sstream>
#include <fstream>
#include <memory>

#endif
