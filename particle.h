#ifndef PARTICLE
#define PARTICLE

#include "point.h"
#include "global.h"
//#include "particle_map.h"

class particle_map;
class particle {
public:
	char id; // Defines traits about the particle that are the same between all of the same typu
	
	//These variables can vary between particles
	point velocity;
	ALLEGRO_COLOR color;
	void tick(); //Can't have array of particles because of this
	particle();
	particle(char id, ALLEGRO_COLOR c, point v);
};

class sand_particle : particle{
	//void tick();

};


#endif