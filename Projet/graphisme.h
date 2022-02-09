/* ------------------------------------------------------------------------ */
/**
 *  \file       graphisme.h
 *  \brief      Fichier description des fonctions de gestion du jeu graphique
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

/*  Parametre graphique */
#define PATH_WIDTH 1
#define Y_BORDER 0.7
#define SIGHT 10 // how far you can see (roughly)
#define GRAVITY 30
#define JUMP_SPEED 8
#define DELAY_START 3
#define SPEED_INCREASE 0.1
#define NBMAXLIGNES 100
#define LENMAXLIGNES 15
#define LENMINLIGNES 5
#define NBMAXOBSTACLES NBMAXLIGNES *LENMAXLIGNES
#define ANSI_RESET "\033[0m"
#define ANSI_BLACK "\033[30m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_PURPLE "\033[35m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"

typedef struct
{
	int obstacles[NBMAXOBSTACLES];
	int dist[NBMAXLIGNES];
	int turn[NBMAXLIGNES];
} partieGraphique_t;

void draw_ascii(char **picture);
char **empty_picture(char empty_char);
void jouerPartie(partieGraphique_t *partie, int *mon_score, int *son_score, char **pic, time_t top, int sock);
void initPartieGraphisme(partieGraphique_t *partie);

#endif