#include "data.h"

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