#ifndef REQREP_H
    #define REQREP_H
    #include "reqRep.h"
#endif
#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h> 
#endif   
//C6-representation data:
/*
    ->choisir un codage asci/EBCID/...
        (implicite)
    ->serialisation/déserialisation:network Order
        ntohs();
        htons();
        short<->networkOrder
*/



void reqTOstr(req_t *req,buffer_t buff);
void strTOreq(req_t *req,buffer_t buff);
void repTOstr(rep_t *req,buffer_t buff);
void strTOrep(rep_t *req,buffer_t buff);