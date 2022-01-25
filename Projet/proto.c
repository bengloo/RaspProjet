#include "proto.h"
//#define SERVER
#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep(short lg,buffer_t buff){};
    void getPartiesRep(short lg,buffer_t buff){};
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ(short lg,buffer_t buff){
        printf("newpartieserv:<%s>\n",buff);

    };
    void getparties(short lg,buffer_t buff){
         printf("getpaties<%s>\n",buff);
    };
    //1 fct de selection traitement selon requete
    void lireReqServ(rep_t rep){
        switch (rep.idRep)
        {
        case 1 :
            newpartieServ(rep.lgrep,rep.msgRep);
            break;
        case 2 :
            getparties(rep.lgrep,rep.msgRep);
            break;

        default:
            break;
        }
    }
#endif
//#define CLIENT
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq(req_t *req){
        req->idReq=1;
        req->msgReq="";
        req->lgreq=1;
    };
    void getPartiesReq(){};
    void joinPartieReq(){};
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
