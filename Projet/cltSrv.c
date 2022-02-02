#include <pthread.h>
#include "cltSrv.h"

statPartie_t listePartie[NBMAXCLIENT]; // Liste des parties en cours
unsigned nbPartie = 0;                 // nb de partie en cours

sem_t mutex;

#ifdef CLIENT
int sock = 0; // Numero Socket client
#endif

#ifdef SERVER
int socketEcoute = 0; // Numero Socket serveur
int socketClient[NBMAXCLIENT];
int nbClient = 0;
int continuer = 1;
#endif

//#define CLIENT
#ifdef CLIENT
void afficherMenu()
{
	printf("Menu:\n \
		1) Se connecter au serveur\n\
		2) Lister les parties en cours\n\
		3) Creer une partie\n\
		0) Quitter\n");
}

int main(/*int argc, char const *argv[]*/)
{
    // TODO
    char myPseudo[MAX_LEN];
    // system("./scriptZoom.sh -m");
    // system("./scriptZoom.sh -p");
    int choix = 4;

    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);

    // est ce qu'on fait une fonction pour la récupération d'ip ?
    // char ip[30]=recupererIp();

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
            getPartiesReq(sock);
            break;
        case 3:
            createPartyReq(sock, myPseudo);
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
    DEBUG_S("Debut connecterServeur\n");
	if (sock != 0)
	{
		printf("Deja connecté au serveur\n");
		return;
	}
	
	sock = creerSocketClient(PORT_SERVER, ADDRSERVERENR);
	if (sock == 0) printf("Erreur connection serveur\n");
};

#endif
// define SERVER
#ifdef SERVER

int main(/*int argc, char const *argv[]*/)
{
    socklen_t cltLen;
    struct sockaddr_in clt;

    pthread_t tid[NBMAXCLIENT];
    int idxThread[NBMAXCLIENT];
    double *status;

    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);

    // On se met en ecoute sur le port Serveur
    socketEcoute = creerSocketEcoute(PORT_SERVER);
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
    exit(0);
#endif

#ifdef SERVER
    DEBUG_S1("Serveur fermeture socket <%d>\n", socketEcoute);
    fermerSocket(socketEcoute); // Numero Socket serveur
    CHECK_T(sem_destroy(&mutex) == 0, "erreur destroy mutex");
	// On force la fermeture des socket
    DEBUG_S("Serveur fermeture des sockets clientes\n");
	for (int i=0; i< nbClient; i++)
	{
		fermerSocket(socketClient[i]);
		DEBUG_S1("Serveur fermeture socket cliente <%d>\n", socketClient[i]);
	}
    exit(0);
#endif
}