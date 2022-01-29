#ifndef POINT
#define POINT

struct point{
	int x, y;
public:
	point();
	point(int x, int y);
	bool equals(point p);
	point below();
	point left();
	point right();
	point above();
	bool operator==(const point& b){
		return equals(b);
	}
	point operator+(const point& b){
		return point(x + b.x, y + b.y);
	}
	point operator-(const point& b){
		return point(x - b.x, y - b.y);
	}
	point operator+=(const point& b){
		return point(x - b.x, y - b.y);
	}
};
#endif