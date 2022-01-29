#ifndef SIMULATION
#define SIMULATION

#include "point.h"
#include "header.h"
#include "input_struct.h"
#include "particle.h"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

class simulation {
public:
	simulation();

	void tick();
	void handle_event(ALLEGRO_EVENT ev);
private:
	particle currentParticles[SCREEN_WIDTH][SCREEN_HEIGHT];
	particle nextParticles[SCREEN_WIDTH][SCREEN_HEIGHT];

	vector<point> tick_order;
	vector<point> changedParticles;

	ALLEGRO_BITMAP* last_frame;

	input_struct* user_input;
	user_stuff* place_data;

	bool cell_exists(point pos);
	bool is_air(point pos);

	void handle_user_input();

	void tick_particles();
	void tick_sand(point pos);
	void tick_water(point pos);
	void tick_ice(point pos);

	particle create_particle(char id);
	void set_particle(particle part, point pos);
	void swap_particles(point start, point target);

	void draw_scene();
	void draw_particles(bool redraw_all);

	void fill_area(char id, point top_left, point bottom_right);
	void fill_area(char id, point origin, int radius);

	ALLEGRO_COLOR get_color(point p);
};
#endif