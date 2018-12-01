#pragma once /* prevent repeated "#inclusions" of this file. */
#include <deque>
using std::deque;

/* we will represent cubes by their center (x,y,z) and "radius" r.
 * see the readme for details. */
struct cube {
	double x,y,z,r;
	cube(double x0, double y0, double z0, double r0) :
		x(x0),y(y0),z(z0),r(r0) {};
	cube():x(0),y(0),z(-1.7),r(0.5) {};
};

/* the following function should apply the "fractalization" described
 * by mode to each cube in the deque M, and repeat this process for
 * d iterations (d for "depth").  If random == true, generate and use
 * a random mode. */
void mengerfy(deque<cube>& M, int mode, int d, bool random);
