/* ------------------------------------------------------------------------ */
/**
 *  \file       cltSrv.c
 *  \brief      Programme réalisant un executable serveur d'enregistrement et un executable client
 *              permettant de gerer des parties temple_run multijoueurs 
 *				table de villes chargée à partir d'un fichier.
 *
 *  \author     Morcq Alexandre B2
 *
 *  \date       08/02/2022
 *
 *	\version    1.0
 * *
 *	\remark		fichier contenant nos fonctions, ...
 *
 */
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include "data.h"
#include "session.h"
#include "proto.h"
#include "graphisme.h"
#include "basic_func.h"

#define PORT_SERVER 8123              //port du server d'enregistrement pour parlé auc client
#define PORT_CLIENTMAITRE_PARTIE 8125 //por du client maitre pour parlé au autre client
#define ADDRSERVER "127.0.0.1"

// Varaible commune client et serveur
statPartie_t listePartie[NBMAXCLIENT]; // Liste des parties en cours

#ifdef CLIENT
int sockServer = 0;       // Numero Socket client du server  d'enregistrement
int portClientMaitre = 0; // Port d'écoute
char ipServer[MAX_LEN];   // IP du Server

// vis à vis du client Maitre adverse
int sockPartie = 0;         // Numero Socket client du server  de partie
int socketEcoutePartie = 0; // Numero SocketPartie
int socketClientPartie[NBMAXCLIENT];
int nbClientPartie = 0;
int continuerPartie = 1; // inutiliser ?

char myPseudo[MAX_LEN];
int mon_score = 0;
int son_score = 0;

#endif

#ifdef SERVER
sem_t mutex;          // mutex server enregistrement
int socketEcoute = 0; // Numero Socket serveur
listeClient_t listeClient[NBMAXCLIENT];
int nbClient = 0;
int continuer = 1;     // inutiliser ?
unsigned nbPartie = 0; // nb de partie en cours nota
#endif

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR    & CLIENT                                        */
/* ------------------------------------------------------------------------ */
// Gestion de signaux
void installDeroute(int numSig, void (*pfct)(int)); // deroute un signal vers la funct en 2ème param = pointeur sur une fontion
void deroute(int numSig);                           // exemple de fonction qui sera appelee en cas de signal
void terminerProcess(void);
void afficherPartie(unsigned nbPart);

/* ------------------------------------------------------------------------ */
/*      FONCTION CLIENT                                                     */
/* ------------------------------------------------------------------------ */
#ifdef CLIENT

void partieMaitre(int sock, char *myPseudo);
void partieAdverse(int sock, char *myPseudo);
int serverPartie(); //generre le server de partie de la meme maniere que le server d'enregistrement
void afficherMenu();
void partieSolo(int sock, char *myPseudo);
void readParam(int argc, char const *argv[], int *portClientMaitre, char *ipServer);
void connecterServeur(void);
void connecterServeurPartie(adresse_t serverPartie);
/**
 *  \fn         int main(int argc, char const *argv[])
 *  \brief      fonction principale 
 *  \param      prend en paramètre les paramètres de ligne de commande 
 *  \return    retourne un int (valeur de retour)
 */
int main(int argc, char const *argv[])
{
    int choix = 4;

    // On catch le SIGINT pour fermer la socket
    installDeroute(SIGINT, deroute);

    // Lecture parametre
    portClientMaitre = PORT_CLIENTMAITRE_PARTIE;
    strcpy(ipServer, ADDRSERVER);
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
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            connecterServeur();
            break;
        case 2:
            partieAdverse(sockServer, myPseudo);
            break;
        case 3:
            partieMaitre(sockServer, myPseudo);
            break;
        case 10:
            partieSolo(sockServer, myPseudo);
            break;

        default:

            break;
        }
    }

    return 0;
}
/**
 *  \fn         void afficherMenu()
 *  \brief      fonction d'affichage du menu  
 *  \param      void
 *  \return     void
 */
void afficherMenu()
{
    printf("Joueur <%s>\n\
		Menu:\n \
		1) Se connecter au serveur\n\
		2) Lister les parties en cours\n\
		3) Creer une partie\n\
		10) Lancer partie en solo\n\
		0) Quitter\n",
           myPseudo);
}

// permet de jouer seul
/**
 *  \fn         void partieSolo(int sock, char *myPseudo)
 *  \brief      fonction permettant de lancer une partie solo 
 *  \param      prend en paramètre une socket et un pseudo
 *  \return     void
 */
void partieSolo(int sock, char *myPseudo)
{
    //init variable globale servant au req
    int mon_score = 0;
    int son_score = 0;
    char **pic = empty_picture(' ');

    //generation des obsacle et top depart
    partieGraphique_t partieGraphique;
    initPartieGraphisme(&partieGraphique);
    time_t now = time(NULL);

    //on lence la partie
    system("./scriptZoom.sh -m");
    jouerPartie(&partieGraphique, &mon_score, &son_score, pic, now + DELAY_START, 0);
    system("./scriptZoom.sh -p");
};
/**
 *  \fn         void connecterServeur(void)
 *  \brief      fonction permettant de se connecter au serveur d'enregistrement 
 *  \param      void
 *  \return     void
 */
void connecterServeur(void)
{
    // envois creation party dgram
    DEBUG_S2("Debut connecterServeur IP <%s> port <%d>\n", ipServer, PORT_SERVER);
    if (sockServer != 0)
    {
        printf("Deja connecté au serveur\n");
        return;
    }

    sockServer = creerSocketClient(PORT_SERVER, ipServer);
    if (sockServer == 0)
        printf("Erreur connection serveur\n");

    if (connecterClientReq(sockServer, myPseudo) != OK)
    {
        printf("Erreur de connexion\n");
        fermerSocket(sockServer);
        return;
    }
    printf("Connexion reussie\n");
};
/**
 *  \fn         void connecterServeurPartie(adresse_t addrServerPartie)
 *  \brief      fonction permettant de se connecter au serveur d'une partie créer par un client 
 *  \param      addrServerPartie une addresse de server de partie de type adresse_t 
 *  \return     void
 */
void connecterServeurPartie(adresse_t addrServerPartie)
{
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
/**
 *  \fn         int serverPartie(unsigned idPartie)
 *  \brief      fonction qui permet d'attendre q'un client adverse se connecte, 
 * mettre a jour l'etat de la partie sur le server et lancer la partie 
 *  \param      ipPartie ID de partie
 *  \return     void
 */
int serverPartie(unsigned idPartie)
{

    // Creation d'une sockPartie en ecoute des autres clients
    socklen_t cltLen;
    struct sockaddr_in clt;

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
            statPartie_t partie;
            adresse_t adversaire;
            strTOadresse(&adversaire, rep.msgRep);
            printf("SERVER PARTI:joueur <%s> demande à joindre la partie\n", adversaire.pseudo);

            partie.id = idPartie;
            partie.statut = RUNNING;
            char IPbuffer[MAX_LEN];
            getMyIp(IPbuffer);
            strcpy(partie.addrMaitre.ip, IPbuffer);
            partie.addrMaitre.port = portClientMaitre;
            strcpy(partie.addrMaitre.pseudo, myPseudo);
            strcpy(partie.addrAdverse.ip, adversaire.ip);
            partie.addrAdverse.port = adversaire.port;
            strcpy(partie.addrAdverse.pseudo, adversaire.pseudo);
            partie.scoreMaitre = 0;
            partie.scoreAdverse = 0;
            updateStatutPartieReq(sockServer, &partie);

            initPartieRep(socketClientPartie[nbClientPartie], &adversaire, &mon_score, &son_score);
            if (mon_score > son_score)
                printf("Vous avez gagné!\n");
            else
                printf("Vous ferez mieux la prochaine fois!\n");

            partie.statut = STOPPED;
            partie.scoreMaitre = mon_score;
            partie.scoreAdverse = son_score;
            updateStatutPartieReq(sockServer, &partie);
        }
    }
    else
        printf("Erreur de message\n");

    fermerSocket(socketClientPartie[nbClientPartie]);
    sockPartie = 0;
    return OK;
}
/**
 *  \fn         void partieMaitre(int masock, char *myPseudo)
 *  \brief      fonction qui permet de creer une partie, de l'enregistrer au près du
 *  serveur et de lancer la fonction d'attente d'un adversaire 
 *  \param      masock socket serveur 
 *  \param      myPseudo pseudo du client maitre
 *  \return     void
 */
void partieMaitre(int masock, char *myPseudo)
{
    int idPartie;
    if (createPartieReq(masock, myPseudo, portClientMaitre, &idPartie))
    {
        serverPartie(idPartie);
    }
}
/**
 *  \fn         void partieAdverse(int masock, char *myPseudo)
 *  \brief      fonction qui permet a un adversaire de recuperer la liste de partie 
 * sur le serveur d'enregistrement, de se connecter au client Maitre(créateur de la partie)
 * et de jouer la partie
 *  \param      masock socket serveur 
 *  \param      myPseudo pseudo du client maitre
 *  \return     void
 */
void partieAdverse(int masock, char *myPseudo)
{
    unsigned nbPart = 0;
    DEBUG_S("partieAdverse debut\n");
    if (nbPart = getPartiesReq(masock, listePartie))
    {
        int choix = -2;
        afficherPartie(nbPart);
        printf("\n\nSelectionez une partie avec son indices\nToute autre choix=revenir au menu principal\n\tchoix:");
        scanf("%d", &choix);
        if (choix >= 0 && choix < nbPart)
        {
            if (listePartie[choix].statut != WAITTINGADVERSE)
            {
                printf("Désolé cette partie n'est pas disponible\n");
                return;
            }
            connecterServeurPartie(listePartie[choix].addrMaitre);
            if (sockPartie != 0)
            {
                DEBUG_S1("ADVERSE:on s'est conecter au client maitre de partie sockPartie<%d>\n", sockPartie);
                time_t top;
                partieGraphique_t partie;
                if (!joinPartieReq(sockPartie, myPseudo, &partie, &mon_score, &son_score, &top))
                {
                    printf("imposible de joindre cette partie\n");
                }
                if (mon_score > son_score)
                    printf("Vous avez gagné!\n");
                else
                    printf("Vous ferez mieux la prochaine fois!\n");
                fermerSocket(sockPartie);
                sockPartie = 0;
            }
            else
            {
                printf("Partie indisponible\n");
            }
        }
    }
    else
        printf("Pas de partie disponible\n");
}
/**
 *  \fn         void usage(const char *prg)
 *  \brief      fonction qui permet d'afficher l'usage des paramètres des paramètres possible pour 
 * lancer le programme
 *  \param      prg nom d'executable
 *  \return     void
 */
void usage(const char *prg)
{
    printf("usage :%s <portClientMaitre> <ipServeur>\n", prg);
}
/**
 *  \fn         void readParam(int argc, char const *argv[], int *portClientMaitre, char *ipServer)
 *  \brief      fonction qui permet de faire la lecture des paramètres passer en arguments au lancement de l'executable
 *  \param      argc nombre de paramètre 
 *  \param      argv liste des chaines de paramètre 
 *  \param      portClientMaitre variable dans laquelle il faut stocker le paramètre portClientMaitre passé
 *  \param      ipServer variable dans laquelle il faut stocker le paramètre ipServer passé
 *  \return     void
 */
void readParam(int argc, char const *argv[], int *portClientMaitre, char *ipServer)
{
    if (argc == 1)
        return;
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

#endif

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR                                                     */
/* ------------------------------------------------------------------------ */
#ifdef SERVER

void initstatPartie(void);
void lireReqServ(listeClient_t *client);
void afficherCLient();

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
    int port = PORT_SERVER;

    // On se met en ecoute sur le port Serveur
    socketEcoute = creerSocketEcoute(port);
    DEBUG_S1("Serveur socket <%d> en ecoute\n", socketEcoute);

    // Initialisatio des parties
    initstatPartie();

    // On prepar le mutex
    CHECK_T(sem_init(&mutex, 0, 1) == 0, "erreur initialisation mutex");
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");

    while (nbClient < NBMAXCLIENT)
    {
        cltLen = sizeof(clt);
        int socket = 0;
        CHECK(socket = accept(socketEcoute, (struct sockaddr *)&clt, &cltLen), "Can't accept"); // accept de recevoir mess
        listeClient[nbClient].statut = OFFLINE;
        listeClient[nbClient].socket = socket;
        listeClient[nbClient].idx = nbClient;
        listeClient[nbClient].pseudo[0] = '\0';
        DEBUG_S2("Nouvelle connexion nbClient <%d> socket <%i>\n", nbClient, socket);
        CHECK_T(pthread_create(&tid[nbClient], NULL, (pf_t)lireReqServ,
                               (void *)(&listeClient[nbClient])) == 0,
                "Erreur pthread_create()");

        nbClient++;
    }
}

void lireReqServ(listeClient_t *client)
{
    DEBUG_S3("Debut lireReqServ : New thread pour idxClient <%d> socket <%d> pseudo <%s>\n", client->idx, client->socket, client->pseudo);
    buffer_t msgLu;
    req_t req;
    int lenLu = 1;
    unsigned idPartieEnCours = 0;
    unsigned partieEnCours = NOT_OK;

    // On attend les inputs du Client Maitre
    while (lenLu > 0)
    {
        afficherCLient();
        afficherPartie(nbPartie);

        lenLu = lireMsgTCP(client->socket, msgLu, sizeof(buffer_t));
        DEBUG_S1("Serveur : message reçu len <%d>\n", lenLu);
        if (lenLu > 0)
        {
            strTOreq(&req, msgLu);
            DEBUG_S3("Serveur : socket <%i> msg recu <%s> avec idReq <%d>\n", client->socket, msgLu, req.idReq);

            switch (req.idReq)
            {
            case CONNECTERCLIENT:
                DEBUG_S("Case ConnecterClient");
                serveurConnecterClientRep(client->socket, &req, client);
                break;
            case CREERPARTIE:
                DEBUG_S("Case CreerPartie");
                partieEnCours = serveurCreatePartieRep(client->socket, &req, &idPartieEnCours, listePartie, &nbPartie, &mutex);
                break;
            case LISTERPARTIE:
                DEBUG_S("Case GetPartie");
                serveurGetPartiesRep(client->socket, listePartie, nbPartie, &mutex);
                break;
            case STATUTPARTIE:
                DEBUG_S("Case STATUTPARTIE");
                updateStatutPartieRep(req.msgReq, listePartie, nbPartie, &mutex);
                break;
            default:
                DEBUG_S("Case Default");
                break;
            }
        }
    }

    DEBUG_S2("Serveur : le client idx <%d> avec la socket <%d> a quitté la connexion\n", client->idx, client->socket);
    if ((listePartie[idPartieEnCours].statut == RUNNING) ||
        (listePartie[idPartieEnCours].statut == WAITTINGADVERSE))
        listePartie[idPartieEnCours].statut = CLOSED;

    // Le Client a fermé on cloture la socket client
    fermerSocket(client->socket);
    client->statut = OFFLINE;
    afficherCLient();
    afficherPartie(nbPartie);
}

// initialisation
void initstatPartie(void)
{
    nbPartie = 0;
}

void afficherCLient()
{
    int i = 0;
    printf("Liste clients connectés:\n");
    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");
    for (i = 0; i < nbClient; i++)
    {
        if (listeClient[i].statut == ONLINE)
            printf("%s\n", listeClient[i].pseudo);
    }
    CHECK_T(sem_post(&mutex) == 0, "erreur post mutex");
}

#endif

/* ------------------------------------------------------------------------ */
/*      FONCTION SERVEUR    & CLIENT                                        */
/* ------------------------------------------------------------------------ */
void afficherPartie(unsigned nbPart)
{
    int i = 0;
    statPartie_t *partie;

    printf("Liste parties disponibles:\n");
#ifdef SERVER
    CHECK_T(sem_wait(&mutex) == 0, "erreur attente mutex");
#endif
    for (i = 0; i < nbPart; i++)
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
    DEBUG_S1("Client fermeture socket <%d>\n", sockServer);
    fermerSocket(sockServer);         // Numero Socket serveur
    fermerSocket(socketEcoutePartie); // Numero Socket Client Maitre
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
        fermerSocket(listeClient[i].socket);
        DEBUG_S1("Serveur fermeture socket cliente <%d>\n", listeClient[i].socket);
    }
    exit(0);
#endif
}
