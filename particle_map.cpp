#include "particle_map.h"


particle_map::particle_map(){
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(!in_bounds(point(x,y)))
                continue;

            particle* p = create_particle(0);
            next_particles[x][y] = p;
            current_particles[x][y] = p;
        }
    }
}

particle_map::~particle_map(){
}

bool particle_map::in_bounds(point pos){
    return pos.x >= 0 && pos.y >= 0 && pos.x < SCREEN_WIDTH && pos.y < SCREEN_HEIGHT;
}
bool particle_map::is_type(char id, point pos){
    return in_bounds(pos) && get_next_particle(pos)->id == id;
}

void particle_map::set_particle(char id, point pos){
    if(!in_bounds(pos))
        return;
    if(is_type(id, pos))
        return;
    to_delete.push_back(next_particles[pos.x][pos.y]);

    particle* p = create_particle(id);
    current_particles[pos.x][pos.y] = p;
	next_particles[pos.x][pos.y] = p;
    changed_particles.push_back(pos);
}
void particle_map::move_particle(particle* part, point pos){
    if(!in_bounds(pos))
        return;

	next_particles[pos.x][pos.y] = part;
    changed_particles.push_back(pos);
}

void particle_map::swap_particles(point start, point target){
	particle* original = next_particles[start.x][start.y];
    move_particle(next_particles[target.x][target.y], start);
    move_particle(original, target);
}

void particle_map::fill_rectangle_area(char id, point top_left, point bottom_right){
    for(int x = top_left.x; x < bottom_right.x; x++){
        for(int y = top_left.y; y < bottom_right.y; y++){
            point cur_point = point(x,y);
            if(!in_bounds(cur_point))
                continue;
            if(is_type(id, cur_point))
                continue;
            set_particle(id, cur_point);
        }
    }
}

void particle_map::fill_circular_area(char id, point origin, int radius){
    point cur_point = point(0,0);
    for(int x = -radius; x < radius; x++){
        for(int y = -radius; y < radius; y++){
            cur_point = point(origin.x + x,origin.y + y);
            if(!in_bounds(cur_point))
                continue;
            if(is_type(id, cur_point))
                continue;
            if(x * x + y * y > radius * radius)
                continue;
            set_particle(id, cur_point);
        }
    }
}

particle* particle_map::create_particle(char id){ 
    switch (id) {
        case 1:
            return new sand_particle();
        case 2:
            return new water_particle();
        case 3:
            return new barrier_particle();
        case 4:
            return new ice_particle();
        case 5:
            return new acid_particle();
        case 6:
            return new spout_particle();
    }
    return new air_particle();
}

particle* particle_map::get_current_particle(point p){
    return current_particles[p.x][p.y];
}
particle* particle_map::get_next_particle(point p){
    return next_particles[p.x][p.y];
}

void particle_map::delete_unused(){
    int size = to_delete.size();
    for(size_t i = 0; i < size; i++){
        delete to_delete[i];
    }
    to_delete.clear();
}

void particle_map::store_next_particles(){
    int size = changed_particles.size();
    for(size_t i = 0; i < size; i++){
        point p = changed_particles[i];
        current_particles[p.x][p.y] = next_particles[p.x][p.y];
    }
}
void particle_map::clear_changed_particles(){
    changed_particles.clear();
}
vector<point> particle_map::get_changed_particles(){
    return changed_particles;
}