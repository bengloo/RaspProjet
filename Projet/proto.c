#include <netdb.h>
#include "proto.h"
#include "cltSrv.h"
#include "basic_func.h"

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
void newpartieServ(req_t *req)
{
    adresse_t clientMaitre;

    DEBUG_S("Nouvelle partie avant strTOadresse");
    strTOadresse(&clientMaitre, req->msgReq);

    DEBUG_S("Nouvelle partie avant mutex");
    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");

    statPartie_t *nouvellePartie = &listePartie[nbPartie];
    nouvellePartie->id = nbPartie;
    nouvellePartie->statut = WAITTINGADVERSE;
    strcpy(nouvellePartie->addrMaitre.ip, clientMaitre.ip);
    nouvellePartie->addrMaitre.port = clientMaitre.port;
    strcpy(nouvellePartie->addrMaitre.pseudo, clientMaitre.pseudo);
    nouvellePartie->addrAdverse.ip[0] = '\0';
    nouvellePartie->addrAdverse.port = 0;
    nouvellePartie->addrAdverse.pseudo[0] = '\0';
    nouvellePartie->scoreMaitre = 0;
    nouvellePartie->scoreAdverse = 0;

    DEBUG_S2("Nouvelle partie pour clientMaitre ip <%s> pseudo <%s>\n", nouvellePartie->addrMaitre.ip, nouvellePartie->addrMaitre.pseudo);

    nbPartie++;
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");
}

void afficherPartie(void)
{
    int i = 0;
    statPartie_t *partie;

    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");

    printf("Liste parties disponibles:\n");
    for (i = 0; i < nbPartie; i++)
    {
        partie = &listePartie[i];

        printf("%3d:%s:%15s:%5d:%20s:%15s:%5d:%20s:%5d:%5d\n",
               partie->id,
               statutPartieTxt[partie->statut],
               partie->addrMaitre.ip,
               partie->addrMaitre.port,
               partie->addrMaitre.pseudo,
               partie->addrAdverse.ip,
               partie->addrAdverse.port,
               partie->addrAdverse.pseudo,
               partie->scoreMaitre,
               partie->scoreAdverse);
    }
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");

}

void getparties(short lg, buffer_t buff, struct sockaddr_in *clt, int sock)
{
    DEBUG_S1("getpaties<%s>\n", buff);
};

//1 fct de selection traitement selon requete
//void lireReqServ(req_t req, struct sockaddr_in *clt, int sock)
void lireReqServ(int *sock)
{
    DEBUG_S1("Serveur : New thread pour socket <%i>\n", *sock);

    buffer_t msgLu;
    req_t req;
    int lenLu = 1;
    // On attend les inputs du Client Maitre

    while (lenLu > 0)
    {
        lenLu = lireMsgTCP(*sock, msgLu, sizeof(buffer_t));
        DEBUG_S1("Serveur : message reçu len <%d>\n", lenLu);
        strTOreq(&req, msgLu);
        DEBUG_S3("Serveur : socket <%i> msg recu <%s> avec idReq <%d>\n", *sock, msgLu, req.idReq);

        switch (req.idReq)
        {
        case 1:
            DEBUG_S("Case CreerPartie");
            newpartieServ(&req);
            afficherPartie();
            break;
        case 2:
            DEBUG_S("Case GetPartie");
            //getparties(req.lgreq, req.msgReq, clt, sock);
            break;

        default:
            DEBUG_S("Case Default");
            break;
        }
    }

    // Le Client a fermé on cloture la socket client
    fermerSocket(*sock);
}

// initialisation de la structure statPatie_t
void initstatPartie(void)
{
    nbPartie = 0;
}

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */

//-fct generation des requétes
void createPartyReq(int sock, char *pseudo)
{
    // On recupere notre adresse IP
    char hostbuffer[MAX_LEN];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    CHECK_T((hostname = gethostname(hostbuffer, sizeof(hostbuffer))) != -1, "Erreur gethostname");
    CHECK_T((host_entry = gethostbyname(hostbuffer)) != NULL, "Erreur gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr *)
                               host_entry->h_addr_list[0]));

    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = CREERPARTIE;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port = PORT_CLIENTMAITRE;
    adresseTOstr(&monAdr, req.msgReq);
    req.lgreq = strlen(req.msgReq);

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);

    ecrireMsgTCP(sock, reqTxt);
}

void getPartiesReq(int sock, char *pseudo){
    // On recupere notre adresse IP
    char hostbuffer[MAX_LEN];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    CHECK_T((hostname = gethostname(hostbuffer, sizeof(hostbuffer))) != -1, "Erreur gethostname");
    CHECK_T((host_entry = gethostbyname(hostbuffer)) != NULL, "Erreur gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr *)
                               host_entry->h_addr_list[0]));

    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = LISTERPARTIE;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port = PORT_CLIENTMAITRE;
    adresseTOstr(&monAdr, req.msgReq);
    req.lgreq = strlen(req.msgReq);

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);

    ecrireMsgTCP(sock, reqTxt);
};
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
