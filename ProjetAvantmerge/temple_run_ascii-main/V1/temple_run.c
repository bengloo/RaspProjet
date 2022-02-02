
#include "graphisme.h"

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


#define PATH_WIDTH 1
#define Y_BORDER 0.7
#define SIGHT 10	// how far you can see (roughly)
#define GRAVITY 30
#define JUMP_SPEED 8
#define SPEED_INCREASE 0.1

int main(void) {
	START:
	srand(time(NULL));

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
		}
		else if (key_is_pressed(XK_Left)) {
			ypos = fmin(ypos + y_move_speed*tstep, Y_BORDER);
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
		}
		else if (zpos < 0) {
			zpos += duckspeed*tstep;
			if (zpos >= 0) {
				zpos = 0;
			}
		}
		// lost
		if (turn_dist < -2) {
			draw_ascii(empty_picture('O'));
			break;
		}
		// turn
		else if (turn_dist < 0) {
			if ((next_turn == -1 && key_is_pressed(XK_Right)) || (next_turn == 1 && key_is_pressed(XK_Left))) {
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
		printf("%d\n", i++);
		usleep(1000000*tstep);
	}

	// game finished
	for (int i = 0; i < 2; ++i) {
		draw_ascii(empty_picture(' '));
		usleep(150000);
		draw_ascii(empty_picture('X'));
		usleep(150000);
	}
	for (int i = 0; i < 200; ++i) {
		if (key_is_pressed(XK_Up) || key_is_pressed(XK_Down) || key_is_pressed(XK_Left) || key_is_pressed(XK_Right)) {
			goto START;
		}
		usleep(10000);
	}
	draw_ascii(empty_picture(' '));
}
