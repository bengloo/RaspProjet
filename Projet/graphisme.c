/* ------------------------------------------------------------------------ */
/**
 *  \file       graphisme.c
 *  \brief      Programme réalisant un executable serveur d'enregistrement et un executable client
 *              permettant de gerer des parties temple_run multijoueurs 
 *				table de villes chargée à partir d'un fichier.
 *
 *  \author     Morcq Alexandre B2
 *
 *  \date       08/02/2022
 *
 *	\version    1.0
 * *
 *	\remark		fichier contenant nos fonctions, ...
 *
 */
#include "graphisme.h"
#include "proto.h"

typedef struct Vector
{
	float x;
	float y;
	float z;
} vect;

typedef struct Vector2
{
	float x;
	float y;
} vect2;

vect vect_scale(float s, vect v);
vect vect_add(vect v1, vect v2);
vect vect_sub(vect v1, vect v2);

float vect_dot(vect v1, vect v2);
vect vect_cross(vect v1, vect v2);
void vect_print(vect v);
vect2 vect2_add(vect2 v1, vect2 v2);

vect2 vect2_sub(vect2 v1, vect2 v2);
vect2 vect2_scale(float a, vect2 v);
float vect2_dot(vect2 v1, vect2 v2);
void vect2_print(vect2 v);
int key_is_pressed(KeySym ks);
vect2 project_point(vect dir, vect point);
void put_point(vect2 projected, char c, char **picture);

void draw_point(vect dir, vect point, char c, char **picture);
void draw_line(vect dir, vect v_from, vect v_to, char c, char **picture);

float random_float();
int min(int a, int b);


const char tabrefnum[10][Y_SCORE][X_SCORE] = {
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', '#', '#', ' '},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', '#', '#', ' ', '#'},
	 {' ', '#', ' ', '#', '#'},
	 {' ', ' ', '#', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', ' ', '#', ' '},
	 {' ', ' ', '#', '#', ' '},
	 {' ', '#', ' ', '#', ' '},
	 {' ', ' ', ' ', '#', ' '},
	 {' ', ' ', ' ', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', '#', '#', '#', ' '},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', ' ', ' ', '#', ' '},
	 {' ', ' ', '#', ' ', ' '},
	 {' ', '#', '#', '#', '#'}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', '#', '#', '#', ' '},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', ' '},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', '#', '#', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', ' ', '#', '#'},
	 {' ', ' ', '#', ' ', '#'},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', '#'},
	 {' ', ' ', ' ', ' ', '#'}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', '#', '#', '#', '#'},
	 {' ', '#', ' ', ' ', ' '},
	 {' ', '#', '#', '#', ' '},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', '#', '#', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', '#', '#', '#'},
	 {' ', '#', ' ', ' ', ' '},
	 {' ', '#', '#', '#', ' '},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', '#', '#', '#', '#'},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', ' ', ' ', '#', ' '},
	 {' ', ' ', '#', ' ', ' '},
	 {' ', ' ', '#', ' ', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', '#', '#', ' '},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', ' '},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', ' '}},
	{{' ', ' ', ' ', ' ', ' '},
	 {' ', ' ', '#', '#', ' '},
	 {' ', '#', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', '#'},
	 {' ', ' ', ' ', ' ', '#'},
	 {' ', ' ', '#', '#', ' '}}};

typedef struct
{
	int sockAdversaire;
	int *advScore;
	int jeuEnCours;
} threadArg_t;

vect vect_scale(float s, vect v)
{
	vect res = {s * v.x, s * v.y, s * v.z};
	return res;
}

vect vect_add(vect v1, vect v2)
{
	vect res;
	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return res;
}

vect vect_sub(vect v1, vect v2)
{
	vect v3 = vect_scale(-1, v2);
	return vect_add(v1, v3);
}

vect vect_normalize(vect v)
{
	float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return v;
}

float vect_dot(vect v1, vect v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vect vect_cross(vect v1, vect v2)
{
	return (vect){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

void vect_print(vect v)
{
	printf("x=%f, y=%f, z=%f\n", v.x, v.y, v.z);
}

vect2 vect2_add(vect2 v1, vect2 v2)
{
	return (vect2){v1.x + v2.x, v1.y + v2.y};
}

vect2 vect2_sub(vect2 v1, vect2 v2)
{
	return (vect2){v1.x - v2.x, v1.y - v2.y};
}

vect2 vect2_scale(float a, vect2 v)
{
	return (vect2){a * v.x, a * v.y};
}

float vect2_dot(vect2 v1, vect2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

void vect2_print(vect2 v)
{
	printf("%f %f\n", v.x, v.y);
}

int key_is_pressed(KeySym ks)
{
	Display *dpy = XOpenDisplay(0);
	char keys_return[32];
	XQueryKeymap(dpy, keys_return);
	KeyCode kc2 = XKeysymToKeycode(dpy, ks);
	int isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
	XCloseDisplay(dpy);
	return isPressed;
}

vect2 project_point(vect dir, vect point)
{
	float alpha = vect_dot(point, dir) / (1 + vect_dot(point, dir));
	// project onto plane orthogonal to dir through -dir
	vect p1 = vect_add(vect_scale(-alpha, dir), vect_scale(1 - alpha, point));
	// turn into 2 coordinates
	vect x = vect_normalize((vect){-dir.y, dir.x, 0});
	vect y = vect_cross(dir, x);
	float x_coord = vect_dot(p1, x);
	float y_coord = vect_dot(p1, y);
	return (vect2){x_coord, y_coord};
}

void put_point(vect2 projected, char c, char **picture)
{
	int x_coord = (int)((-projected.x + WIDTH) / (2 * WIDTH) * X_PIX);
	int y_coord = (int)((-projected.y + HEIGHT) / (2 * HEIGHT) * Y_PIX);
	if (x_coord < 0 || x_coord >= X_PIX || y_coord < 0 || y_coord >= Y_PIX)
	{
		return;
	}
	else
	{
		picture[y_coord][x_coord] = c;
	}
}

void draw_point(vect dir, vect point, char c, char **picture)
{
	// can't see points behind you
	if (vect_dot(dir, point) <= 0)
	{
		return;
	}
	vect2 projected = project_point(dir, point);
	put_point(projected, c, picture);
}

void draw_line(vect dir, vect v_from, vect v_to, char c, char **picture)
{
	// remove the part of the line that is behind the viewer
	float scale_from = vect_dot(v_from, dir);
	float scale_to = vect_dot(v_to, dir);
	if (scale_from < 0 && scale_to < 0)
	{
		return;
	}
	if (scale_from < 0)
	{
		scale_from = -scale_from;
		v_from = vect_add(vect_scale(scale_from / (scale_from + scale_to), v_to), vect_scale(scale_to / (scale_from + scale_to), v_from));
	}
	if (scale_to < 0)
	{
		scale_to = -scale_to;
		v_to = vect_add(vect_scale(scale_to / (scale_to + scale_from), v_from), vect_scale(scale_from / (scale_to + scale_from), v_to));
	}
	// now draw the line
	vect2 proj_from = project_point(dir, v_from);
	vect2 proj_to = project_point(dir, v_to);
	vect2 diff = vect2_sub(proj_to, proj_from);
	int steps = (X_PIX * (fabs(diff.x) / WIDTH) + Y_PIX * (fabs(diff.y) / HEIGHT)) / 2 + 1;
	vect2 step = vect2_scale(1.0 / steps, diff);
	for (int i = 0; i <= steps; ++i)
	{
		put_point(proj_from, c, picture);
		proj_from = vect2_add(proj_from, step);
	}
}

void draw_ascii(char **picture)
{
	printf("\033[0;0H"); // jump to position 0 0 to overwrite current picture
	for (int i = 0; i < Y_PIX; ++i)
	{
		for (int j = 0; j < X_PIX; ++j)
		{
			printf("%c", picture[i][j]);
		}
		printf("\n");
	}
}

void draw_ascii_score(char **picture, int s1, int s2)
{
	int s1chiffre[4];
	int s2chiffre[4];
	int yscore;
	s1chiffre[3] = s1 / 1000;
	s1chiffre[2] = (s1 - s1chiffre[3] * 1000) / 100;
	s1chiffre[1] = (s1 - s1chiffre[3] * 1000 - s1chiffre[2] * 100) / 10;
	s1chiffre[0] = (s1 - s1chiffre[3] * 1000 - s1chiffre[2] * 100 - s1chiffre[1] * 10);

	s2chiffre[3] = s2 / 1000;
	s2chiffre[2] = (s2 - s2chiffre[3] * 1000) / 100;
	s2chiffre[1] = (s2 - s2chiffre[3] * 1000 - s2chiffre[2] * 100) / 10;
	s2chiffre[0] = (s2 - s2chiffre[3] * 1000 - s2chiffre[2] * 100 - s2chiffre[1] * 10);
	//printf("avant\n");
	printf("\033[0;0H"); // jump to position 0 0 to overwrite current picture
	//printf("aprés\n");

	for (int i = 0; i < Y_PIX; ++i)
	{
		for (int j = 0; j < X_PIX; ++j)
		{
			if (picture[i][j] == 'o')
			{
				printf("%s", ANSI_CYAN);
			}
			if (picture[i][j] == 'x')
			{
				printf("%s", ANSI_WHITE);
			}
			printf("%c", picture[i][j]);
		}
		printf("%s", ANSI_BLUE);
		printf("####");
		printf("%s", ANSI_GREEN);
		//on affiche mon score
		if (i >= 60 && i < 60 + Y_SCORE * PIX_SCORE)
		{
			yscore = floor((i - 60) / PIX_SCORE); //coordoné dans la matrice de police
			//TODO écrire "Mon Score:"
			int flag = 0;
			for (int k = 3; k >= 0; k--)
			{
				if (s1chiffre[k] != 0 || flag == 1 || k == 0)
				{ //ne veux pas écrire les digit null inutil
					flag = 1;
					for (int l = 0; l < X_SCORE; l++)
					{
						for (int m = 0; m < PIX_SCORE * 2; m++)
						{
							printf("%c", tabrefnum[s1chiffre[k]][yscore][l]);
						}
					}
				}
				if (s1chiffre[k] != 0)
					flag = 1;
			}
		}
		printf("%s", ANSI_RED);
		//on affiche son score
		if (i >= 100 && i < 100 + Y_SCORE * PIX_SCORE)
		{
			yscore = floor((i - 100) / PIX_SCORE); //coordoné dans la matrice de police
			//TODO écrire "Mon Score:"
			int flag = 0;
			for (int k = 3; k >= 0; k--)
			{
				if (s2chiffre[k] != 0 || flag == 1 || k == 0)
				{ //ne veux pas écrire les digit null inutil
					flag = 1;
					for (int l = 0; l < X_SCORE; l++)
					{
						for (int m = 0; m < PIX_SCORE * 2; m++)
						{
							printf("%c", tabrefnum[s2chiffre[k]][yscore][l]);
						}
					}
				}
				if (s2chiffre[k] != 0)
					flag = 1;
			}
		}
		printf("%s", ANSI_RESET);
		printf("\n");
	}
}

char **empty_picture(char empty_char)
{
	char **pic;
	pic = malloc(sizeof(char *) * Y_PIX);
	for (int i = 0; i < Y_PIX; ++i)
	{
		pic[i] = malloc(sizeof(char *) * X_PIX);
		for (int j = 0; j < X_PIX; ++j)
		{
			pic[i][j] = empty_char;
		}
	}
	return pic;
}

float random_float()
{
	float r = (float)rand() / (float)RAND_MAX;
	return r;
}
int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

void majScoreAdverse(void *arg)
{

	threadArg_t *data = (threadArg_t *)arg;
	/*struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 50000000L;*/
	int score=1;
	while ((data->jeuEnCours) && (score >=0))
	{
		score = readScoreReq(data->sockAdversaire);
		if (score >=0) *(data->advScore) = score;
		//printf("Thread apres read <%d>\n",score);
		//nanosleep(&tim, NULL);
	}
}

void jouerPartie(partieGraphique_t *partie, int *mon_score, int *son_score, char **pic, time_t top, int sock)
{

	//START:
	// On change la resolution cf README
	system("./scriptZoom.sh -m");

	srand(time(NULL));

	vect dir = (vect){1, 0, 0};
	float speed = 3;
	float tstep = 0.03;

	// Init premiere ligne droite
	int turn_dist_orig = partie->dist[0];
	float turn_dist = turn_dist_orig;
	int next_turn_dist = partie->dist[1];
	int next_turn = partie->turn[0];
	int idxLigne = 0;
	int *obstacles = partie->obstacles;
	int *next_obstacles = &(partie->obstacles[next_turn_dist - 1]);

	float cam_height = 1;
	float y_move_speed = 3;
	float duckspeed = 4;
	float zpos = 0;
	float ypos = 0;
	float zspeed = 0;
	
	// Reset score
	*son_score=0;
	*mon_score=0;

	// main game loop
	int i = 0;
	long diff = top - time(NULL);
	if (diff > 9)
		diff = 9;
	//getchar();
	while (diff > 0)
	{
		printf("\033[0;0H"); // jump to position 0 0 to overwrite current picture
		for (i = 0; i < Y_PIX; ++i)
		{
			//on affiche le top.
			for (int m = 0; m < 200; m++)
			{
				printf(" ");
			}
			if (i >= 100 && i < 100 + Y_SCORE * PIX_SCORE)
			{
				int yscore = floor((i - 100) / PIX_SCORE); //coordoné dans la matrice de police
				for (int l = 0; l < X_SCORE; l++)
				{
					for (int m = 0; m < PIX_SCORE * 2; m++)
					{
						printf("%c", tabrefnum[diff][yscore][l]);
					}
				}
			}
			printf("\n");
		}
		diff = top - time(NULL);
		usleep(1000000 * tstep);
	}

	// On se met en ecoute pour le score
	threadArg_t data;
	data.sockAdversaire = sock;
	data.advScore = son_score;
	data.jeuEnCours = 1;
	pthread_t tid;
	// Si on a un adversaire on lance la MAJ du score
	if (sock != 0)
		CHECK_T(pthread_create(&tid, NULL, (pf_t)majScoreAdverse,
							   (void *)&data) == 0,
				"Erreur pthread_create()");

	//getchar();
	/*score i*/
	i = 0;
	while (1)
	{
		// keyboard stuff
		// move left/right
		if (key_is_pressed(XK_Right))
		{
			ypos = fmax(ypos - y_move_speed * tstep, -Y_BORDER);
		}
		else if (key_is_pressed(XK_Left))
		{
			ypos = fmin(ypos + y_move_speed * tstep, Y_BORDER);
		}
		else
		{
			if (ypos > 0)
			{
				ypos = fmax(0, ypos - y_move_speed * tstep);
			}
			else if (ypos < 0)
			{
				ypos = fmin(0, ypos + y_move_speed * tstep);
			}
		}
		// jump
		if (zpos == 0 && key_is_pressed(XK_Up))
		{
			// initiate jump
			zspeed = JUMP_SPEED;
		}
		zpos += zspeed * tstep;
		zspeed -= GRAVITY * tstep;
		if (zpos < 0)
		{
			zspeed = 0;
		}
		// duck
		if (key_is_pressed(XK_Down))
		{
			zpos -= duckspeed * tstep;
			zpos = fmax(zpos, -0.5);
		}
		else if (zpos < 0)
		{
			zpos += duckspeed * tstep;
			if (zpos >= 0)
			{
				zpos = 0;
			}
		}
		// lost
		if (turn_dist < -2)
		{
			draw_ascii(empty_picture('O'));
			break;
		}
		// turn
		else if (turn_dist < 0)
		{
			if ((next_turn == -1 && key_is_pressed(XK_Right)) || (next_turn == 1 && key_is_pressed(XK_Left)))
			{
				idxLigne++;
				if (idxLigne >= NBMAXLIGNES - 1)
					return; // GAGNER le joueur est aller au bout
				turn_dist = next_turn_dist;
				turn_dist_orig = next_turn_dist;
				next_turn = partie->turn[idxLigne];
				obstacles = next_obstacles;
				obstacles[next_turn_dist - 1] = 0;
				obstacles[next_turn_dist] = 0;
				next_obstacles = &(next_obstacles[next_turn_dist - 1]);
				next_obstacles[0] = 0;
				next_obstacles[1] = 0;
				next_turn_dist = partie->dist[idxLigne];
			}
		}
		pic = empty_picture(' ');
		for (float d = turn_dist; d > 0; d -= 1)
		{
			draw_line(dir, (vect){d, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){d, PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
		}

		// check for collision
		float real_pos = turn_dist_orig - turn_dist;
		int int_pos = trunc(real_pos);
		// printf("\n%f %f %d %f\n", next_turn_dist, real_pos, int_pos, fabs(int_pos-real_pos));
		if (fabs(int_pos - real_pos) < tstep * speed * 5)
		{
			// check for collision
			if (obstacles[int_pos] == 1 && ypos < 0.1)
			{
				break;
			}
			else if (obstacles[int_pos] == 2 && ypos > -0.1)
			{
				break;
			}
			else if (obstacles[int_pos] == 3 && zpos < 0.5)
			{
				break;
			}
			else if (obstacles[int_pos] == 4 && zpos > -0.2)
			{
				break;
			}
		}

		// draw path and next path
		if (turn_dist > SIGHT)
		{
			draw_line(dir, (vect){0, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){SIGHT, PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
			draw_line(dir, (vect){0, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){SIGHT, -PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
		}
		else
		{
			if (next_turn == -1)
			{
				draw_line(dir, (vect){0, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist + 2 * PATH_WIDTH, PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){0, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist, -PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist + 2 * PATH_WIDTH, PATH_WIDTH - ypos, -(cam_height + zpos)},
						  (vect){turn_dist + 2 * PATH_WIDTH, -SIGHT, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist, -SIGHT, -(cam_height + zpos)}, 'x', pic);
			}
			else
			{ // next_turn = 1
				draw_line(dir, (vect){0, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist + 2 * PATH_WIDTH, -PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){0, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist, PATH_WIDTH - ypos, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist + 2 * PATH_WIDTH, -PATH_WIDTH - ypos, -(cam_height + zpos)},
						  (vect){turn_dist + 2 * PATH_WIDTH, SIGHT, -(cam_height + zpos)}, 'x', pic);
				draw_line(dir, (vect){turn_dist, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){turn_dist, SIGHT, -(cam_height + zpos)}, 'x', pic);
			}
			for (float i = 1 - next_turn * ypos; i < SIGHT; i += 1)
			{
				draw_line(dir, (vect){turn_dist, i * next_turn, -(cam_height + zpos)},
						  (vect){turn_dist + 2 * PATH_WIDTH, i * next_turn, -(cam_height + zpos)}, 'x', pic);
			}
		}

		// draw obstacles
		for (int i = 0; i <= turn_dist; ++i)
		{
			int obst = obstacles[turn_dist_orig - i];
			float pos = turn_dist - i;
			if (obst == 1)
			{
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height + zpos)}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height + zpos)}, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
			}
			else if (obst == 2)
			{
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height + zpos)}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -ypos, -(cam_height + zpos)}, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, -ypos, -zpos + 0.5}, 'o', pic);
			}
			else if (obst == 3)
			{
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.5}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.5}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -(cam_height + zpos)}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.5}, 'o', pic);
			}
			else if (obst == 4)
			{
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.2}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.2}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.2}, 'o', pic);
				draw_line(dir, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, PATH_WIDTH - ypos, -zpos - 0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, -PATH_WIDTH - ypos, -zpos - 0.2}, 'o', pic);
				draw_line(dir, (vect){pos, PATH_WIDTH - ypos, -zpos + 0.5}, (vect){pos, -PATH_WIDTH - ypos, -zpos + 0.5}, 'o', pic);
			}
		}
		// draw next obstacles
		if (turn_dist < SIGHT)
		{
			for (int i = 0; i <= min(SIGHT, next_turn_dist); ++i)
			{
				int obst = next_obstacles[i];
				float obst_y = -ypos + (i + PATH_WIDTH) * next_turn;
				float right_x = turn_dist + (next_turn + 1) * PATH_WIDTH;
				float left_x = turn_dist + (next_turn * (-1) + 1) * PATH_WIDTH;
				if (obst == 1)
				{
					draw_line(dir, (vect){right_x, obst_y, -(cam_height + zpos)}, (vect){right_x, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -(cam_height + zpos)}, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos + 0.5}, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -(cam_height + zpos)}, (vect){right_x, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, (vect){right_x, obst_y, -(cam_height + zpos)}, 'o', pic);
				}
				else if (obst == 2)
				{
					draw_line(dir, (vect){left_x, obst_y, -(cam_height + zpos)}, (vect){left_x, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -(cam_height + zpos)}, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos + 0.5}, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -(cam_height + zpos)}, (vect){left_x, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){turn_dist + PATH_WIDTH, obst_y, -zpos + 0.5}, (vect){left_x, obst_y, -(cam_height + zpos)}, 'o', pic);
				}
				else if (obst == 3)
				{
					draw_line(dir, (vect){left_x, obst_y, -(cam_height + zpos)}, (vect){left_x, obst_y, -zpos - 0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -(cam_height + zpos)}, (vect){right_x, obst_y, -zpos - 0.5}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos - 0.5}, (vect){right_x, obst_y, -zpos - 0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -(cam_height + zpos)}, (vect){left_x, obst_y, -zpos - 0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos - 0.5}, (vect){left_x, obst_y, -(cam_height + zpos)}, 'o', pic);
				}
				else if (obst == 4)
				{
					draw_line(dir, (vect){left_x, obst_y, -zpos + 0.5}, (vect){left_x, obst_y, -zpos - 0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos + 0.5}, (vect){right_x, obst_y, -zpos - 0.2}, 'o', pic);
					draw_line(dir, (vect){left_x, obst_y, -zpos - 0.2}, (vect){right_x, obst_y, -zpos - 0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos + 0.5}, (vect){left_x, obst_y, -zpos - 0.2}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos - 0.2}, (vect){left_x, obst_y, -zpos + 0.5}, 'o', pic);
					draw_line(dir, (vect){right_x, obst_y, -zpos + 0.5}, (vect){left_x, obst_y, -zpos + 0.5}, 'o', pic);
				}
			}
		}
		turn_dist -= tstep * speed;
		// printf("%f %d \n", turn_dist, next_turn);
		// printf("%d\n", key_is_pressed(XK_Right));
		speed += SPEED_INCREASE * tstep;
		y_move_speed += SPEED_INCREASE * tstep * 0.5;
		duckspeed += SPEED_INCREASE * tstep * 0.5;
		i++;
		// printf("mon score:%d son score:%d\n", i++,*son_score);
		*mon_score = i;
		draw_ascii_score(pic, *mon_score, *son_score);
		updateScoreReq(sock, *mon_score);
		usleep(1000000 * tstep);
	}

	// On verifie si l'autre joueur continue de jouer en verifiant si le score change, la thread s'occupant des MAJ
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 100000000L;
	int score=*son_score-1;
	while (score < *son_score)
	{
		score = *son_score;
		draw_ascii_score(pic, *mon_score, *son_score);
		nanosleep(&tim, NULL);
	}
	
	// Fin update score
	data.jeuEnCours = 0;

	// game finished
	for (int i = 0; i < 2; ++i)
	{
		draw_ascii(empty_picture(' '));
		usleep(150000);
		draw_ascii(empty_picture('X'));
		usleep(150000);
	}
	for (int i = 0; i < 200; ++i)
	{
		if (key_is_pressed(XK_Up) || key_is_pressed(XK_Down) || key_is_pressed(XK_Left) || key_is_pressed(XK_Right))
		{
			//goto START;
		}
		usleep(10000);
	}
	draw_ascii(empty_picture(' '));

	// On remet  la resolution cf README
	system("./scriptZoom.sh -p");
};
void initPartieGraphisme(partieGraphique_t *partie)
{
	int i = 0;
	for (i = 0; i < NBMAXOBSTACLES; ++i)
	{
		partie->obstacles[i] = rand() % 5;
		if (partie->obstacles[i] != 0)
		{
			partie->obstacles[i + 1] = 0;
			partie->obstacles[i + 2] = 0;
			i += 2;
		}
	}
	partie->obstacles[0] = 0;
	partie->obstacles[1] = 0;
	partie->obstacles[NBMAXOBSTACLES - 1] = 0;

	for (i = 0; i < NBMAXLIGNES; ++i)
	{
		partie->dist[i] = LENMINLIGNES + rand() % (LENMAXLIGNES - LENMINLIGNES);
		partie->turn[i] = (rand() % 2) * 2 - 1;
	}
}