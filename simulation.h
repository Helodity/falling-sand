#ifndef SIMULATION
#define SIMULATION

#include "point.h"
#include "global.h"
#include "input_struct.h"
#include "particle.h"
#include "particle_map.h"

class simulation {
public:
	simulation();

	void tick();
	void handle_event(ALLEGRO_EVENT ev);
private:

	particle_map* map;

	vector<point> tick_order;

	ALLEGRO_BITMAP* last_frame;

	input_struct* user_input;
	user_stuff* place_data;

	void handle_user_input();

	void tick_particles();
	void tick_sand(point pos);
	void tick_water(point pos);
	void tick_ice(point pos);

	void draw_scene();
	void draw_particles(bool redraw_all);
};
#endif