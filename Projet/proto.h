/* ------------------------------------------------------------------------ */
/**
 *  \file       proto.h
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
#ifndef __PROTO_H__
#define __PROTO_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "reqRep.h"
#include "basic_func.h"
#include "graphisme.h"



//-à chaque req ,on associera &fct de traitement qui genere une réponse
//1 fct de selection traitement selon requete

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */
#ifdef SERVER
//-fct generation des reponses
int serveurCreatePartieRep(int sock, req_t *req, int *idPart, statPartie_t *listePartie, unsigned *nbPartie, sem_t *mutex);
void serveurConnecterClientRep(int sock, req_t *req, listeClient_t *client);
void serveurGetPartiesRep(int sock, statPartie_t *listePartie, unsigned nbPartie, sem_t *mutex);
void updateStatutPartieRep(char *txt, statPartie_t *listePartie, unsigned nbPartie, sem_t *mutex);
#endif

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */
#ifdef CLIENT

//-fct generation des requétes vers serveur
int connecterClientReq(int sock, char *pseudo);
int createPartieReq(int sock, char *pseudo, int portClientMaitre, unsigned *idPartie);
int getPartiesReq(int sock, statPartie_t *listePartie);
void updateStatutPartieReq(int sock, statPartie_t *statutpartie);

//-fct generation des requétes vers client maitre
int joinPartieReq(int masock, char *pseudo, partieGraphique_t *partie, int *mon_score, int *son_score, time_t *top);
int readScoreReq(int sock);
void updateScoreReq(int sock, int score);

//-fct generation des reponses de client maitre vers adversaire
void joinPartieRep(int sock, partieGraphique_t *partie, time_t topdepart);
void initPartieRep(int sock, adresse_t *adversaire, int *mon_score, int *son_score);

//-à chaque req ,on associera &fct de traitement qui genere une réponse
#endif

#endif
