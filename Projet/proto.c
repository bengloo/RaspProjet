#include "proto.h"
void lireReqServ(req_t req){
    switch (req.idReq)
    {
    case 1 :
        newpartieServ();
        break;
    case 2 :
        getparties();
        break;

    default:
        break;
    }
}

void lireReqClient(req_t req){
    switch (req.idReq)
    {
    case 1 :
        waitParties();
        break;
    case 2 :
        afficherPartie();
        break;
    case 3 :
        initPartie();
        break;
    case 4 :
        partieMaitre();
        break;
    case 5 :
        partieInvite();
        break;
    case 6 :
        updateStatutPlayerMaitre();
        break;
    case 7 :
        updateStatutPlayerInvite();
        break;     
    default:
        break;
    }
}

void newpartieServ(){

}

void getparties(){

}

void waitParties(){

}

void afficherPartie(){

}

void initPartie(){

}

void partieMaitre(){

}

void partieInvite(){

}

void updateStatutPlayerMaitre(){

}

void updateStatutPlayerInvite(){

}
