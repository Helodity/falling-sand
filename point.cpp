#include "point.h"

point::point(int x, int y) :
x(x),
y(y){
}

bool point::equals(point p){
        return p.x == x && p.y == y;
    }
point point::left(){
    return point(x - 1, y);
}
point point::right(){
    return point(x + 1, y);
}
point point::above(){
    return point(x, y - 1);
}
point point::below(){
    return point(x, y + 1);
}