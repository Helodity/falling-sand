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

particle::~particle()
{
}


air_particle::air_particle():
    particle(){
}

void air_particle::tick(particle_map* map, point pos){
}

sand_particle::sand_particle()
{
	id = 1;
	int rng = rand() % 2;
	if(rng == 1)
        color = al_map_rgb(180, 150, 0);
    else
        color = al_map_rgb(200, 180, 0);
	velocity = point(0,0);
}
void sand_particle::tick(particle_map* map, point pos){
	int rng = rand() % 2;
    //Below
    if(map->in_bounds(pos.below())) {
        point t = pos.below();
        char id = map->get_next_particle(t)->id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left())->id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right())->id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below())->id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
    //Below Left
    if(map->in_bounds(pos.below().left()) && rng == 0) {
        point t = pos.below().left();
        char id = map->get_next_particle(t)->id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left())->id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right())->id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below())->id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
    //Below Right
    if(map->in_bounds(pos.below().right()) && rng == 1) {
        point t = pos.below().right();
        char id = map->get_next_particle(t)->id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left())->id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right())->id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below())->id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
}


water_particle::water_particle()
{
    id = 2;
	color = al_map_rgb(50, 50, 250);
	velocity = point(0,0);
}

void water_particle::tick(particle_map* map, point pos){
    int rng = rand() % 2;
    //Below
    if(map->in_bounds(pos.below()) && map->get_next_particle(pos.below())->id == 0) {
        map->swap_particles(pos, pos.below());
        return;
    }
    //Below Left
    if(map->in_bounds(pos.below().left()) && map->get_next_particle(pos.below().left())->id == 0 && rng == 0) {
        map->swap_particles(pos, pos.below().left());
        return;
    }
    //Below Right
    if(map->in_bounds(pos.below().right()) && map->get_next_particle(pos.below().right())->id == 0 && rng == 1) {
        map->swap_particles(pos, pos.below().right());
        return;
    }
    //Left
    if(map->in_bounds(pos.left()) && map->get_next_particle(pos.left())->id == 0 && rng == 0) {
        map->swap_particles(pos, pos.left());
        return;
    }
    //Right
    if(map->in_bounds(pos.right()) && map->get_next_particle(pos.right())->id == 0 && rng == 1) {
        map->swap_particles(pos, pos.right());
        return;
    }
}

barrier_particle::barrier_particle(){
    id = 1;
	int rng = rand() % 2;
	if(rng == 1)
        color = al_map_rgb(130, 130, 130);
    else
        color = al_map_rgb(150, 150, 150);
	velocity = point(0,0);
}

void barrier_particle::tick(particle_map* map, point pos){
}

ice_particle::ice_particle()
{
    id = 1;
	int rng = rand() % 2;
	if(rng == 1)
        color = al_map_rgb(130, 130, 250);
    else
        color = al_map_rgb(150, 150, 250);
	velocity = point(0,0);
}

void ice_particle::tick(particle_map* map, point pos){
    if(map->in_bounds(pos.below())){
        if(map->get_current_particle(pos.below())->id == 2) {
            map->set_particle(map->create_particle(4), pos.below());
            return;
        }
    }
    if(map->in_bounds(pos.below().left())){
        if(map->get_current_particle(pos.below().left())->id == 2) {
            map->set_particle(map->create_particle(4), pos.below().left());
            return;
        }
    }
    if(map->in_bounds(pos.below().right())){
        if(map->get_current_particle(pos.below().right())->id == 2) {
            map->set_particle(map->create_particle(4), pos.below().right());
            return;
        }
    }
    if(map->in_bounds(pos.above())){
        if(map->get_current_particle(pos.above())->id == 2) {
            map->set_particle(map->create_particle(4), pos.above());
            return;
        }
    }
    if(map->in_bounds(pos.above().left())){
        if(map->get_current_particle(pos.above().left())->id == 2) {
            map->set_particle(map->create_particle(4), pos.above().left());
            return;
        }
    }
    if(map->in_bounds(pos.above().right())){
        if(map->get_current_particle(pos.above().right())->id == 2) {
            map->set_particle(map->create_particle(4), pos.above().right());
            return;
        }
    }
    if(map->in_bounds(pos.left())){
        if(map->get_current_particle(pos.left())->id == 2) {
            map->set_particle(map->create_particle(4), pos.left());
            return;
        }
    }
    if(map->in_bounds(pos.left())){
        if(map->get_current_particle(pos.left())->id == 2) {
            map->set_particle(map->create_particle(4), pos.right());
            return;
        }
    }
}