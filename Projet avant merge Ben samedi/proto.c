#include <netdb.h>
#include "proto.h"
#include "cltSrv.h"
#include "basic_func.h"
#include "data.h"



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

///-à chaque req ,on associera &fct de traitement qui genere une réponse
//    void newpartieServ(short lg,buffer_t buff,struct sockaddr_in *clt,int sock){
//-fct generation des requétes
void createPartieRep(int sock, int status){
    rep_t rep;
    rep.idRep = STATUT;
	statutReq_t statut;
	statut.statut=status;
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
		if (lenLu>0)
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


/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */

//-fct generation des requétes
void createPartyReq(int sock, char *pseudo)
{
	// Verification on est connecte
	if (sock == 0)
	{
		printf("Connectez vous avant svp\n");
		return;
	}
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
	
	// Attente de la reponse du serveur
	buffer_t msgLu;
    int lenLu = 1;
	rep_t rep;
	statutReq_t statut;

	lenLu = lireMsgTCP(sock, msgLu, sizeof(buffer_t));
    DEBUG_S1("Client : message reçu len <%d>\n", lenLu);
	strTOrep(&rep, msgLu);
	if (rep.idRep != STATUT)
	{
		printf("Mauvaise reponse reçu : impossible\n");
		return;
	}
	strTOstatutReq(&statut, rep.msgRep);
	DEBUG_S4("Client : socket <%i> msg recu <%s> avec idReq <%d> et statut <%d>\n", sock, msgLu, rep.idRep, statut.statut);
	if (statut.statut == OK)
		printf("Creation de partir sur le serveur réussie\n");
	else
		printf("Echec de creation de partir sur le serveur\n");

}

void getPartiesReq(int sock){
	// Verification on est connecte
	if (sock == 0)
	{
		printf("Connectez vous avant svp\n");
		return;
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
		return;
	}
	StrTOlistePartie(listePartie, rep.msgRep);
	DEBUG_S1("getPartiesReq retour nbPartie <%d>\n", nbPartie);
	afficherPartie();

};

void joinPartieReq(int sock, int idPartie, char *pseudo){
    // On recupere notre adresse IP
   /* char hostbuffer[MAX_LEN];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    CHECK_T((hostname = gethostname(hostbuffer, sizeof(hostbuffer))) != -1, "Erreur gethostname");
    CHECK_T((host_entry = gethostbyname(hostbuffer)) != NULL, "Erreur gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr *)
                               host_entry->h_addr_list[0]));

    // On prepare la requete pour le serveur
    req_t req;
    req.idReq = JOIN;
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
    
    initPartie
    */








};
void joinPartieRep(){};

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
    monAdr.port = PORT_CLIENTMAITRE;
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
void initPartie(){
    //
};
void getStart(){};
void partieMaitre(){
    //recuparation du timestamp







    // prévenir l'adversaire via startRep()


};
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
