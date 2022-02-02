#include "particle.h"

particle::particle() :
	id(0),
	velocity(point::point()),
	color(al_map_rgb(0,0,30)),
    is_liquid(false)
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
    is_liquid = false;
}
void sand_particle::tick(particle_map* map, point pos){
    if(try_move(map, pos, pos.below())){return;}
    int rng = rand() % 2;
    if(rng == 0){
        if(try_move(map, pos, pos.below().right())){return;}
        if(try_move(map, pos, pos.below().left())){return;}
    } else{
        if(try_move(map, pos, pos.below().left())){return;}
        if(try_move(map, pos, pos.below().right())){return;}
    } 
}
bool sand_particle::try_move(particle_map* map, point pos, point target){
    if(!map->in_bounds(target))
        return false;
    char id = map->get_next_particle(target)->id;
    if(id == 0) {
        map->swap_particles(pos, target);
        return true;
    }
    if(id == 2) {
        //Try to push water before swapping
        if(map->is_type(0, target.left())){
            map->swap_particles(target, target.left());
        } else if(map->is_type(0, target.right())){
            map->swap_particles(target, target.right());
        } else if(map->is_type(0, target.below())){
            map->swap_particles(target, target.below());
        }
        map->swap_particles(pos, target);
        return true;
    }
    return false;
}

water_particle::water_particle()
{
    id = 2;
	color = al_map_rgb(50, 50, 250);
	velocity = point(0,0);
    is_liquid = true;
}
void water_particle::tick(particle_map* map, point pos){
    //Below
    if(try_move(map,pos, pos.below())){return;}
    //Rng changes the check order
    int rng = rand() % 2;
    if(rng == 0){
        if(try_move(map,pos, pos.below().left())){return;}
        if(try_move(map,pos, pos.left())){return;}
        if(try_move(map,pos, pos.below().right())){return;}
        if(try_move(map,pos, pos.right())){return;}
    } else {
        if(try_move(map,pos, pos.below().right())){return;}
        if(try_move(map,pos, pos.right())){return;}
        if(try_move(map,pos, pos.below().left())){return;}
        if(try_move(map,pos, pos.left())){return;}
    }
}
bool water_particle::try_move(particle_map* map, point pos, point target){
    if(map->is_type(0,target)) {
        map->swap_particles(pos, target);
        return true;
    }
    return false;
}

barrier_particle::barrier_particle(){
    id = 3;
	int rng = rand() % 2;
	if(rng == 1)
        color = al_map_rgb(130, 130, 130);
    else
        color = al_map_rgb(150, 150, 150);
	velocity = point(0,0);
    is_liquid = false;
}
void barrier_particle::tick(particle_map* map, point pos){
}

ice_particle::ice_particle()
{
    id = 4;
	int rng = rand() % 2;
	if(rng == 1)
        color = al_map_rgb(130, 130, 250);
    else
        color = al_map_rgb(150, 150, 250);
	velocity = point(0,0);
    is_liquid = false;
}
void ice_particle::tick(particle_map* map, point pos){
    if (try_spread(map, pos, pos.below())){return;}
    if (try_spread(map, pos, pos.left())){return;}
    if (try_spread(map, pos, pos.above())){return;}
    if (try_spread(map, pos, pos.right())){return;}
}
bool ice_particle::try_spread(particle_map* map, point pos, point target){
    if(map->is_type(2, target)){
        map->set_particle(4, target);
        return true;
    }
    return false;
}

acid_particle::acid_particle()
{
    id = 5;
	color = al_map_rgb(0, 250, 0);
	velocity = point(0,0);
    is_liquid = true;
}
void acid_particle::tick(particle_map* map, point pos){
    if(melt(map,pos)) {return;}
    move(map, pos);
}
bool acid_particle::move(particle_map* map, point pos){
    //Below
    if(try_move(map,pos, pos.below())){return true;}
    //Rng changes the check order
    int rng = rand() % 2;
    if(rng == 0){
        if(try_move(map,pos, pos.below().left())){return true;}
        if(try_move(map,pos, pos.left())){return true;}
        if(try_move(map,pos, pos.below().right())){return true;}
        if(try_move(map,pos, pos.right())){return true;}
    } else {
        if(try_move(map,pos, pos.below().right())){return true;}
        if(try_move(map,pos, pos.right())){return true;}
        if(try_move(map,pos, pos.below().left())){return true;}
        if(try_move(map,pos, pos.left())){return true;}
    }
    return false;
}
bool acid_particle::melt(particle_map* map, point pos){
    if (try_melt(map, pos, pos.below())){return true;}
    if (try_melt(map, pos, pos.left())){return true;}
    if (try_melt(map, pos, pos.above())){return true;}
    if (try_melt(map, pos, pos.right())){return true;}

    return false;
}
bool acid_particle::try_move(particle_map* map, point pos, point target){
    if(map->in_bounds(target)){
        char id = map->get_next_particle(target)->id;
        if(id == 0) {
            map->swap_particles(pos, target);
            return true;
        }
    }
    return false;
}
bool acid_particle::try_melt(particle_map* map, point pos, point target){
    if(!map->in_bounds(target))
        return false;
    particle* target_p = map->get_next_particle(target);

    int rng = rand() % 256;
    if(rng < 255 && !target_p->is_liquid)
        return false;

    if(target_p->id != 5 && target_p->id != 0){
        map->set_particle(0, target);
        map->set_particle(0, pos);
        return true;
    }
    return false;
}