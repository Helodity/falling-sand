#include "particle_map.h"


particle_map::particle_map(){
}

particle_map::~particle_map(){
}

bool particle_map::in_bounds(point pos){
    return pos.x >= 0 && pos.y >= 0 && pos.x < SCREEN_WIDTH && pos.y < SCREEN_HEIGHT;
}


void particle_map::set_particle(particle part, point pos){
	next_particles[pos.x][pos.y] = part;
    changed_particles.push_back(pos);
}

void particle_map::swap_particles(point start, point target){
	particle original = next_particles[start.x][start.y];
    set_particle(next_particles[target.x][target.y], start);
    set_particle(original, target);
}

void particle_map::fill_rectangle_area(char id, point top_left, point bottom_right){
    for(int x = top_left.x; x < bottom_right.x; x++){
        for(int y = top_left.y; y < bottom_right.y; y++){
            if(!in_bounds(point(x,y)))
                continue;
            changed_particles.push_back(point(x,y));
            particle p = create_particle(id);
            set_particle(p, point(x,y));
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
            if(x * x + y * y > radius * radius)
                continue;

            changed_particles.push_back(cur_point);
            particle p = create_particle(id);
            current_particles[cur_point.x][cur_point.y] = p;
            next_particles[cur_point.x][cur_point.y] = p;
        }
    }
}

particle particle_map::create_particle(char id){
    int rng = rand() % 2;
    ALLEGRO_COLOR color = al_map_rgb(0,0,30);
    switch (id) {
        case 1:
            if(rng == 1)
                color = al_map_rgb(180, 150, 0);
            else
                color = al_map_rgb(200, 180, 0);
            break;
        case 2:
            color = al_map_rgb(50, 50, 250);
            break;
        case 3:
            if(rng == 1)
                color = al_map_rgb(130, 130, 130);
            else
                color = al_map_rgb(150, 150, 150);
            break;
        case 4:
            if(rng == 1)
                color = al_map_rgb(130, 130, 250);
            else
                color = al_map_rgb(180, 180, 250);
            break;
    }
    return particle(id, color, point(0,0));
}

particle particle_map::get_current_particle(point p){
    return current_particles[p.x][p.y];
}
particle particle_map::get_next_particle(point p){
    return next_particles[p.x][p.y];
}
void particle_map::store_next_particles(){
    for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            current_particles[x][y] = next_particles[x][y];
        }
    }
}
void particle_map::clear_changed_particles(){
    changed_particles.clear();
}
vector<point> particle_map::get_changed_particles(){
    return changed_particles;
}