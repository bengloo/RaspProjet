/* ------------------------------------------------------------------------ */
/**
 *  \file       reqRep.h
 *  \brief      Fichier description de l'ensemble des strcutures requetes et reponses
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
#ifndef __REQREP_H__
#define __REQREP_H__
#include "basic_func.h"

#define FMT_REQ "%3hd:%hd:%[^n]"
#define FMT_REQSTR "%3d:%d:"
#define FMT_REPSTR "%d:%hd:"
#define FMT_REP "%d:%hd:%s"

#define OK 1
#define NOP 0
#define NBMAXCLIENT 100

typedef char buffer_t[MAX_BUFF];

typedef enum
{
    // Status
    STATUT = 1,

    //Client Maitre vers Serveur
    CONNECTERCLIENT = 5,

    //Client Maitre vers Serveur
    CREERPARTIE = 10,

    //Client Adversaire vers Serveur
    LISTERPARTIE = 20,

    //Client Adversaire vers Client Maitre
    JOIN = 30,

    //Client lecteur vers Client Maitre
    STREAM = 40,

    STARTPARTIE = 50,

    UPDATESCORE = 60,

    STATUTPARTIE = 70

} idRequeteServeur_t;

typedef struct
{
    idRequeteServeur_t idReq; //0 à 999
    short lgreq;              //0 à 1024
    buffer_t msgReq;
} req_t;

typedef struct
{
    idRequeteServeur_t idRep; //0 à 999
    short lgrep;              //0 à 1024
    buffer_t msgRep;
} rep_t;

typedef struct
{
    char ip[MAX_LEN];
    int port;
    char pseudo[MAX_LEN];
} adresse_t;

typedef struct
{
    int statut;
} statutReq_t;

typedef enum
{
    RUNNING = 0,
    STOPPED = 1,
    FAILED = 2,
    WAITTINGADVERSE = 3,
    CLOSED = 4
} statutPartie_t;

extern const char *statutPartieTxt[];

typedef struct
{
    int id;
    statutPartie_t statut;
    adresse_t addrMaitre;
    adresse_t addrAdverse;
    int scoreMaitre;
    int scoreAdverse;

} statPartie_t;

typedef enum
{
    OFFLINE = 0,
    ONLINE = 1
} statutClient_t;
typedef struct
{
    statutClient_t statut;
    int socket;
    int idx;
    char pseudo[MAX_LEN];
} listeClient_t;

#endif