#ifndef PARTICLE_MAP
#define PARTICLE_MAP
#include "global.h"
#include "particle.h"

class particle; //TODO: resolve cyclical dependency
class particle_map{
private:

	particle* current_particles[SCREEN_WIDTH][SCREEN_HEIGHT];
	particle* next_particles[SCREEN_WIDTH][SCREEN_HEIGHT];
	vector<point> changed_particles;

public:
	particle_map();
	~particle_map();

	bool in_bounds(point pos);
	bool is_type(char id, point pos);

	void set_particle(particle* part, point pos);
	void swap_particles(point start, point target);

	void fill_rectangle_area(char id, point top_left, point bottom_right);
	void fill_circular_area(char id, point origin, int radius);

	particle* create_particle(char id);
	particle* get_current_particle(point p);
	particle* get_next_particle(point p);

	void store_next_particles();
	void clear_changed_particles();
	vector<point> get_changed_particles();
};
#endif