#ifndef __DATA_H__
#define __DATA_H__
#include <stdio.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "reqRep.h"
#include "basic_func.h"
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
}adresse_t;

typedef enum{
    RUNNING = 1,
    STOPPED = 2,
    FAILED = 3,
    WAITTINGADVERSE = 4
} statutPatie_t;

typedef struct 
{
  int id;
  statutPatie_t statut;
  adresse_t addrMaitre;
  adresse_t addrAdverse;
  int scoreMaitre;
  int scoreAdverse;

} statPatie_t;


void adresseTOstr(adresse_t *adr,char *dest);

void reqTOstr(req_t *req,buffer_t buff);
void strTOreq(req_t *req,buffer_t buff);
void repTOstr(rep_t *req,buffer_t buff);
void strTOrep(rep_t *req,buffer_t buff);

#endif