#include <netdb.h>
#include "proto.h"

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */

//-fct generation des requétes
void createPartieRep(rep_t *rep, char *ch){

};
void getPartiesRep(rep_t *rep, char *ch){

};
//-à chaque req ,on associera &fct de traitement qui genere une réponse
//    void newpartieServ(short lg,buffer_t buff,struct sockaddr_in *clt,int sock){
void newpartieServ(int sock)
{
    DEBUG_S1("Serveur : New thread pour socket <%i>\n", sock);

    buffer_t msgLu;
    // On attend les inputs du Client Maitre
    lireMsgTCP(sock, msgLu, sizeof(buffer_t));
    DEBUG_S2("Serveur : socket <%i> msg recu <%s>\n", sock, msgLu);


    /*rep_t rep;
        createPartieRep(&rep,"1");//TODO ajouté le client à la liste client du server d'enregistrement renvoyé en réponse 0 en cas d'echec (>max client ...) sinonn 
    buffer_t buffrep;
    repTOstr(&rep, buffrep);
    ecrireMsgUDP(*clt, sock, buffrep);
    */
}

void getparties(short lg, buffer_t buff, struct sockaddr_in *clt, int sock)
{
    DEBUG_S1("getpaties<%s>\n", buff);
};

//1 fct de selection traitement selon requete
//void lireReqServ(req_t req, struct sockaddr_in *clt, int sock)
void lireReqServ(int *sock)
{
    int choix=1;
    switch (choix)
    {
    case 1:
        newpartieServ(*sock);
        break;
    case 2:
        //getparties(req.lgreq, req.msgReq, clt, sock);
        break;

    default:
        break;
    }
}

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */

//-fct generation des requétes
void createPartyReq(int sock,char * pseudo)
{
    // On recupere notre adresse IP
    char hostbuffer[MAX_LEN];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    CHECK_T((hostname = gethostname(hostbuffer, sizeof(hostbuffer)))!=-1, "Erreur gethostname");
    CHECK_T((host_entry = gethostbyname(hostbuffer))!=NULL, "Erreur gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0]));
  
    // On prepare la requete pour le serveur
    req_t req;
    req.idReq=CREERPARTIE;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port=PORT_CLIENTMAITRE;
    adresseTOstr(&monAdr,req.msgReq);
    req.lgreq=sizeof(adresse_t);

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req,reqTxt);

    ecrireMsgTCP(sock, reqTxt);
}


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
void lireReqClient(req_t req)
{
    switch (req.idReq)
    {
    case 1:
        waitParties();
        break;
    case 2:
        afficherParties();
        break;
    case 3:
        initPartie();
        break;
    case 4:
        getStart();
        break;
    case 5:
        partieMaitre();
        break;
    case 6:
        partieAdverse();
        break;
    case 7:
        updateStatutPlayerMaitre();
        break;
    case 8:
        updateStatutPlayerInvite();
        break;
    case 9:
        stream();
        break;
    case 10:
        afficherStream();
        break;
    default:
        break;
    }
}
