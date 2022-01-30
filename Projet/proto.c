#include "proto.h" 
#ifdef SERVER
    //-fct generation des requétes
    void createPartieRep(rep_t *rep,char * ch){

    };
    void getPartiesRep(rep_t *rep,char * ch){

    };
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void newpartieServ(short lg,buffer_t buff,struct sockaddr_in *clt,int sock){
        rep_t rep;
        createPartieRep(&rep,"1");/*TODO ajouté le client à la liste client du server d'enregistrement renvoyé en réponse 0 en cas d'echec (>max client ...) sinonn 1*/
        buffer_t buffrep;
        repTOstr(&rep,buffrep);
        ecrireMsgUDP(*clt, sock,buffrep); 
    };
    void getparties(short lg,buffer_t buff,struct sockaddr_in *clt,int sock){
         printf("getpaties<%s>\n",buff);
    };
    //1 fct de selection traitement selon requete
    void lireReqServ(req_t req,struct sockaddr_in *clt,int sock){
        switch (req.idReq)
        {
        case 1 :
            newpartieServ(req.lgreq,req.msgReq,clt,sock);
            break;
        case 2 :
            getparties(req.lgreq,req.msgReq,clt,sock);
            break;

        default:
            break;
        }
    }
#endif
//#define CLIENT
#ifdef CLIENT
    //-fct generation des requétes
    void createPartyReq(req_t *req,char *Nom){
        req->idReq=1;
        printf("test1\n");
        strcpy(req->msgReq,Nom);
        printf("test2\n");
        req->lgreq=strlen(Nom);
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
