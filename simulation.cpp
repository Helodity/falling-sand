#include "simulation.h"

simulation::simulation()
{
	for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            rngValues[x][y] = rand() % 255;
            currentParticles[x][y] = 0;
            nextParticles[x][y] = 0;
        }
    }
    draw(true);
}

bool simulation::cell_exists(point pos){
    return pos.x >= 0 && pos.y >= 0 && pos.x < SCREEN_WIDTH && pos.y < SCREEN_HEIGHT;
}

bool simulation::is_air(point pos){
    return cell_exists(pos) && nextParticles[pos.x][pos.y] == 0;
}


void simulation::tick(){

    currentParticles[SCREEN_WIDTH / 4][0] = 1;
    currentParticles[SCREEN_WIDTH / 2][0] = 1;
    currentParticles[3 * SCREEN_WIDTH / 4][0] = 1;
    currentParticles[145][0] = 1;
    currentParticles[170][0] = 2;

    tick_particles();
    draw(false);
}

void simulation::tick_particles(){
    changedParticles.clear();
    for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            point p = point(x,y);
            unsigned char id = currentParticles[x][y];
            switch (id) {
                //Sand
                case 1:
                    tick_sand(p);
                break;
                //Water
                case 2:
                    tick_water(p);
                break;
                case 3:
                break;
            }
        }
    }
    for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_WIDTH; y++){
            currentParticles[x][y] = nextParticles[x][y];
        }
    }
}
void simulation::tick_water(point pos){
    int rng = rand() % 2;
    //Below
    if(is_air(pos.below())) {
        swap_particles(pos, pos.below());
        return;
    }
    //Below Left
    if(is_air(pos.below().left()) && rng == 0) {
        swap_particles(pos, pos.below().left());
        return;
    }
    //Below Right
    if(is_air(pos.below().right()) && rng == 1) {
        swap_particles(pos, pos.below().right());
        return;
    }
    //Left
    if(is_air(pos.left()) && rng == 0) {
        swap_particles(pos, pos.left());
        return;
    }
    //Right
    if(is_air(pos.right()) && rng == 1) {
        swap_particles(pos, pos.right());
        return;
    }
}

void simulation::tick_sand(point pos){
     int rng = rand() % 2;
    //Below
    if(cell_exists(pos.below())) {
        point t = pos.below();
        if(nextParticles[pos.x][pos.y + 1] == 0) {
            swap_particles(pos, t);
            return;
        }
        if(nextParticles[pos.x][pos.y + 1] == 2) {
            //Try to push water before swapping
            if(is_air(t.left())){
                swap_particles(t, t.left());
            } else if(is_air(t.right())){
                swap_particles(t, t.right());
            } else if(is_air(t.below())){
                swap_particles(t, t.below());
            }
            swap_particles(pos, t);
            return;
        }
    }
    //Below Left
    if(cell_exists(pos.below().left()) && rng == 0) {
        point t = pos.below().left();
        if(nextParticles[pos.x - 1][pos.y + 1] == 0) {
            swap_particles(pos, t);
            return;
        }
        if(nextParticles[pos.x - 1][pos.y + 1] == 2) {
            //Try to push water before swapping
            if(is_air(t.left())){
                swap_particles(t, t.left());
            } else if(is_air(t.right())){
                swap_particles(t, t.right());
            } else if(is_air(t.below())){
                swap_particles(t, t.below());
            }
            swap_particles(pos, t);
            return;
        }
    }
    //Below Right
    if(cell_exists(pos.below().right()) && rng == 1) {
        point t = pos.below().right();
        if(nextParticles[pos.x + 1][pos.y + 1] == 0) {
            swap_particles(pos, t);
            return;
        }
        if(nextParticles[pos.x + 1][pos.y + 1] == 2) {
            //Try to push water before swapping
            if(is_air(t.left())){
                swap_particles(t, t.left());
            } else if(is_air(t.right())){
                swap_particles(t, t.right());
            } else if(is_air(t.below())){
                swap_particles(t, t.below());
            }
            swap_particles(pos, t);
            return;
        }
    }
}


void simulation::swap_particles(point start, point target){
    nextParticles[start.x][start.y] = nextParticles[target.x][target.y];
    nextParticles[target.x][target.y] = currentParticles[start.x][start.y];
    changedParticles.push_back(start);
    changedParticles.push_back(target);
}

void simulation::draw(bool redraw_all){
    if(!redraw_all){
        al_hold_bitmap_drawing(true);
        for(size_t i = 0; i < changedParticles.size(); i++){
            point p = changedParticles[i];
            al_draw_pixel(p.x,p.y, get_color(p));
        }
        al_hold_bitmap_drawing(false);
    } else {
        al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
                al_put_pixel(x - 1,y - 1, get_color(point(x,y)));
            }
        }
        al_unlock_bitmap(al_get_target_bitmap());
    }
    al_flip_display();
}
void simulation::fill_area(char id, point top_left, point bottom_right){
    for(unsigned int x = top_left.x; x < bottom_right.x; x++){
        for(unsigned int y = top_left.y; y < bottom_right.y; y++){
            currentParticles[x][y] = id;
            nextParticles[x][y] = id;
        }
    }
}
ALLEGRO_COLOR simulation::get_color(point p){
    unsigned char id = currentParticles[p.x][p.y];
    char rng = rngValues[p.x][p.y];
    switch (id) {
        case 1:
            rng = rng % 2;
            if(rng == 1)
                return al_map_rgb(180, 158, 0);
            return al_map_rgb(180, 180, 0);
        case 2:
            return al_map_rgb(20, 40, 200);
        case 3:
            return al_map_rgb(100, 100, 100);
    }
    return al_map_rgb(0, 0, 30);
}