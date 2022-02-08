/* ------------------------------------------------------------------------ */
/**
 *  \file       data.h
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
#ifndef __DATA_H__
#define __DATA_H__
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "reqRep.h"
#include "basic_func.h"
#include "graphisme.h"

void adresseTOstr(adresse_t *adr, char *dest);
void strTOadresse(adresse_t *adr, char *dest);

void statutReqTOstr(statutReq_t *adr, char *dest);
void strTOstatutReq(statutReq_t *adr, char *dest);

void listePartieTOStr(statPartie_t *listePartie, unsigned nbPartie, char *dest);
int StrTOlistePartie(statPartie_t *listePartie, char *dest);

void reqTOstr(req_t *req, buffer_t buff);
void strTOreq(req_t *req, buffer_t buff);
void repTOstr(rep_t *rep, buffer_t buff);
void strTOrep(rep_t *rep, buffer_t buff);

/*fonction caste des data imbriqué*/
void obstTOstring(char *dest, const int *obstacles);
void stringTOobst(int *dest, const char *obstaclesch);
void timeTostring(char *timeDataRep, time_t temps);
void stringToTime(time_t *temps, char *timeDataRep);
void initPartiTOString(char *dataTxt, time_t temps, partieGraphique_t *partie);
void StringinitTOParti(time_t *temps, partieGraphique_t *partie, char *dataTxt);

void scoreTOstr(char *dest, int score);
void strTOscore(char *dest, int *score);

void partieTOstr(char *dest, statPartie_t *partie);
void strTOpartie(statPartie_t *partie, char *datatxt);
#endif