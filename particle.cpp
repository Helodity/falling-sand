#include "particle.h"

particle::particle() :
	id(0),
	velocity(point::point()),
	color(al_map_rgb(0,0,30))
{
}

particle::particle(char id, ALLEGRO_COLOR c, point v) :
	id(id),
	velocity(v),
	color(c)
{
}
