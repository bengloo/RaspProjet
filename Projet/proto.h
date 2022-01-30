#ifndef __PROTO_H__
    #define __PROTO_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "session.h"
#include "data.h"
#include "reqRep.h"
#include "basic_func.h"


    
#define PORT_SVC 5000 
//#define SERVER
#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep(rep_t *rep,char * ch);
    void getPartiesRep(rep_t *rep,char * ch);
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ(short lg,buffer_t buff,struct sockaddr_in *clt,int sock);
    void getparties(short lg,buffer_t buff,struct sockaddr_in *clt,int sock);
    //1 fct de selection traitement selon requete
    void lireReqServ(req_t req,struct sockaddr_in *clt,int sock);
#endif
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq(req_t *req,char * Nom);
    void getPartiesReq();
    void joinPartieReq();
    void joinPartieRep();
    void startReq();
    void startRep();
    void UpdateStatutPlayerReq();
    void updateStatutPlayerRep();
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void waitParties();
    void afficherParties();
    void initPartie();
    void getStart();
    void partieMaitre();
    void partieAdverse();
    void updateStatutPlayerMaitre();
    void updateStatutPlayerInvite();
    void stream();
    void afficherStream();
    //1 fct de selection traitement selon requete
    void lireReqClient(req_t req);
#endif
#endif


