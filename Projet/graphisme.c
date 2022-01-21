/*#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include "X11/keysym.h"
#include <time.h>
#include <unistd.h>*/

#include "graphisme.h"

vect vect_scale(float  s, vect v) {
	vect res = {s*v.x, s*v.y, s*v.z};
	return res;
}

vect vect_add(vect v1, vect v2) {
	vect res;
	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return res;
}

vect vect_sub(vect v1, vect v2) {
	vect v3 = vect_scale(-1, v2);
	return vect_add(v1, v3);
}


vect vect_normalize(vect v) {
	float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return v;
}

float vect_dot(vect v1, vect v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

vect vect_cross(vect v1, vect v2) {
	return (vect) {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

void vect_print(vect v) {
	printf("x=%f, y=%f, z=%f\n", v.x, v.y, v.z);
}

vect2 vect2_add(vect2 v1, vect2 v2) {
	return (vect2) {v1.x+v2.x, v1.y+v2.y};
}

vect2 vect2_sub(vect2 v1, vect2 v2) {
	return (vect2) {v1.x-v2.x, v1.y-v2.y};
}

vect2 vect2_scale(float a, vect2 v) {
	return (vect2) {a*v.x, a*v.y};
}

float vect2_dot(vect2 v1, vect2 v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

void vect2_print(vect2 v) {
	printf("%f %f\n", v.x, v.y);
}


int key_is_pressed(KeySym ks) {
    Display *dpy = XOpenDisplay(0);
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    int isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    XCloseDisplay(dpy);
    return isPressed;
}


vect2 project_point(vect dir, vect point) {
	float alpha = vect_dot(point, dir) / (1 + vect_dot(point, dir));
	// project onto plane orthogonal to dir through -dir
	vect p1 = vect_add(vect_scale(-alpha, dir), vect_scale(1-alpha, point));
	// turn into 2 coordinates
	vect x = vect_normalize((vect) {-dir.y, dir.x, 0});
	vect y = vect_cross(dir, x);
	float x_coord = vect_dot(p1, x);
	float y_coord = vect_dot(p1, y);
	return (vect2) {x_coord, y_coord};
}


void put_point(vect2 projected, char c, char **picture) {
	int x_coord = (int) ((-projected.x + WIDTH)/(2*WIDTH)*X_PIX);
	int y_coord = (int) ((-projected.y + HEIGHT)/(2*HEIGHT)*Y_PIX);
	if (x_coord < 0 || x_coord >= X_PIX || y_coord < 0 || y_coord >= Y_PIX) {
		return;
	}
	else {
		picture[y_coord][x_coord] = c;
	}
}

void draw_point(vect dir, vect point, char c, char **picture) {
	// can't see points behind you
	if (vect_dot(dir, point) <= 0) {
		return;
	}
	vect2 projected = project_point(dir, point);
	put_point(projected, c, picture);
}

void draw_line(vect dir, vect v_from, vect v_to, char c, char **picture) {
	// remove the part of the line that is behind the viewer
	float scale_from = vect_dot(v_from, dir);
	float scale_to = vect_dot(v_to, dir);
	if (scale_from < 0 && scale_to < 0) {
		return;
	}
	if (scale_from < 0) {
		scale_from = -scale_from;
		v_from = vect_add(vect_scale(scale_from/(scale_from+scale_to), v_to), vect_scale(scale_to/(scale_from+scale_to), v_from));
	}
	if (scale_to < 0) {
		scale_to = -scale_to;
		v_to = vect_add(vect_scale(scale_to/(scale_to+scale_from), v_from), vect_scale(scale_from/(scale_to+scale_from), v_to));
	}
	// now draw the line
	vect2 proj_from = project_point(dir, v_from);
	vect2 proj_to = project_point(dir, v_to);
	vect2 diff = vect2_sub(proj_to, proj_from);
	int steps = (X_PIX * (fabs(diff.x)/WIDTH) + Y_PIX * (fabs(diff.y)/HEIGHT))/2 + 1;
	vect2 step = vect2_scale(1.0/steps, diff);
	for (int i = 0; i <= steps; ++i) {
		put_point(proj_from, c, picture);
		proj_from = vect2_add(proj_from, step);
	}
}


void draw_ascii(char **picture) {
	printf("\033[0;0H");	// jump to position 0 0 to overwrite current picture
	for (int i = 0; i < Y_PIX; ++i) {
		for (int j = 0; j < X_PIX; ++j) {
			printf("%c", picture[i][j]);
		}
		printf("\n");
	}	
}

char **empty_picture(char empty_char) {
	char **pic;
	pic = malloc(sizeof(char *) * Y_PIX);
	for (int i = 0; i < Y_PIX; ++i) {
		pic[i] = malloc(sizeof(char *) * X_PIX);
		for (int j = 0; j < X_PIX; ++j) {
			pic[i][j] = empty_char;
		}
	}
	return pic;
}

float random_float()
{
    float r = (float)rand()/(float)RAND_MAX;
    return r;
}