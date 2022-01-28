#pragma once
#include "point.h"
#include "header.h"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

class simulation {
public:
	simulation();

	void tick();
	void handle_event(ALLEGRO_EVENT ev);
private:
	unsigned char currentParticles[SCREEN_WIDTH][SCREEN_HEIGHT];
	unsigned char nextParticles[SCREEN_WIDTH][SCREEN_HEIGHT];
	unsigned char rngValues[SCREEN_WIDTH][SCREEN_HEIGHT];
	vector<point> changedParticles;

	bool cell_exists(point pos);
	bool is_air(point pos);

	void tick_particles();
	void tick_sand(point pos);
	void tick_water(point pos);
	void swap_particles(point start, point target);
	void draw(bool redraw_all);
	void fill_area(char id, point top_left, point bottom_right);
	void fill_area(char id, point origin, int radius);

	ALLEGRO_COLOR get_color(point p);
};