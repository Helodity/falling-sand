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
	float disolve_chance;


	virtual void tick(particle_map* map, point pos) = 0;
	particle();
	~particle();
};

class air_particle : public particle {
public:
	void tick(particle_map* map, point pos);
	air_particle();
};

class sand_particle : public particle {
private:
	bool try_move(particle_map* map, point pos, point target);

public:
	void tick(particle_map* map, point pos);
	sand_particle();
};

class water_particle : public particle {
private:
	bool try_move(particle_map* map, point pos, point target);
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
private:
	bool try_spread(particle_map* map, point pos, point target);

public:
	void tick(particle_map* map, point pos);
	ice_particle();
};

class acid_particle : public particle {
private:
	bool move(particle_map* map, point pos);
	bool melt(particle_map* map, point pos);
	bool try_move(particle_map* map, point pos, point target);
	bool try_melt(particle_map* map, point pos, point target);

public:
	void tick(particle_map* map, point pos);
	acid_particle();
};

class spout_particle : public particle {
private:
	void copy(particle_map* map, point pos);
	bool try_copy(particle_map* map, point pos, point target);
	void spawn(particle_map* map, point pos);
	bool try_spawn(particle_map* map, point pos, point target);
	char id_to_spawn = 0;
public:
	void tick(particle_map* map, point pos);
	spout_particle();
};


#endif