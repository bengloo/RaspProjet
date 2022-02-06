#ifndef __DATA_H__
#define __DATA_H__
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "reqRep.h"
#include "basic_func.h"
#include "graphisme.h"


//C6-representation data:
/*
    ->choisir un codage asci/EBCID/...
        (implicite)
    ->serialisation/déserialisation:network Order
        ntohs();
        htons();
        short<->networkOrder
*/
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
    RUNNING = 4,
    STOPPED = 1,
    FAILED = 2,
    WAITTINGADVERSE = 3
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

void adresseTOstr(adresse_t *adr, char *dest);
void strTOadresse(adresse_t *adr, char *dest);

void statutReqTOstr(statutReq_t *adr, char *dest);
void strTOstatutReq(statutReq_t *adr, char *dest);

void listePartieTOStr(statPartie_t *listePartie, char *dest);
void StrTOlistePartie(statPartie_t *listePartie, char *dest);

void reqTOstr(req_t *req, buffer_t buff);
void strTOreq(req_t *req, buffer_t buff);
void repTOstr(rep_t *rep, buffer_t buff);
void strTOrep(rep_t *rep, buffer_t buff);

/*fonction caste des data imbriqué*/
void obstTOstring(char *dest, const int *obstacles);
void stringTOobst(int *dest, const char *obstaclesch);
void strutToString(statPartie_t *tableau, char ch[300]);
void stringToStruct(statPartie_t *tableau2, char ch[300]);
void timeTostring(char *timeDataRep, time_t temps);
void stringToTime(time_t *temps, char *timeDataRep);
void initPartiTOString(char *dataTxt, time_t temps, partieGraphique_t *partie);
void StringinitTOParti(time_t *temps, partieGraphique_t *partie, char *dataTxt);

void scoreTOstr(char*ch,int monscore,int sonscore);
void strTOscore(char*dest,int* monscore,int* sonscore);

void partieTOstr(char *dest,statPartie_t partie);
void strTOpartie(statPartie_t *partie, char *datatxt);
#endif