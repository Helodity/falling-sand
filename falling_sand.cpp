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
};

const int targetFPS = 50;
float actualFPS;

const int screenWidth = 500;
const int screenHeight = 500;

unsigned char currentParticles[screenWidth][screenHeight];
unsigned char nextParticles[screenWidth][screenHeight];
vector<point> changedParticles;

ALLEGRO_COLOR get_color(unsigned int x, unsigned int y){
    unsigned char id = currentParticles[x][y];
    switch (id) {
        case 1:
            return al_map_rgb(200, 128, 0);
        case 2:
            return al_map_rgb(20, 20, 200);
        case 3:
            return al_map_rgb(150, 150, 200);
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
                al_put_pixel(x,y, get_color(x,y));
            }
        }
        al_unlock_bitmap(al_get_target_bitmap());
    }
    al_draw_filled_rectangle(1,1,20,10, al_map_rgb(100, 100, 100));
    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_text(font, al_map_rgb(255, 255, 255), 1, 1, 0, std::to_string((int)actualFPS).c_str());
    al_destroy_font(font);

    al_flip_display();
}

bool exists(int x, int y){
    return x >= 0 && y >= 0 && x < screenWidth && y < screenHeight;
}
bool exists(point p){
    return p.x >= 0 && p.y >= 0 && p.x < screenWidth && p.y < screenHeight;
}
bool isAir(int x, int y){
    return exists(x,y) && nextParticles[x][y] == 0;
}
bool isAir(point p){
    return exists(p) && nextParticles[p.x][p.y] == 0;
}

point below(point p){
    return point(p.x, p.y + 1);
}
point left(point p){
    return point(p.x - 1, p.y);
}
point above(point p){
    return point(p.x, p.y - 1);
}
point right(point p){
    return point(p.x + 1, p.y);
}

void swap_particle(point start, point target){
    nextParticles[start.x][start.y] = nextParticles[target.x][target.y];
    nextParticles[target.x][target.y] = currentParticles[start.x][start.y];
    changedParticles.push_back(start);
    changedParticles.push_back(target);
}
void swap_particle(int start_x, int start_y, int target_x, int target_y){
    nextParticles[start_x][start_y] = nextParticles[target_x][target_y];
    nextParticles[target_x][target_y] = currentParticles[start_x][start_y];
    changedParticles.push_back(point(start_x, start_y));
    changedParticles.push_back(point(target_x, target_y));
}

void push_particle(int start_x, int start_y, int target_x, int target_y){

    nextParticles[start_x][start_y] = nextParticles[target_x][target_y];
    nextParticles[target_x][target_y] = currentParticles[start_x][start_y];;
    changedParticles.push_back(point(start_x, start_y));
    changedParticles.push_back(point(target_x, target_y));
}


void tick_sand(point p){
    int rng = rand() % 2;
    //Below
    if(exists(below(p))) {
        point t = below(p);
        if(nextParticles[p.x][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(left(t))){
                swap_particle(t, left(t));
            } else if(isAir(right(t))){
                swap_particle(t, right(t));
            } else if(isAir(below(t))){
                swap_particle(t, below(t));
            }
            swap_particle(p, t);
            return;
        }
    }
    //Below Left
    if(exists(below(left(p))) && rng == 0) {
        point t = below(left(p));
        if(nextParticles[p.x - 1][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x - 1][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(left(t))){
                swap_particle(t, left(t));
            } else if(isAir(right(t))){
                swap_particle(t, right(t));
            } else if(isAir(below(t))){
                swap_particle(t, below(t));
            }
            swap_particle(p, t);
            return;
        }
    }
    //Below Right
    if(exists(below(right(p))) && rng == 1) {
        point t = below(right(p));
        if(nextParticles[p.x + 1][p.y + 1] == 0) {
            swap_particle(p, t);
            return;
        }
        if(nextParticles[p.x + 1][p.y + 1] == 2) {
            //Try to push water before swapping
            if(isAir(left(t))){
                swap_particle(t, left(t));
            } else if(isAir(right(t))){
                swap_particle(t, right(t));
            } else if(isAir(below(t))){
                swap_particle(t, below(t));
            }
            swap_particle(p, t);
            return;
        }
    }
}
void tick_water(point p){
    int rng = rand() % 2;
    //Below
    if(isAir(below(p))) {
        swap_particle(p, below(p));
        return;
    }
    //Below Left
    if(isAir(below(left(p)))) {
        swap_particle(p, below(left(p)));
        return;
    }
    //Below Right
    if(isAir(below(right(p)))) {
        swap_particle(p, below(right(p)));
        return;
    }
    //Left
    if(isAir(left(p)) && rng == 0) {
        swap_particle(p, left(p));
        return;
    }
    //Right
    if(isAir(right(p)) && rng == 1) {
        swap_particle(p, right(p));
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
            currentParticles[screenWidth / 4][300] = 1;
            currentParticles[screenWidth / 2][300] = 1;
            currentParticles[3 * screenWidth / 4][300] = 1;
            currentParticles[145][300] = 1;
            currentParticles[170][350] = 2;

            //currentParticles[170][100] = 3;
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