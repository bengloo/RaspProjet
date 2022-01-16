#ifndef REQREP_H
    #define REQREP_H
    #include "reqRep.h"
#endif  
//-fct generation des requétes
void createPartyReq();
void createPartieRep();
void getPartiesReq();
void getPartiesRep();
void oinPartieReq();
void joinPartieRep();
void startReq();
void startRep();
void UpdateStatutPlayerReq();
void updateStatutPlayerRep();
//-à chaque req ,on associera &fct de traitement qui genere une réponse
void newpartieServ();
void getparties();
void waitParties();
void afficherPartie();
void initPartie();
void partieMaitre();
void partieInvite();
void updateStatutPlayerMaitre();
void updateStatutPlayerInvite();
//1 fct de selection traitement selon requete
void lireReqServ(req_t req);
void lireReqClient(req_t req);

