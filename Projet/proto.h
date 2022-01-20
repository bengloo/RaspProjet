#ifndef REQREP_H
    #define REQREP_H
    #include "reqRep.h"
#endif  


#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep();
    void getPartiesRep();
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ();
    void getparties();
    //1 fct de selection traitement selon requete
    void lireReqServ(req_t req);
#endif
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq();
    void getPartiesReq();
    void joinPartieReq();
    void joinPartieRep();
    void startReq();
    void startRep();
    void UpdateStatutPlayerReq();
    void updateStatutPlayerRep();
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void waitParties();
    void afficherPartie();
    void initPartie();
    void partieMaitre();
    void partieInvite();
    void updateStatutPlayerMaitre();
    void updateStatutPlayerInvite();
    //1 fct de selection traitement selon requete
    void lireReqClient(req_t req);
#endif

