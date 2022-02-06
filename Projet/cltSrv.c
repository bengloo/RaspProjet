#include <pthread.h>
#include "cltSrv.h"

statPartie_t listePartie[NBMAXCLIENT]; // Liste des parties en cours
unsigned nbPartie = 0;                 // nb de partie en cours

// mutex server enregistrement
sem_t mutex;
// mutex server partie
sem_t mutexpartie;

#ifdef CLIENT
// vis à vis du server
int sock = 0; // Numero Socket client du server  d'enregistrement
int portClientMaitre = 0; // Port d'écoute
char ipServer[MAX_LEN]; // IP du Server


// vis à vis du client Maitre adverse
int sockPartie = 0;         // Numero Socket client du server  de partie
int socketEcoutePartie = 0; // Numero SocketPartie
int socketClientPartie[NBMAXCLIENT];
int nbClientPartie = 0;
int continuerPartie = 1; // inutiliser ?

char myPseudo[MAX_LEN];

#endif

#ifdef SERVER
int socketEcoute = 0; // Numero Socket serveur
int socketClient[NBMAXCLIENT];
int nbClient = 0;
int continuer = 1; // inutiliser ?
#endif

//#define CLIENT
#ifdef CLIENT
void afficherMenu()
{
    printf("Joueur <%s>\n\
		Menu:\n \
		1) Se connecter au serveur\n\
		2) Lister les parties en cours\n\
		3) Creer une partie\n\
		4) Voir la partie d'un joueur\n\
		10) Lancer partie en solo\n\
		0) Quitter\n", myPseudo);
}

int main(int argc, char const *argv[])
{
    int choix = 4;

    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);

    // Lecture parametre
    portClientMaitre = PORT_CLIENTMAITRE_PARTIE;
	strcpy(ipServer, ADDRSERVERENR);
    readParam(argc, argv, &portClientMaitre, ipServer);

    // On ouvre la socket sur le port  du Serveur de partie // a faire une fois pour eviter Already in use à chaque partie
    socketEcoutePartie = creerSocketEcoute(portClientMaitre);
    DEBUG_S1("Serveur de partie socket <%d> ouverte\n", socketEcoutePartie);


    draw_ascii(empty_picture(' '));
    printf("veuillez saisir votre pseudo pour vous connecter:\n");
    scanf("%s", myPseudo);
    choix = 1;

    while (choix != 0)
    {
        afficherMenu();
        // draw_ascii(empty_picture(' '));
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            connecterServeur();
            break;
        case 2:
            partieAdverse(sock, myPseudo);
            break;
        case 3:
            partieMaitre(sock, myPseudo);
            break;
        case 4:
            afficherStream(sock, myPseudo);
            break;
        case 10:
            partieSolo(sock, myPseudo);
            break;

        default:

            break;
        }
    }

    return 0;
}

void connecterServeur(void)
{
    req_t req;
    // envois creation party dgram
    DEBUG_S2("Debut connecterServeur IP <%s> port <%d>\n", ipServer, PORT_SERVER);
    if (sock != 0)
    {
        printf("Deja connecté au serveur\n");
        return;
    }

    sock = creerSocketClient(PORT_SERVER, ipServer);
    if (sock == 0)
        printf("Erreur connection serveur\n");
};

void connecterServeurPartie(adresse_t addrServerPartie)
{
    req_t req;
    // envois creation party dgram
    DEBUG_S("Debut connecterServeurPartie\n");
    if (sockPartie != 0)
    {
        printf("Deja connecté au serveurPartie\n");
        return;
    }
    DEBUG_S2("tentative de conexion à %d,%s", addrServerPartie.port, addrServerPartie.ip);
    sockPartie = creerSocketClient(addrServerPartie.port, addrServerPartie.ip);
    if (sockPartie == 0)
        printf("Erreur connection serveurPartie\n");
};

int serverPartie()
{

    // Creation d'une sockPartie en ecoute des autres clients
    socklen_t cltLen;
    struct sockaddr_in clt;

    pthread_t tid[NBMAXCLIENT];
    int idxThread[NBMAXCLIENT];
    double *status;

    // TODO adapté instalderoute pour passé en parametre la socket a fermé que ce soit inter client ou server
    /*
    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);
    */

	// Attente connexion client adverse
    cltLen = sizeof(clt);
    printf("Attente de connexion d'un client sur port <%d>\n", portClientMaitre);
    CHECK(socketClientPartie[nbClientPartie] = accept(socketEcoutePartie, (struct sockaddr *)&clt, &cltLen), "Can't accept"); // accept de recevoir mess
    DEBUG_S1("Nouvelle connexion <%i>\n", socketClientPartie[nbClientPartie]);

    // Le client adverse demanade le join
    char msgLu[MAX_LEN];
    int lenLu = lireMsgTCP(socketClientPartie[nbClientPartie], msgLu, sizeof(buffer_t));
    DEBUG_S1("Serveur : message reçu len <%d>\n", lenLu);
    if (lenLu > 0)
    {
        rep_t rep;
        strTOrep(&rep, msgLu);
        if (rep.idRep == JOIN)
        {
            adresse_t adversaire;
            strTOadresse(&adversaire, rep.msgRep);
            printf("SERVER PARTI:joueur <%s> demande à joindre la partie\n", adversaire.pseudo);
            

            statPartie_t partie;

            partie.id=0;//TODO 
            partie.statut=RUNNING;
            strcpy(partie.addrMaitre.ip,"SAME");
            partie.addrMaitre.port=-1;
            strcpy(partie.addrMaitre.pseudo,"SAME");
            strcpy(partie.addrAdverse.ip,adversaire.ip);
            partie.addrAdverse.port=adversaire.port;
            strcpy(partie.addrAdverse.pseudo,adversaire.pseudo);
            partie.scoreMaitre=0;
            partie.scoreAdverse=0;

            updateStatutPartieReq(sock,partie);
            initPartie(socketClientPartie[nbClientPartie], &adversaire);

            partie.statut=STOPPED;
            strcpy(partie.addrAdverse.ip,"SAME");
            partie.addrAdverse.port=-1;
            strcpy(partie.addrAdverse.pseudo,"SAME");
            partie.scoreMaitre=10;//mon_score;TODO
            partie.scoreAdverse=10;//son_score;
        }
    }
    fermerSocket(socketClientPartie[nbClientPartie]);

    //nbClientPartie++;
    //}
    return 0;
}

void partieMaitre(int masock, char *myPseudo)
{

    if (createPartyReq(masock, myPseudo))
    {
        if (serverPartie())
        {
            // TODO reinitialisé les varaible globale de partie avant de revenir au menu.
        }
        else
        {
            // TODO update le statut de la partie au server d'enregistrement en erreure
        };
    };

    // retour au menu
};
void partieAdverse(int masock, char *myPseudo)
{
	DEBUG_S("partieAdverse debut\n");
    if (getPartiesReq(masock))
    {
        int choix = -2;
		afficherPartie();
		printf("\n\nSelectionez une partie avec son indices\nToute autre choix=revenir au menu principal\n\tchoix:");
		scanf("%d", &choix);
		if (choix >= 0 && choix < nbPartie)
		{
			connecterServeurPartie(listePartie[choix].addrMaitre);
			if (sockPartie != 0)
			{
				DEBUG_S1("ADVERSE:on s'est conecter au client maitre de partie sockPartie<%d>\n", sockPartie);
				time_t top;
				partieGraphique_t partie;
				if (joinPartieReq(sockPartie, myPseudo, &partie, &top))
				{
				}
				else
				{
					printf("imposible de joindre cette partie\n");
				}
			}
			else
			{
				printf("Partie indisponible\n");
			}
        }
    };

    // retour menu
};

#endif
// define SERVER
#ifdef SERVER

int main(int argc, char const *argv[])
{
    socklen_t cltLen;
    struct sockaddr_in clt;

    pthread_t tid[NBMAXCLIENT];
    int idxThread[NBMAXCLIENT];
    double *status;

    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);

    // Lecture parametre
    int port = PORT_SERVER; // Attention si on change ici il faut rendre aussi dynamique dans les clients
                            // readParam(argc, argv, &port);

    // On se met en ecoute sur le port Serveur
    socketEcoute = creerSocketEcoute(port);
    DEBUG_S1("Serveur socket <%d> en ecoute\n", socketEcoute);

    // Initialisatio des parties
    initstatPartie();

    // On prepar le mutex
    CHECK_T(sem_init(&mutex, 0, 1) == 0, "erreur initialisation mutex");
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");

    while (1)
    {
        cltLen = sizeof(clt);
        CHECK(socketClient[nbClient] = accept(socketEcoute, (struct sockaddr *)&clt, &cltLen), "Can't accept"); // accept de recevoir mess
        DEBUG_S1("Nouvelle connexion <%i>\n", socketClient[nbClient]);
        CHECK_T(pthread_create(&tid[nbClient], NULL, (pf_t)lireReqServ,
                               (void *)(&socketClient[nbClient])) == 0,
                "Erreur pthread_create()");

        nbClient++;
    }
}
#endif

void usage(const char *prg)
{
    printf("usage :%s <portClientMaitre> <ipServeur>\n", prg);
}

void readParam(int argc, char const *argv[], int *portClientMaitre, char *ipServer)
{
	if (argc == 1) return;
    if (argc > 1)
    {
        *portClientMaitre = atoi(argv[1]);
        char tmp[MAX_LEN];
        sprintf(tmp, "%d", *portClientMaitre);
        if (strcmp(tmp, argv[1]) != 0)
        {
            usage(argv[0]);
            exit(0);
        }
    }
    if (argc > 2)
    {
		strcpy(ipServer, argv[2]);
    }
	else
        usage(argv[0]);
	
}

void installDeroute(int numSig, void (*pfct)(int))
{
    struct sigaction newAction;
    // renseigner l'action pour ignorer le crtl C
    newAction.sa_handler = pfct;
    CHECK_T(sigemptyset(&newAction.sa_mask) == 0, "--Erreur sigemptyset--");
    newAction.sa_flags = SA_RESTART; // permet de redemarrer les appel bloquants
                                     // on installe le gestionnaire de SIGINT
    CHECK_T(sigaction(numSig, &newAction, NULL) == 0, "--sigaction--");
}

void deroute(int numSig)
{
    switch (numSig)
    {
    case SIGINT:
        DEBUG_S1("\t[PID=%d]^C reçu, on ferme\n", getpid());
        terminerProcess();
        break;

    default:
        printf("Incredible\n");
    }
}

void terminerProcess(void)
{
#ifdef CLIENT
    DEBUG_S1("Client fermeture socket <%d>\n", sock);
    fermerSocket(sock); // Numero Socket serveur
	fermerSocket(socketEcoutePartie);// Numero Socket Client Maitre
    exit(0);
#endif

#ifdef SERVER
    DEBUG_S1("Serveur fermeture socket <%d>\n", socketEcoute);
    fermerSocket(socketEcoute); // Numero Socket serveur
    CHECK_T(sem_destroy(&mutex) == 0, "erreur destroy mutex");
    // On force la fermeture des socket
    DEBUG_S("Serveur fermeture des sockets clientes\n");
    for (int i = 0; i < nbClient; i++)
    {
        fermerSocket(socketClient[i]);
        DEBUG_S1("Serveur fermeture socket cliente <%d>\n", socketClient[i]);
    }
    exit(0);
#endif
}
