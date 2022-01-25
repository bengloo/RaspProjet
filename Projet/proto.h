#include <stdio.h>
#include <string.h>
#ifndef REQREP_H
    #define REQREP_H
    #include "reqRep.h"
#endif
#define ADDRSERVERENR "127.0.0.1"
#define PORT_SVC 5000 
//#define SERVER
#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep(short lg,buffer_t buff);
    void getPartiesRep(short lg,buffer_t buff);
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ(short lg,buffer_t buff);
    void getparties(short lg,buffer_t buff);
    //1 fct de selection traitement selon requete
    void lireReqServ(rep_t rep);
#endif
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq(req_t *req);
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

