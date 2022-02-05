#include "data.h"
const char *statutPartieTxt[]={"RUN ", "STOP", "FAIL", "WAIT"};

void adresseTOstr(adresse_t *adr,char *dest)
{
    sprintf(dest,"%s:%d:%s",adr->ip, adr->port, adr->pseudo);
}

void strTOadresse(adresse_t *adr,char *dest)
{
    sscanf(dest,"%[^:]:%d:%[^:]",adr->ip, &adr->port, adr->pseudo);
}

void reqTOstr(req_t *req,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",req->idReq,req->lgreq,req->msgReq);
}
void strTOreq(req_t *req,buffer_t buff){
    sscanf(buff,FMT_REQ,(short int*)&req->idReq,&req->lgreq,req->msgReq);
}

void repTOstr(rep_t *rep,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",rep->idRep,rep->lgrep,rep->msgRep);
}
void strTOrep(rep_t *rep,buffer_t buff){
    sscanf(buff,FMT_REQ,&rep->idRep,&rep->lgrep,rep->msgRep);
}

