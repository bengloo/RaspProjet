
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#ifndef PI
	#include <X11/Xlib.h>
	#include "X11/keysym.h"
#else
	extern int usleep (__useconds_t __useconds);
	#include <wiringPi.h>
	//#include "7seg_bp_ada.h"
	#define XK_Left 6
	#define XK_Right 24
	#define XK_Up 25
	#define XK_Down 23

	#define APPUYE LOW
	#define	BUZZER	1
	#define VIBRER 	2

#endif 

#ifdef DEBUG
	#define DEBUG_I(i) printf("%d\n", i);
	#define DEBUG_S(s) printf("%s\n", s);
	#define DEBUG_S1(s, p1) printf(s, p1);
	#define DEBUG_S2(s, p1, p2) printf(s, p1, p2);
	#define DEBUG_S3(s, p1, p2, p3) printf(s, p1, p2, p3);
	#define DEBUG_S4(s, p1, p2, p3, p4) printf(s, p1, p2, p3, p4);
#else
	#define DEBUG_I(i)
	#define DEBUG_S(s) (void)0;
	#define DEBUG_S1(s, p1) (void)0;
	#define DEBUG_S2(s, p1, p2) (void)0;
	#define DEBUG_S3(s, p1, p2, p3) (void)0;
	#define DEBUG_S4(s, p1, p2, p3, p4) (void)0;
#endif

#ifdef PI
	#define X_PIX 250
	#define Y_PIX 100
#else 
	#define X_PIX 500
	#define Y_PIX 200	
#endif

#define WIDTH 1
#define HEIGHT 0.7


typedef struct Vector{
	float x;
	float y;
	float z;
} vect;

typedef struct Vector2{
	float x;
	float y;
} vect2;

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

#ifndef PI
	int key_is_pressed(KeySym ks) {
		Display *dpy = XOpenDisplay(0);
		char keys_return[32];
		XQueryKeymap(dpy, keys_return);
		KeyCode kc2 = XKeysymToKeycode(dpy, ks);
		int isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
		XCloseDisplay(dpy);
		return isPressed;
	}
#else
	int key_is_pressed(int btn){
		return (digitalRead(btn) == APPUYE);
	}
#endif


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
	#ifndef UNDRAW
	printf("\033[0;0H");	// jump to position 0 0 to overwrite current picture
	for (int i = 0; i < Y_PIX; ++i) {
		
		/*for (int j = 0; j < X_PIX; ++j) {
			printf("%c", picture[i][j]);
		}
		printf("\n");*/
		printf("%s\n",picture[i]);
	}
	#else
		//printf("\033[0;0H");
		printf("appel de la fonction draw_ascci\n");
	#endif	
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

int *init_obstacles(int size) {
	/*
	0 = no obstacle
	1 = right thing
	2 = left thing
	3 = down thing
	4 = up thing
	*/
	int *res = calloc(size, sizeof(int));
	for (int i = 0; i < size; ++i) {
		res[i] = rand()%5;
		if (res[i] != 0) {
			i += 2;
		}
	}
	res[0] = 0;
	res[1] = 0;
	res[size-1] = 0;
	return res;
}

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

#ifndef PI
	#define PATH_WIDTH 1
	#define Y_BORDER 0.7
	#define SIGHT 10	// how far you can see (roughly)
	#define GRAVITY 30
	#define JUMP_SPEED 8
	#define SPEED_INCREASE 0.1
	#define TEMPO_END 150000
	#define TEMPO_FRAME 1000000
#else
	#define PATH_WIDTH 1
	#define Y_BORDER 0.7
	#define SIGHT 10	// how far you can see (roughly)
	#define GRAVITY 30
	#define JUMP_SPEED 8
	#define SPEED_INCREASE 0.1
	#define TEMPO_END 50000
	#define TEMPO_FRAME 1000000
#endif

int main(void) {
	START:
	srand(time(NULL));
	#ifndef PI
		vect dir = (vect) {1, 0, 0};
		float speed = 3;
		float tstep = 0.03;
		int turn_dist_orig = 5 + rand()%10;
		float turn_dist = turn_dist_orig;
		int next_turn_dist = 5 + rand()%10;
		// next_turn: -1 for right, 1 for left
		int next_turn = (rand()%2)*2 - 1;
		float cam_height = 1;
		float y_move_speed = 3;
		float duckspeed = 4;
		float zpos = 0;
		float ypos = 0;
		float zspeed = 0;
	#else
		vect dir = (vect) {1, 0, 0};
		float speed = 3;//3
		float tstep = 0.03;//0.03
		int turn_dist_orig = 5 + rand()%10;
		float turn_dist = turn_dist_orig;
		int next_turn_dist = 5 + rand()%10;
		// next_turn: -1 for right, 1 for left
		int next_turn = (rand()%2)*2 - 1;
		float cam_height = 1;
		float y_move_speed = 3;//3
		float duckspeed = 4;//4
		float zpos = 0;
		float ypos = 0;
		float zspeed = 0;

		//init  pin
		wiringPiSetup () ;
		pinMode (VIBRER, OUTPUT) ;
		pinMode (XK_Down, INPUT) ;
		pinMode (XK_Up, INPUT) ;
		pinMode (XK_Left, INPUT) ;
		pinMode (XK_Right, INPUT) ;
		pullUpDnControl (XK_Down,PUD_UP);
		pullUpDnControl (XK_Left,PUD_UP);
		pullUpDnControl (XK_Right,PUD_UP);
		pullUpDnControl (XK_Up,PUD_UP);
		/*
		//init 7segment
		int chiffre[4];
		int rc;
		HT16K33 led_backpack1 = HT16K33_INIT(1, HT16K33_ADDR_01);
		rc = HT16K33_OPEN(&led_backpack1);
		if(rc != 0) {
			//fprintf(stderr, "Error initializing HT16K33 led backpack (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
			HT16K33_CLOSE(&led_backpack1);
			return 1;
		}	
		rc = HT16K33_ON(&led_backpack1);
		if(rc != 0) {
			//fprintf(stderr, "Error putting the HT16K33 led backpack ON (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
			HT16K33_OFF(&led_backpack1);
			HT16K33_CLOSE(&led_backpack1);
			return 1;
		}
		// make it shining bright
		HT16K33_BRIGHTNESS(&led_backpack1, 0x0F);	
		// make it not blinking
		HT16K33_BLINK(&led_backpack1, HT16K33_BLINK_OFF);
		// power on the display
		HT16K33_DISPLAY(&led_backpack1, HT16K33_DISPLAY_ON);*/
	#endif

	int *obstacles = malloc(sizeof(int)*100);
	for (int i = 0; i < 100; ++i) {
		obstacles[i] = 0;
	}
	int *next_obstacles = init_obstacles(next_turn_dist+1);

	// main game loop
	int i = 0;
	while (1) {
		// keyboard stuff
		// move left/right
		if (key_is_pressed(XK_Right)) {
			ypos = fmax(ypos-y_move_speed*tstep, -Y_BORDER);
			DEBUG_S("Right pressed");

		}
		else if (key_is_pressed(XK_Left)) {
			ypos = fmin(ypos + y_move_speed*tstep, Y_BORDER);
			DEBUG_S("Left pressed");
		}
		else {
			if (ypos > 0) {
				ypos = fmax(0, ypos-y_move_speed*tstep);
			}
			else if (ypos < 0) {
				ypos = fmin(0, ypos + y_move_speed*tstep);
			}
		}
		// jump
		if (zpos == 0 && key_is_pressed(XK_Up)) {
			// initiate jump
			zspeed = JUMP_SPEED;
			DEBUG_S("up pressed");
		}
		zpos += zspeed*tstep;
		zspeed -= GRAVITY*tstep;
		if (zpos < 0) {
			zspeed = 0;
		}
		// duck
		if (key_is_pressed(XK_Down)) {
			zpos -= duckspeed*tstep;
			zpos = fmax(zpos, -0.5);
			DEBUG_S("Down pressed");
		}
		else if (zpos < 0) {
			zpos += duckspeed*tstep;
			if (zpos >= 0) {
				zpos = 0;
			}
		}
		// lost
		if (turn_dist < -2) {
			draw_ascii(empty_picture('-'));
			break;
		}
		// turn
		else if (turn_dist < 0) {
			if ((next_turn == -1 && key_is_pressed(XK_Right)) || (next_turn == 1 && key_is_pressed(XK_Left))) {
				DEBUG_S("Right or left pressed");
				turn_dist = next_turn_dist;
				turn_dist_orig = next_turn_dist;
				next_turn_dist = 5 + rand()%10;
				next_turn = (rand()%2)*2 - 1;
				obstacles = next_obstacles;
				next_obstacles = init_obstacles(next_turn_dist+1);
			}
		}
		char **pic = empty_picture(' ');
		for (float d = turn_dist; d > 0; d -= 1) {
			draw_line(dir, (vect){d, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){d, PATH_WIDTH-ypos, -(cam_height+zpos)}, 'o', pic);
		}

		// check for collision
		float real_pos = turn_dist_orig - turn_dist;
		int int_pos = trunc(real_pos);
		// printf("\n%f %f %d %f\n", next_turn_dist, real_pos, int_pos, fabs(int_pos-real_pos));
		if (fabs(int_pos - real_pos) < tstep*speed*5) {
			// check for collision
			if (obstacles[int_pos] == 1 && ypos < 0.1) {
				break;
			}
			else if (obstacles[int_pos] == 2 && ypos > -0.1) {
				break;
			}
			else if (obstacles[int_pos] == 3 && zpos < 0.5) {
				break;
			}
			else if (obstacles[int_pos] == 4 && zpos > -0.2) {
				break;
			}
		}

		// draw path and next path
		if (turn_dist > SIGHT) {
			draw_line(dir, (vect){0, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){SIGHT, PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
			draw_line(dir, (vect){0, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){SIGHT, -PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
		}
		else {
			if (next_turn == -1) {
				draw_line(dir, (vect){0, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist+2*PATH_WIDTH, PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir, (vect){0, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist, -PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir,  (vect){turn_dist+2*PATH_WIDTH, PATH_WIDTH-ypos, -(cam_height+zpos)}, 
								(vect){turn_dist+2*PATH_WIDTH, -SIGHT, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist, -SIGHT, -(cam_height+zpos)}, 'x', pic);
			}
			else {	// next_turn = 1
				draw_line(dir, (vect){0, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist+2*PATH_WIDTH, -PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir, (vect){0, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist, PATH_WIDTH-ypos, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir,  (vect){turn_dist+2*PATH_WIDTH, -PATH_WIDTH-ypos, -(cam_height+zpos)}, 
								(vect){turn_dist+2*PATH_WIDTH, SIGHT, -(cam_height+zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){turn_dist, SIGHT, -(cam_height+zpos)}, 'x', pic);
			}
			for (float i = 1-next_turn*ypos; i < SIGHT; i += 1) {
				draw_line(dir,  (vect){turn_dist, i*next_turn, -(cam_height+zpos)}, 
								(vect){turn_dist+2*PATH_WIDTH, i*next_turn, -(cam_height+zpos)}, 'x', pic);
			}
		}

		// draw obstacles
		for (int i = 0; i <= turn_dist; ++i) {
			int obst = obstacles[turn_dist_orig - i];
			float pos = turn_dist - i;
			if (obst == 1) {
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height+zpos)}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height+zpos)}, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);

			}
			else if (obst == 2) {
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height+zpos)}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height+zpos)}, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, -ypos, -zpos+0.5}, 'o', pic);
			}
			else if (obst == 3) {
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, PATH_WIDTH-ypos, -zpos-0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos-0.5}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, PATH_WIDTH-ypos, -zpos-0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -(cam_height+zpos)}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.5}, 'o', pic);				
			}
			else if (obst == 4) {
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, PATH_WIDTH-ypos, -zpos-0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos-0.2}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.2}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.2}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, PATH_WIDTH-ypos, -zpos-0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, -PATH_WIDTH-ypos, -zpos-0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH-ypos, -zpos+0.5}, (vect){pos, -PATH_WIDTH-ypos, -zpos+0.5}, 'o', pic);			
			}
		}
		// draw next obstacles
		if (turn_dist < SIGHT) {
			for (int i = 0; i <= min(SIGHT, next_turn_dist); ++i) {
				int obst = next_obstacles[i];
				float obst_y = -ypos+(i+PATH_WIDTH)*next_turn;
				float right_x = turn_dist + (next_turn+1)*PATH_WIDTH;
				float left_x = turn_dist + (next_turn*(-1)+1)*PATH_WIDTH;
				if (obst == 1) {
					draw_line(dir, (vect){right_x, obst_y, -(cam_height+zpos)}, (vect){right_x, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -(cam_height+zpos)}, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos+0.5}, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -(cam_height+zpos)},(vect){right_x, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, (vect){right_x, obst_y, -(cam_height+zpos)}, 'o', pic);

				}
				else if (obst == 2) {
					draw_line(dir, (vect){left_x, obst_y, -(cam_height+zpos)}, (vect){left_x, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -(cam_height+zpos)}, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos+0.5}, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -(cam_height+zpos)},(vect){left_x, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist+PATH_WIDTH, obst_y, -zpos+0.5}, (vect){left_x, obst_y, -(cam_height+zpos)}, 'o', pic);
				}
				else if (obst == 3) {
					draw_line(dir, (vect){left_x, obst_y, -(cam_height+zpos)}, (vect){left_x, obst_y, -zpos-0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -(cam_height+zpos)}, (vect){right_x, obst_y, -zpos-0.5}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos-0.5}, (vect){right_x, obst_y, -zpos-0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -(cam_height+zpos)},(vect){left_x, obst_y, -zpos-0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos-0.5}, (vect){left_x, obst_y, -(cam_height+zpos)}, 'o', pic);
				}
				else if (obst == 4) {
					draw_line(dir, (vect){left_x, obst_y, -zpos+0.5}, (vect){left_x, obst_y, -zpos-0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos+0.5}, (vect){right_x, obst_y, -zpos-0.2}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos-0.2}, (vect){right_x, obst_y, -zpos-0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos+0.5},(vect){left_x, obst_y, -zpos-0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos-0.2}, (vect){left_x, obst_y, -zpos+0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos+0.5}, (vect){left_x, obst_y, -zpos+0.5}, 'o', pic);
				}
			}
		}
		turn_dist -= tstep*speed;	
		draw_ascii(pic);
		// printf("%f %d \n", turn_dist, next_turn);
		// printf("%d\n", key_is_pressed(XK_Right));
		speed += SPEED_INCREASE*tstep;
		y_move_speed += SPEED_INCREASE*tstep*0.5;
		duckspeed += SPEED_INCREASE*tstep*0.5;
		#ifndef PI
			printf("%d\n", i++);
		#else
			i++;
			/*chiffre[3] = i / 1000;
			chiffre[2] = (i - chiffre[3] * 1000) / 100;
			chiffre[1] = (i - chiffre[3] * 1000 - chiffre[2] * 100) / 10;
			chiffre[0] = (i - chiffre[3] * 1000 - chiffre[2] * 100 - chiffre[1] * 10);
			HT16K33_UPDATE_DIGIT(&led_backpack1, 0, 48+chiffre[3], 0);
			HT16K33_UPDATE_DIGIT(&led_backpack1, 1, 48+chiffre[2], 0);
			HT16K33_UPDATE_DIGIT(&led_backpack1, 2, 48+chiffre[1], 0);
			HT16K33_UPDATE_DIGIT(&led_backpack1, 3, 48+chiffre[0], 0);
			HT16K33_UPDATE_DIGIT(&led_backpack1, 4, 48+chiffre[3], 0);
			HT16K33_COMMIT(&led_backpack1);*/
		#endif
		usleep(TEMPO_FRAME*tstep);
	}

	// game finished
	#ifdef PI
		digitalWrite (VIBRER, HIGH) ;
	#endif
	for (int i = 0; i < 2; ++i) {
		draw_ascii(empty_picture(' '));
		usleep(TEMPO_END);
		draw_ascii(empty_picture('X'));
		usleep(TEMPO_END);

	}
	#ifdef PI
		digitalWrite (VIBRER, LOW) ;
	#endif
	//redemarage automatique si clé préssé
	for (int i = 0; i < 200; ++i) {
		if (key_is_pressed(XK_Up) || key_is_pressed(XK_Down) || key_is_pressed(XK_Left) || key_is_pressed(XK_Right)) {
			goto START;
		}
		usleep(10000);
	}
	draw_ascii(empty_picture(' '));
	#ifdef PI
		//HT16K33_CLOSE(&led_backpack1);
	#endif
}
