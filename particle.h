#ifndef PARTICLE
#define PARTICLE

#include "point.h"
#include "header.h"

class particle {
public:
	char id; // Defines traits about the particle that are the same between all of the same typu
	
	//These variables can vary between particles
	point velocity;
	ALLEGRO_COLOR color;

	particle();
	particle(char id, ALLEGRO_COLOR c, point v);
};
#endif