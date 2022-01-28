#include "simulation.h"

simulation::simulation()
{
    user_input = new input_struct();
	for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            rngValues[x][y] = rand() % 255;
            currentParticles[x][y] = 0;
            nextParticles[x][y] = 0;
            tick_order.push_back(point(x,y));
        }
    }
    std::random_shuffle(tick_order.begin(), tick_order.end());
    draw_particles(true);
}

void simulation::handle_event(ALLEGRO_EVENT ev){
    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        if(ev.mouse.button & 1)
            user_input->left_mouse_down = true;
        if(ev.mouse.button & 2)
            user_input->right_mouse_down = true;
    }
    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        if(ev.mouse.button & 1)
            user_input->left_mouse_down = false;
        if(ev.mouse.button & 2)
            user_input->right_mouse_down = false;
    }
    if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
        user_input->mouse_pos = point(ev.mouse.x, ev.mouse.y);
    }
}

void simulation::tick(){
    fill_area(2, point(100,0), point(110, 1));
    fill_area(1, point(200,0), point(210, 1));

    if(user_input->left_mouse_down)
        fill_area(3, user_input->mouse_pos, 10);
    if(user_input->right_mouse_down)
        fill_area(0, user_input->mouse_pos, 10);

    tick_particles();
    draw_particles(false);
}

bool simulation::cell_exists(point pos){
    return pos.x >= 0 && pos.y >= 0 && pos.x < SCREEN_WIDTH && pos.y < SCREEN_HEIGHT;
}

bool simulation::is_air(point pos){
    return cell_exists(pos) && nextParticles[pos.x][pos.y] == 0;
}

void simulation::tick_particles(){

    for(size_t i = 0; i < tick_order.size(); i++){
        point p = tick_order[i];
        unsigned char id = currentParticles[p.x][p.y];
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
    char id = nextParticles[start.x][start.y];
    nextParticles[start.x][start.y] = nextParticles[target.x][target.y];
    nextParticles[target.x][target.y] = id;
    changedParticles.push_back(start);
    changedParticles.push_back(target);
}

void simulation::draw_particles(bool redraw_all){
    if(!redraw_all){
        al_hold_bitmap_drawing(true);
        for(size_t i = 0; i < changedParticles.size(); i++){
            point p = changedParticles[i];
            al_draw_pixel(p.x ,p.y, get_color(p));
        }
        al_hold_bitmap_drawing(false);
        changedParticles.clear();
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
    for(int x = top_left.x; x < bottom_right.x; x++){
        for(int y = top_left.y; y < bottom_right.y; y++){
            if(!cell_exists(point(x,y)))
                continue;
            changedParticles.push_back(point(x,y));
            currentParticles[x][y] = id;
            nextParticles[x][y] = id;
        }
    }
}
void simulation::fill_area(char id, point origin, int radius){
    for(int x = -radius; x < radius; x++){
        for(int y = -radius; y < radius; y++){
            point cur_point = point(origin.x + x,origin.y + y);
            if(!cell_exists(cur_point))
                continue;
            if(x * x + y * y > radius * radius)
                continue;

            changedParticles.push_back(cur_point);
            currentParticles[cur_point.x][cur_point.y] = id;
            nextParticles[cur_point.x][cur_point.y] = id;
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