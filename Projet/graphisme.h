#ifndef __GRAPHISME_H__
#define __GRAPHISME_H__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include <time.h>
#include <unistd.h>
#include "X11/keysym.h"
#include "basic_func.h"


#define X_PIX 500
#define Y_PIX 200
#define WIDTH 1
#define HEIGHT 0.7
#define Y_SCORE 6
#define X_SCORE 5
#define PIX_SCORE 4


typedef struct Vector{
	float x;
	float y;
	float z;
} vect;

typedef struct Vector2{
	float x;
	float y;
} vect2;

vect vect_scale(float  s, vect v) ;
vect vect_add(vect v1, vect v2) ;
vect vect_sub(vect v1, vect v2) ;

float vect_dot(vect v1, vect v2) ;
vect vect_cross(vect v1, vect v2) ;
void vect_print(vect v) ;
vect2 vect2_add(vect2 v1, vect2 v2);

vect2 vect2_sub(vect2 v1, vect2 v2);
vect2 vect2_scale(float a, vect2 v) ;
float vect2_dot(vect2 v1, vect2 v2) ;
void vect2_print(vect2 v) ;
int key_is_pressed(KeySym ks) ;
vect2 project_point(vect dir, vect point) ;
void put_point(vect2 projected, char c, char **picture) ;

void draw_point(vect dir, vect point, char c, char **picture) ;
void draw_line(vect dir, vect v_from, vect v_to, char c, char **picture) ;
void draw_ascii(char **picture);

char **empty_picture(char empty_char);

float random_float();
int min(int a, int b);
void partie(int * init_obstacles,int* mon_score,int*son_score,char **pic,time_t *top);
int *init_obstacles(int size);
#endif