struct point{
	int x, y;
public:
	point(int x, int y);
	bool equals(point p);
	point below();
	point left();
	point right();
	point above();
};