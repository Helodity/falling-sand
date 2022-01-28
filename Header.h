#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdio.h>
#include <chrono>

#include "point.h"
using namespace std;

const int TARGET_FPS = 100;
float CURRENT_FPS;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

unsigned char currentParticles[SCREEN_WIDTH][SCREEN_HEIGHT];
unsigned char nextParticles[SCREEN_WIDTH][SCREEN_HEIGHT];
unsigned char rngValues[SCREEN_WIDTH][SCREEN_HEIGHT];
vector<point> changedParticles;