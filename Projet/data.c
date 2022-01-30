#include "data.h"

void adresseTOstr(adresse_t *adr,char *dest)
{
        char ip[MAX_LEN];
    int port;
    char pseudo[MAX_LEN];
    sprintf(dest,"%s:%3d:%s",adr->ip, adr->port, adr->pseudo);
    DEBUG_S1("adresseTOstr <%s>\n", dest);
}

void reqTOstr(req_t *req,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",req->idReq,req->lgreq,req->msgReq);
}
void strTOreq(req_t *req,buffer_t buff){
    sscanf(buff,FMT_REQ,&req->idReq,&req->lgreq,req->msgReq);
}

void repTOstr(rep_t *rep,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",rep->idRep,rep->lgrep,rep->msgRep);
}
void strTOrep(rep_t *rep,buffer_t buff){
    sscanf(buff,FMT_REQ,&rep->idRep,&rep->lgrep,rep->msgRep);
}

