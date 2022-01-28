#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdio.h>
#include <chrono>

using namespace std;

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

struct point{
    int x, y;
    point(int x, int y) :
    x(x),
    y(y){
    }

public:
    bool equals(point p){
        return p.x == x && p.y == y;
    }
    point left(){
        return point(x - 1, y);
    }
    point right(){
        return point(x + 1, y);
    }
    point above(){
        return point(x, y - 1);
    }
    point below(){
        return point(x, y + 1);
    }
};

const int targetFPS = 100;
float actualFPS;

const int screenWidth = 500;
const int screenHeight = 500;

unsigned char currentParticles[screenWidth][screenHeight];
unsigned char nextParticles[screenWidth][screenHeight];
unsigned char rngValues[screenWidth][screenHeight];
vector<point> changedParticles;

ALLEGRO_COLOR get_color(unsigned int x, unsigned int y){
    unsigned char id = currentParticles[x][y];
    char rng = rngValues[x][y];
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


void draw_map(bool redraw_all = false){
    if(!redraw_all){
        al_hold_bitmap_drawing(true);
        for(size_t i = 0; i < changedParticles.size(); i++){
            point p = changedParticles[i];
            al_draw_pixel(p.x,p.y, get_color(p.x,p.y));
        }
        al_hold_bitmap_drawing(false);
    } else {
        al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        for(unsigned int y = 0; y < screenHeight; y++){
            for(unsigned int x = 0; x < screenWidth; x++){
                al_put_pixel(x - 1,y - 1, get_color(x,y));
            }
        }
        al_unlock_bitmap(al_get_target_bitmap());
    }
    string fpsString = std::to_string(std::min((int)actualFPS,99));
    al_draw_filled_rectangle(1, 1, 16, 10, al_map_rgb(100, 100, 100));
    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_text(font, al_map_rgb(255, 255, 255), 1, 1, 0, fpsString.c_str());
    al_destroy_font(font);

    al_flip_display();
}

bool exists(point p){
    return p.x >= 0 && p.y >= 0 && p.x < screenWidth && p.y < screenHeight;
}
bool isAir(point p){
    return exists(p) && nextParticles[p.x][p.y] == 0;
}

void swap_particle(point start, point target){
    nextParticles[start.x][start.y] = nextParticles[target.x][target.y];
    nextParticles[target.x][target.y] = currentParticles[start.x][start.y];
    changedParticles.push_back(start);
    changedParticles.push_back(target);
}


void tick_sand(point p){
    int rng = rand() % 2;
    //Below
    if(exists(p.below())) {
        point t = p.below();
        if(nextParticles[p.x][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(t.left())){
                swap_particle(t, t.left());
            } else if(isAir(t.right())){
                swap_particle(t, t.right());
            } else if(isAir(t.below())){
                swap_particle(t, t.below());
            }
            swap_particle(p, t);
            return;
        }
    }
    //Below Left
    if(exists(p.below().left()) && rng == 0) {
        point t = p.below().left();
        if(nextParticles[p.x - 1][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x - 1][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(t.left())){
                swap_particle(t, t.left());
            } else if(isAir(t.right())){
                swap_particle(t, t.right());
            } else if(isAir(t.below())){
                swap_particle(t, t.below());
            }
            swap_particle(p, t);
            return;
        }
    }
    //Below Right
    if(exists(p.below().right()) && rng == 1) {
        point t = p.below().right();
        if(nextParticles[p.x + 1][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x + 1][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(t.left())){
                swap_particle(t, t.left());
            } else if(isAir(t.right())){
                swap_particle(t, t.right());
            } else if(isAir(t.below())){
                swap_particle(t, t.below());
            }
            swap_particle(p, t);
            return;
        }
    }
}
void tick_water(point p){
    int rng = rand() % 2;
    //Below
    if(isAir(p.below())) {
        swap_particle(p, p.below());
        return;
    }
    //Below Left
    if(isAir(p.below().left()) && rng == 0) {
        swap_particle(p, p.below().left());
        return;
    }
    //Below Right
    if(isAir(p.below().right()) && rng == 1) {
        swap_particle(p, p.below().right());
        return;
    }
    //Left
    if(isAir(p.left()) && rng == 0) {
        swap_particle(p, p.left());
        return;
    }
    //Right
    if(isAir(p.right()) && rng == 1) {
        swap_particle(p, p.right());
        return;
    }
}
void tick_particles(){
    changedParticles.clear();
    for(unsigned int x = 0; x < screenWidth; x++){
        for(unsigned int y = 0; y < screenHeight; y++){
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
    for(unsigned int x = 0; x < screenWidth; x++){
        for(unsigned int y = 0; y < screenWidth; y++){
            currentParticles[x][y] = nextParticles[x][y];
        }
    }
}


void fill_particles(char id, point top_left, point bottom_right){
    for(unsigned int x = top_left.x; x < bottom_right.x; x++){
        for(unsigned int y = top_left.y; y < bottom_right.y; y++){
            currentParticles[x][y] = id;
            nextParticles[x][y] = id;
        }
    }
}

int main()
{
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init(), "allegro");
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / (float)targetFPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(screenWidth, screenHeight);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    for(unsigned int x = 0; x < screenWidth; x++){
        for(unsigned int y = 0; y < screenWidth; y++){
            rngValues[x][y] = rand() % 255;
        }
    }

    fill_particles(3, point(100, 300), point(300, 310));
    fill_particles(2, point(130, 200), point(270, 250));

    draw_map(true);
    while(1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        al_wait_for_event(queue, &event);
        if(event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        if(redraw && al_is_event_queue_empty(queue))
        {
            //currentParticles[screenWidth / 4][0] = 1;
            //currentParticles[screenWidth / 2][0] = 1;
            //currentParticles[3 * screenWidth / 4][0] = 1;
            //currentParticles[145][0] = 1;
            //currentParticles[170][0] = 2;

            tick_particles();
            draw_map();
            redraw = false;
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            actualFPS = 1 / elapsed.count();
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}