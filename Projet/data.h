#ifndef REQREP_H
    #define REQREP_H
    #include "reqRep.h"
#endif
#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h> 
#endif 
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
    char* ip;
    int port;
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



void reqTOstr(req_t *req,buffer_t buff);
void strTOreq(req_t *req,buffer_t buff);
void repTOstr(rep_t *req,buffer_t buff);
void strTOrep(rep_t *req,buffer_t buff);
