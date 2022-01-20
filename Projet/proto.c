#include "proto.h"

#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep(){};
    void getPartiesRep(){};
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ(){};
    void getparties(){};
    //1 fct de selection traitement selon requete
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
#endif
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq(){};
    void getPartiesReq(){};
    void oinPartieReq(){};
    void joinPartieRep(){};
    void startReq(){};
    void startRep(){};
    void UpdateStatutPlayerReq(){};
    void updateStatutPlayerRep(){};
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void waitParties(){};
    void afficherPartie(){};
    void initPartie(){};
    void partieMaitre(){};
    void partieInvite(){};
    void updateStatutPlayerMaitre(){};
    void updateStatutPlayerInvite(){};
    //1 fct de selection traitement selon requete
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
#endif
