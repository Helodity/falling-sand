#ifndef INPUT
#define INPUT

#include "point.h"
#include "global.h"

class input_struct
{
public:
	bool left_mouse_down;
	bool right_mouse_down;
	int scroll_wheel_change;
	point mouse_pos;
	vector<int> keycodes;

	input_struct();
};

class user_stuff{
public:
	int place_radius;
	char selected_id;

	user_stuff();
};
#endif