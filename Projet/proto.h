#ifndef __PROTO_H__
    #define __PROTO_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "session.h"
#include "reqRep.h"
#include "basic_func.h"



/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */
  
    // Initialisation
    void initstatPartie(void);

    //-fct generation des requétes
	void createPartieRep(int sock, int status);
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    int newpartieServ(int sock, req_t *req);
    void getParties(int sock);
    //1 fct de selection traitement selon requete
    //void lireReqServ(req_t req,struct sockaddr_in *clt,int sock);
    void lireReqServ(int *sock);
    void afficherPartie(void);

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */

    //-fct generation des requétes
    void createPartyReq(int sock, char * pseudo);
    void getPartiesReq(int sock);
    void joinPartieReq(int sock, int idPartie, char *pseudo);
    void joinPartieRep(int sock,char*obstacle,char*topdepart);
    void startReq(int sock);
    void startRep();
    void UpdateStatutPlayerReq();
    void updateStatutPlayerRep();
    //-à chaque req ,on associera &fct de traitement qui genere une réponse
    void waitParties();
    void afficherParties();
    void initPartie(int sock);
    void getStart();
    void partieMaitre();
    void partieAdverse();
    void updateStatutPlayerMaitre();
    void updateStatutPlayerInvite();
    void stream();
    void afficherStream();
    //1 fct de selection traitement selon requete
    void lireReqClient(int *sock);
#endif


