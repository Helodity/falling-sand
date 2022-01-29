#ifndef PARTICLE
#define PARTICLE

#include "point.h"
#include "global.h"
#include "particle_map.h"

class particle_map; //TODO: resolve cyclical dependency
class particle {
public:
	char id; // Uneeded since each type is its own class. will remove
	
	//These variables can vary between particles
	point velocity;
	ALLEGRO_COLOR color;
	virtual void tick(particle_map* map, point pos) = 0;
	particle();
	particle(char id, ALLEGRO_COLOR c, point v);
	~particle();
};

class air_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	air_particle();
};

class sand_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	sand_particle();
};

class water_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	water_particle();
};

class barrier_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	barrier_particle();
};

class ice_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	ice_particle();
};


#endif