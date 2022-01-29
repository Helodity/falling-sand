#include "simulation.h"

simulation::simulation()
{
    user_input = new input_struct();
    place_data = new user_stuff();
    map = new particle_map();
	for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            tick_order.push_back(point(x,y));
        }
    }
    std::random_shuffle(tick_order.begin(), tick_order.end());

    map->fill_rectangle_area(0, point(0,0), point(SCREEN_WIDTH, SCREEN_HEIGHT));

    draw_particles(true);
    last_frame = al_clone_bitmap(al_get_target_bitmap());
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
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
        user_input->keycodes.push_back(ev.keyboard.keycode);
    }
    if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
        user_input->scroll_wheel_change = ev.mouse.dz;
        user_input->mouse_pos = point(ev.mouse.x, ev.mouse.y);
    }
}

void simulation::handle_user_input(){
    if(user_input->scroll_wheel_change != 0){
        place_data->place_radius += user_input->scroll_wheel_change;
        user_input->scroll_wheel_change = 0;
    }

    for(size_t i = 0; i < user_input->keycodes.size(); i++){
        int keycode = user_input->keycodes[i];
        switch (keycode)
        {
        case ALLEGRO_KEY_1:
            place_data->selected_id = 1;
            break;
            case ALLEGRO_KEY_2:
            place_data->selected_id = 2;
            break;
            case ALLEGRO_KEY_3:
            place_data->selected_id = 3;
            break;
            case ALLEGRO_KEY_4:
            place_data->selected_id = 4;
            break;
        }
    }
    user_input->keycodes.clear();

    if(user_input->left_mouse_down)
        map->fill_circular_area(place_data->selected_id, user_input->mouse_pos, place_data->place_radius);
    if(user_input->right_mouse_down)
        map->fill_circular_area(0, user_input->mouse_pos, place_data->place_radius);
}

void simulation::tick(){
    handle_user_input();
    tick_particles();
    draw_scene();
}

void simulation::tick_particles(){
    for(size_t i = 0; i < tick_order.size(); i++){
        point p = tick_order[i];
        unsigned char id = map->get_current_particle(p).id;
            switch (id) {
                //Sand
                case 1:
                    tick_sand(p);
                break;
                //Water
                case 2:
                    tick_water(p);
                break;
                case 4:
                    tick_ice(p);
                break;
            }
    }
    map->store_next_particles();
}
void simulation::tick_water(point pos){
    int rng = rand() % 2;
    //Below
    if(map->get_next_particle(pos.below()).id == 0) {
        map->swap_particles(pos, pos.below());
        return;
    }
    //Below Left
    if(map->get_next_particle(pos.below().left()).id == 0 && rng == 0) {
        map->swap_particles(pos, pos.below().left());
        return;
    }
    //Below Right
    if(map->get_next_particle(pos.below().right()).id == 0 && rng == 1) {
        map->swap_particles(pos, pos.below().right());
        return;
    }
    //Left
    if(map->get_next_particle(pos.left()).id == 0 && rng == 0) {
        map->swap_particles(pos, pos.left());
        return;
    }
    //Right
    if(map->get_next_particle(pos.right()).id == 0 && rng == 1) {
        map->swap_particles(pos, pos.right());
        return;
    }
}

void simulation::tick_sand(point pos){
    int rng = rand() % 2;
    //Below
    if(map->in_bounds(pos.below())) {
        point t = pos.below();
        char id = map->get_next_particle(t).id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left()).id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right()).id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below()).id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
    //Below Left
    if(map->in_bounds(pos.below().left()) && rng == 0) {
        point t = pos.below().left();
        char id = map->get_next_particle(t).id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left()).id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right()).id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below()).id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
    //Below Right
    if(map->in_bounds(pos.below().right()) && rng == 1) {
        point t = pos.below().right();
        char id = map->get_next_particle(t).id;
        if(id == 0) {
            map->swap_particles(pos, t);
            return;
        }
        if(id == 2) {
            //Try to push water before swapping
            if(map->get_next_particle(t.left()).id == 0){
                map->swap_particles(t, t.left());
            } else if(map->get_next_particle(t.right()).id == 0){
                map->swap_particles(t, t.right());
            } else if(map->get_next_particle(t.below()).id == 0){
                map->swap_particles(t, t.below());
            }
            map->swap_particles(pos, t);
            return;
        }
    }
}

void simulation::tick_ice(point pos){
    if(map->in_bounds(pos.below())){
        if(map->get_current_particle(pos.below()).id == 2) {
            map->set_particle(map->create_particle(4), pos.below());
            return;
        }
    }
    if(map->in_bounds(pos.below().left())){
        if(map->get_current_particle(pos.below().left()).id == 2) {
            map->set_particle(map->create_particle(4), pos.below().left());
            return;
        }
    }
    if(map->in_bounds(pos.below().right())){
        if(map->get_current_particle(pos.below().right()).id == 2) {
            map->set_particle(map->create_particle(4), pos.below().right());
            return;
        }
    }
    if(map->in_bounds(pos.above())){
        if(map->get_current_particle(pos.above()).id == 2) {
            map->set_particle(map->create_particle(4), pos.above());
            return;
        }
    }
    if(map->in_bounds(pos.above().left())){
        if(map->get_current_particle(pos.above().left()).id == 2) {
            map->set_particle(map->create_particle(4), pos.above().left());
            return;
        }
    }
    if(map->in_bounds(pos.above().right())){
        if(map->get_current_particle(pos.above().right()).id == 2) {
            map->set_particle(map->create_particle(4), pos.above().right());
            return;
        }
    }
    if(map->in_bounds(pos.left())){
        if(map->get_current_particle(pos.left()).id == 2) {
            map->set_particle(map->create_particle(4), pos.left());
            return;
        }
    }
    if(map->in_bounds(pos.right())){
        if(map->get_current_particle(pos.left()).id == 2) {
            map->set_particle(map->create_particle(4), pos.right());
            return;
        }
    }
}

void simulation::draw_scene(){
    draw_particles(false);
    map->clear_changed_particles();

    al_destroy_bitmap(last_frame);
    last_frame = al_clone_bitmap(al_get_target_bitmap());

    al_draw_circle(user_input->mouse_pos.x,user_input->mouse_pos.y, place_data->place_radius, al_map_rgb(255,0,0), 2);
    al_flip_display();
}

void simulation::draw_particles(bool redraw_all){
    if(!redraw_all){
        al_draw_bitmap(last_frame, 0, 0, 0);
        al_hold_bitmap_drawing(true);
        vector<point> changed = map->get_changed_particles();
        for(size_t i = 0; i < changed.size(); i++){
            point p = changed[i];
            al_draw_pixel(p.x ,p.y, map->get_current_particle(p).color);
        }
        al_hold_bitmap_drawing(false);
    } else {
        al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        for(unsigned int y = 0; y < SCREEN_HEIGHT; y++){
            for(unsigned int x = 0; x < SCREEN_WIDTH; x++){
                al_put_pixel(x - 1,y - 1, map->get_current_particle(point(x,y)).color);
            }
        }
        al_unlock_bitmap(al_get_target_bitmap());
    }
}
