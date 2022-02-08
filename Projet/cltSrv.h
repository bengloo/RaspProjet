/* ------------------------------------------------------------------------ */
/**
 *  \file       cltSrv.h
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
#ifndef __CLTSRV_H__
#define __CLTSRV_H__
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include "data.h"
#include "session.h"
#include "proto.h"
#include "graphisme.h"
#include "basic_func.h"

void readParam(int argc, char const *argv[], int *portClientMaitre, char *ipServer);

void connecterServeur(void);
void connecterServeurPartie(adresse_t serverPartie);

// Gestion de signaux
void installDeroute(int numSig, void (*pfct)(int)); // deroute un signal vers la funct en 2ème param = pointeur sur une fontion
void deroute(int numSig);                           // exemple de fonction qui sera appelee en cas de signal
void terminerProcess(void);

#ifdef CLIENT
extern int sock;               // Numero Socket client vers serveur
extern int portClientMaitre;   // Port d'écoute
extern char ipServer[MAX_LEN]; // IP du Server
extern int mon_score;
extern int son_score;
extern unsigned idPartie;
void partieMaitre(int sock, char *myPseudo);
void partieAdverse(int sock, char *myPseudo);
int serverPartie(); //generre le server de partie de la meme manierre que le server d'enregistrement
#endif

#ifdef SERVER
extern int socketEcoute; // Numero Socket serveur
extern int continuer;
#endif

extern statPartie_t listePartie[NBMAXCLIENT]; // Liste des parties en cours
extern unsigned nbPartie;                     // nb de partie en cours

extern sem_t mutex;

#endif