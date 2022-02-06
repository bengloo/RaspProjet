#include <netdb.h>
#include "proto.h"
#include "cltSrv.h"
#include "basic_func.h"
#include "data.h"
#include "graphisme.h"

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR    & CLIENT                                                  */
/* ------------------------------------------------------------------------ */

void afficherPartie(void)
{
    int i = 0;
    statPartie_t *partie;

    printf("Liste parties disponibles:\n");
#ifdef SERVER
    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");
#endif
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
#ifdef SERVER
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");
#endif
}

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */
#ifdef SERVER

///-à chaque req ,on associera &fct de traitement qui genere une réponse
//    void newpartieServ(short lg,buffer_t buff,struct sockaddr_in *clt,int sock){
//-fct generation des requétes
void createPartieRep(int sock, int status)
{
    rep_t rep;
    rep.idRep = STATUT;
    statutReq_t statut;
    statut.statut = status;
    statutReqTOstr(&statut, rep.msgRep);
    rep.lgrep = strlen(rep.msgRep);

    // Envoie de la requete au serveur
    char repTxt[sizeof(rep_t)];
    repTOstr(&rep, repTxt);

    ecrireMsgTCP(sock, repTxt);
};
int newpartieServ(int sock, req_t *req)
{
    // Peut on encore accueillir des clients
    if (nbPartie == NBMAXCLIENT)
    {
        createPartieRep(sock, NOT_OK);
        return NOT_OK;
    }

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

    createPartieRep(sock, OK);
    return OK;
}

void getParties(int sock)
{
    DEBUG_S("Debut getParties\n");
    rep_t rep;
    rep.idRep = LISTERPARTIE;

    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");
    listePartieTOStr(listePartie, rep.msgRep);
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");
    rep.lgrep = strlen(rep.msgRep);
    DEBUG_S1("getParties msgRep=<%s>\n", rep.msgRep);

    // Envoie de la requete au serveur
    char repTxt[sizeof(rep_t)];
    repTOstr(&rep, repTxt);

    ecrireMsgTCP(sock, repTxt);
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
        afficherPartie();
        lenLu = lireMsgTCP(*sock, msgLu, sizeof(buffer_t));
        DEBUG_S1("Serveur : message reçu len <%d>\n", lenLu);
        if (lenLu > 0)
        {
            strTOreq(&req, msgLu);
            DEBUG_S3("Serveur : socket <%i> msg recu <%s> avec idReq <%d>\n", *sock, msgLu, req.idReq);

            switch (req.idReq)
            {
            case CREERPARTIE:
                DEBUG_S("Case CreerPartie");
                newpartieServ(*sock, &req);
                break;
            case LISTERPARTIE:
                DEBUG_S("Case GetPartie");
                getParties(*sock);
                break;

            default:
                DEBUG_S("Case Default");
                break;
            }
        }
    }

    DEBUG_S1("Serveur : le client avec la socket <%d> a quitté la connexion\n", *sock);

    // Le Client a fermé on cloture la socket client
    fermerSocket(*sock);
}

// initialisation de la structure statPatie_t
void initstatPartie(void)
{
    nbPartie = 0;
}

#endif
/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */
#ifdef CLIENT
//-fct generation des requétes
int createPartyReq(int sock, char *pseudo)
{
    // Verification on est connecte
    if (sock == 0)
    {
        printf("Connectez vous avant svp\n");
        return 0;
    }
    // On recupere notre adresse IP
    char hostbuffer[MAX_LEN];
    char IPbuffer[MAX_LEN];
    getMyIp(IPbuffer);
    DEBUG_S2("Creation partie avec IP <%s> et port <%d>\n", IPbuffer, portClientMaitre);

    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = CREERPARTIE;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port = portClientMaitre;
    adresseTOstr(&monAdr, req.msgReq);
    req.lgreq = strlen(req.msgReq);

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);
    ecrireMsgTCP(sock, reqTxt);

    // Attente de la reponse du serveur
    buffer_t msgLu;
    int lenLu = 1;
    rep_t rep;
    statutReq_t statut;

    lenLu = lireMsgTCP(sock, msgLu, sizeof(buffer_t));
    DEBUG_S2("Client : message reçu len <%d> <%s>\n", lenLu, msgLu);
    strTOrep(&rep, msgLu);

    if (rep.idRep != STATUT)
    {
        printf("Mauvaise reponse reçu : impossible\n");
        return 0;
    }
    strTOstatutReq(&statut, rep.msgRep);
    DEBUG_S4("Client : socket <%i> msg recu <%s> avec idReq <%d> et statut <%d>\n", sock, msgLu, rep.idRep, statut.statut);
    if (statut.statut == OK)
        printf("Creation de partie sur le serveur réussie\n");
    else
        printf("Echec de creation de partir sur le serveur\n");

    return 1;
}

int getPartiesReq(int sock)
{
    // Verification on est connecte
    if (sock == 0)
    {
        printf("Connectez vous avant svp\n");
        return 0;
    }

    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = LISTERPARTIE;
    req.lgreq = 0;

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);

    ecrireMsgTCP(sock, reqTxt);

    // Attente de la reponse du serveur
    buffer_t msgLu;
    int lenLu = 1;
    rep_t rep;
    statutReq_t statut;

    lenLu = lireMsgTCP(sock, msgLu, sizeof(buffer_t));
    DEBUG_S2("Client : message reçu len <%d> <%s>\n", lenLu, msgLu);
    strTOrep(&rep, msgLu);
    if (rep.idRep != LISTERPARTIE)
    {
        printf("Mauvaise reponse reçu : impossible\n");
        return 0;
    }
    StrTOlistePartie(listePartie, rep.msgRep);
    DEBUG_S1("getPartiesReq retour nbPartie <%d>\n", nbPartie);
    return 1;
};

// Fonction joinPartieReq utilisée par l'adversaire
int joinPartieReq(int masock, char *pseudo, int *obst, time_t *top)
{

    // On recupere notre adresse IP
    char hostbuffer[MAX_LEN];
    char IPbuffer[MAX_LEN];
    getMyIp(IPbuffer);

    // On prepare la requete pour le client Maitre
    req_t req;
    req.idReq = JOIN;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port = 0; // le client distant n'a pas de port d'écoute
    adresseTOstr(&monAdr, req.msgReq);
    req.lgreq = strlen(req.msgReq);

    // Envoie de la requete JOIN au client maitre
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);
    ecrireMsgTCP(masock, reqTxt);

    // Le CLient Maitre repond avec les DATA de la partie
    char repTxt[sizeof(rep_t)];
    lireMsgTCP(masock, repTxt, MAX_LEN);
    printf("DATA init partie recus:%s\n", repTxt);
    rep_t rep;
    strTOrep(&rep, repTxt);
    if (rep.idRep = 50)
    {
        StringinitTOParti(top, obst, rep.msgRep);
    }
    else
    {
        printf("Reponse du client maitre incoherente\n");
        return 0;
    }
    return 1;
};



// Utilise par client maitre
void initPartie(int masock, adresse_t *adversaire)
{

    //init variable globale servant au req //TODO les rendre global pour qui soivent accesible à la rep de streaming et la rep de statpartie de l'adversaire
    printf("debut init partie\n");
    int mon_score = 0;
    int son_score = 0;
    char **pic = empty_picture(' ');
    //generation des obsacle et top depart
    //srand((unsigned int)time);
    int *obstaclesInitiaux = init_obstacles(NBMAXOBSTACLES);
    time_t now = time(NULL);

    //caste data
    joinPartieRep(masock, obstaclesInitiaux, now + 9); //TODO complété le contenus
    printf("Pret pour lancer la partie");
    partie(obstaclesInitiaux, &mon_score, &son_score, pic, now + 9);
};

// Fonction joinPartieRep utilisée par le client maitre
void joinPartieRep(int masock, int *obstacle, time_t temps)
{
    rep_t rep;
    rep.idRep = STARTPARTIE;
    initPartiTOString(rep.msgRep, temps, obstacle);
    rep.lgrep = strlen(rep.msgRep);

    // Envoie de la requete au serveur
    char repTxt[sizeof(rep_t)];
    repTOstr(&rep, repTxt);
    printf("DATA init envoyé:%s\n", repTxt);
    ecrireMsgTCP(masock, repTxt);
};


void startReq(int sock){

    //Le client adverse prévient le client maitre qu'il est prêt à demarrer

    // On recupere notre adresse IP
    /*  char hostbuffer[MAX_LEN];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    CHECK_T((hostname = gethostname(hostbuffer, sizeof(hostbuffer))) != -1, "Erreur gethostname");
    CHECK_T((host_entry = gethostbyname(hostbuffer)) != NULL, "Erreur gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr *)
                               host_entry->h_addr_list[0]));


    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = START;
    adresse_t monAdr;
    strcpy(monAdr.ip, IPbuffer);
    strcpy(monAdr.pseudo, pseudo);
    monAdr.port = PORT_CLIENTMAITRE_PARTIE;
    adresseTOstr(&monAdr, req.msgReq);
    req.lgreq = strlen(req.msgReq);

    // Envoie de la requete au serveur
    char reqTxt[sizeof(req_t)];
    reqTOstr(&req, reqTxt);

    ecrireMsgTCP(sock, reqTxt); */

    // Va ensuite à la fonction partieMaitre

};
void startRep(){};
void UpdateStatutPlayerReq(){};
void updateStatutPlayerRep(){};
//-à chaque req ,on associera &fct de traitement qui genere une réponse
void waitParties(){};
void afficherParties(){};

void getStart(){};
void updateStatutPlayerMaitre(){};
void updateStatutPlayerInvite(){};
void stream(){};                                 // publier ma partie
void afficherStream(int sock, char *myPseudo){}; // voir une partie
void partieSolo(int sock, char *myPseudo)
{
    //init variable globale servant au req
    int mon_score = 0;
    int son_score = 0;
    char **pic = empty_picture(' ');
    //generation des obsacle et top depart
    //srand(time);
    int *obstaclesInitiaux = init_obstacles(NBMAXOBSTACLES);
    time_t now = time(NULL);
    //on lence la partie
    system("./scriptZoom.sh -m");
    //draw_ascii_score(empty_picture('?'),mon_score,son_score);
    partie(obstaclesInitiaux, &mon_score, &son_score, pic, now + 3);
    system("./scriptZoom.sh -p");
    //printf("mon score:%d son score:%d\n", mon_score,son_score);
};

//1 fct de selection traitement selon requete
//UNUSED
/*
void lireReqClient(int *masock)
{
    //DEBUG_S1("Serveur Partie: New thread pour socket <%i>\n", *masock);
    DEBUG_S1("Serveur Partie: Debut lireReqClient pour socket <%i>\n", *masock);
    
    buffer_t msgLu;
    req_t req;
    int lenLu = 1;
	
    // On attend les inputs du Client adverse
    while (lenLu > 0)
    {
		//afficherPartie();
        lenLu = lireMsgTCP(*masock, msgLu, sizeof(buffer_t));
        DEBUG_S1("Serveur : message reçu len <%d>\n", lenLu);
		if (lenLu>0)
		{
			strTOreq(&req, msgLu);
			DEBUG_S3("Serveur : socket <%i> msg recu <%s> avec idReq <%d>\n", *masock, msgLu, req.idReq);
            
            switch (req.idReq)
            {
            case JOIN:
                printf("SERVER PARTI:on demande à joindre la partie\n");
                initPartie(*masock);
                break;
            default:
                break;
            }
        }
    }
}
*/
#endif