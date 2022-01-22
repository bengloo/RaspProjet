#include "proto.h"
#include <string.h>

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
    void createPartyReq(){
        req_t req;
        buffer_t buff;
        req.idReq=1;
        req.msgReq="";
        //reqTOstr(&req,buff);
        req.lgreq=strlen(buff);
    };
    void getPartiesReq(){};
    void oinPartieReq(){};
    void joinPartieRep(){};
    void startReq(){};
    void startRep(){};
    void UpdateStatutPlayerReq(){};
    void updateStatutPlayerRep(){};
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void waitParties(){};
    void afficherParties(){};
    void initPartie(){};
    void getStart(){};
    void partieMaitre(){};
    void partieAdverse(){};
    void updateStatutPlayerMaitre(){};
    void updateStatutPlayerInvite(){};
    void stream(){};
    void afficherStream(){};
    //1 fct de selection traitement selon requete
    void lireReqClient(req_t req){
        switch (req.idReq)
        {
        case 1 :
            waitParties();
            break;
        case 2 :
            afficherParties();
            break;
        case 3 :
            initPartie();
            break;
        case 4 :
            getStart();
            break;
        case 5 :
            partieMaitre();
            break;
        case 6 :
            partieAdverse();
            break;
        case 7 :
            updateStatutPlayerMaitre();
            break;
        case 8 :
            updateStatutPlayerInvite();
            break;
        case 9 :
            stream();
            break; 
        case 10 :
            afficherStream();
            break;      
        default:
            break;
        }
    }
#endif
