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
#include "session.h"
#include "reqRep.h"
#include "basic_func.h"
#include "data.h"

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR    & CLIENT                                                  */
/* ------------------------------------------------------------------------ */

void afficherPartie(unsigned nbPart);

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */
#ifdef SERVER
// Initialisation
void initstatPartie(void);

//-fct generation des requétes
void createPartieRep(int sock, statPartie_t *partie);
//-à chaque req ,on associera &fct de traitement qui genere une réponse
int newpartieServ(int sock, req_t *req, int *idPart);
void getParties(int sock);
void updateStatutPartie(char *txt);
//1 fct de selection traitement selon requete
//void lireReqServ(req_t req,struct sockaddr_in *clt,int sock);
void lireReqServ(int *sock);
#endif

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */
#ifdef CLIENT

//-fct generation des requétes
int createPartyReq(int sock, char *pseudo);
int getPartiesReq(int sock);
int joinPartieReq(int masock, char *pseudo, partieGraphique_t *partie, time_t *top);
void joinPartieRep(int sock, partieGraphique_t *partie, time_t topdepart);
void updateStatutPlayerReq(int sock, int *monscore, int *sonscore);
void updateStatutPlayerRep(int sock, int *monscore, int *sonscore);
void updateScoreReq(int sock, int score);
int readScoreReq(int sock);

void updateStatutPartieReq(int sock, statPartie_t *statutpartie);
//-à chaque req ,on associera &fct de traitement qui genere une réponse
void waitParties();
void afficherParties();
void initPartie(int sock, adresse_t *adversaire);
void updateStatutPlayerMaitre();
void updateStatutPlayerInvite();
void stream();
void afficherStream(int sock, char *myPseudo);
void partieSolo(int sock, char *myPseudo);
//1 fct de selection traitement selon requete
//void lireReqClient(int *sock);
#endif

#endif
