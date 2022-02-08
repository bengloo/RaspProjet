/* ------------------------------------------------------------------------ */
/**
 *  \file       reqRep.h
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
#ifndef __REQREP_H__
#define __REQREP_H__
#include "basic_func.h"
#define FMT_REQ "%3hd:%hd:%[^n]"
#define OK 1
#define NOP 0
typedef char buffer_t[MAX_BUFF];
//typedef char* buffer_t;

typedef enum
{
    // Status
    STATUT = 1,

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
#endif