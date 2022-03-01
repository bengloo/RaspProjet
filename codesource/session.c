/* ------------------------------------------------------------------------ */
/**
 *  \file       session.c
 *  \brief      Contient l'ensemble des fonctions de gestion des sockets
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
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "session.h"

/**
 *  \brief recupere adresse IP
 *  
 *  \param [in] str chaine pour stocker l'IP
 *  \return void
 */
void getMyIp(char *str)
{
    struct ifaddrs *addr, *intf;
    char hostname[NI_MAXHOST];
    int family, s;

    *str = '\0';

    if (getifaddrs(&intf) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (addr = intf; addr != NULL; addr = addr->ifa_next)
    {
        family = addr->ifa_addr->sa_family;
        //AF_INET est la famille d'adresses pour IPv4
        if (family == AF_INET)
        {
            //getnameinfo permet la résolution de noms.
            s = getnameinfo(addr->ifa_addr,
                            sizeof(struct sockaddr_in),
                            hostname,
                            NI_MAXHOST,
                            NULL,
                            0,
                            NI_NUMERICHOST);
            DEBUG_S2("Host <%s> IP<%s>\n", addr->ifa_name, hostname);
            if (strcmp(addr->ifa_name, "lo") != 0)
                strcpy(str, hostname);
        }
    }
}

/**
 *  \brief Permet de creer une socket en ecoute
 *  
 *  \param [in] port numero de port d'ecoute
 *  \return numero de socket creee
 */
int creerSocketEcoute(unsigned int port)
{

    int sock;
    struct sockaddr_in serv;

    // Création de la socket de réception d’écoute des appels
    CHECK(sock = socket(PF_INET, SOCK_STREAM, 0), "Can't create");

    // Préparation de l’adressage du service (d’appel)
    serv.sin_family = PF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    memset(&serv.sin_zero, 0, 8);

    // Association de l’adressage préparé avec la socket d’écoute
    CHECK(bind(sock, (struct sockaddr *)&serv, sizeof serv), "Can't bind TCP");

    // Mise en écoute de la socket
    CHECK(listen(sock, MAX_SOCK_BACKLOG), "Can't calibrate");

    return sock;
};

/**
 *  \brief accepter les connexions sur une socket
 *  
 *  \param [in] clt structure du client qui se connecte
 *  \param [in] sock socket
 *  \return void
 */
void accepterConnexion(struct sockaddr_in clt, int sock)
{

    int sd;
    socklen_t cltLen;
    cltLen = sizeof(clt);

    CHECK(sd = accept(sock, (struct sockaddr *)&clt, &cltLen), "Can't connect");
};
/**
 *  \brief Permet de se connecter à une socket à distance
 *  
 *  \param [in] port port de connexion à utliser
 *  \param [in] addrIp adresse ip de connexion à utliliser 
 *  \return numero de socket connectee
 */
int creerSocketClient(unsigned int port, char *addrIp)
{
    int masock;
    struct sockaddr_in svc;

    // Création de la socket d’appel et de dialogue
    CHECK(masock = socket(PF_INET, SOCK_STREAM, 0), "Can't create");

    // Préparation de l’adressage du service à contacter
    svc.sin_family = PF_INET;
    svc.sin_port = htons(port);
    svc.sin_addr.s_addr = inet_addr(addrIp);
    memset(&svc.sin_zero, 0, 8);

    // Demande d’une connexion au service
    CHECK(connect(masock, (struct sockaddr *)&svc, sizeof svc), "Can't connect");
    return masock;
};
/**
 *  \brief Fermer une socket
 *  
 *  \param [in] sock numero de socket a fermer
 *  \return void
 */
void fermerSocket(int sock)
{
    if (sock != 0)
        close(sock);
}

/**
 *  \brief Permet d'etablir une connexion
 *  
 *  \param [in] clt structure du client qui se connecte
 *  \param [in] sock socket
 *  \return void
 */
void etablirConexion(struct sockaddr_in clt, int sock)
{

    CHECK(connect(sock, (struct sockaddr *)&clt, sizeof clt), "Can't connect");
}

/**
 *  \brief Envoyer un message sur une socket
 *  
 *  \param [in] sock socket a utliser pour l'envoi
 *  \param [in] msg message à envoyer
 *  \return void
 */
void ecrireMsgTCP(int sock, char *msg)
{
    // Ecriture du message
    DEBUG_S1("Envoi du message [%s]\n", msg);
    write(sock, msg, strlen(msg) + 1);
}

/**
 *  \brief Lire un message sur une socket
 *  
 *  \param [in] sock socket a utliser pour la lecture
 *  \param [in] reponse chaine ou stocker le message reçu
 *  \param [in] taille_max taille maximum de la chaine ou stocker le message reçu
 *  \return void
 */
ssize_t lireMsgTCP(int sock, char *reponse, int taille_max)
{
    // Réception d’un message
    return read(sock, reponse, taille_max);
};

/**
 *  \brief Creer une socket UDP
 *  
 *  \param [in] addrdest adresse IP de destination
 *  \param [in] port port de destination
 *  \param [in] serv structure socket
 *  \return socket creee
 */
int creerSocketUDP(char *addrdest, int port, struct sockaddr_in *serv)
{
    int sock;
    // Création de la socket de réception des requêtes
    CHECK(sock = socket(PF_INET, SOCK_DGRAM, 0), "Can't create");

    // Préparation de l’adressage du service
    serv->sin_family = PF_INET;
    serv->sin_port = htons(port);
    serv->sin_addr.s_addr = inet_addr(addrdest);
    memset(&(serv->sin_zero), 0, 8);
    printf("Client installé avec l'addr %s:%d\n", inet_ntoa(serv->sin_addr), ntohs(serv->sin_port));
    return sock;
}

/**
 *  \brief Creer une socket UDP avec adresse default
 *  
 *  \param [in] serv structure socket
 *  \return socket creee
 */
int creerSocketUDPAdr(struct sockaddr_in *serv)
{
    int sock;

    // Création de la socket de réception des requêtes
    CHECK(sock = socket(PF_INET, SOCK_DGRAM, 0), "Can't create");

    // Préparation de l’adressage du service
    serv->sin_family = PF_INET;
    serv->sin_port = htons(PORT_SVC);
    serv->sin_addr.s_addr = INADDR_ANY;
    memset(&(serv->sin_zero), 0, 8);
    printf("Server installé avec l'addr %s:%d\n", inet_ntoa(serv->sin_addr), ntohs(serv->sin_port));
    //printf("serveur en écoute  en :%i,%s,%s\n",port,serv->sin_addr.s_addr ,"127.0.0.1");
    // Association de l’adressage préparé avec la socket créée
    CHECK(bind(sock, (struct sockaddr *)serv, sizeof *serv), "Can't bind UDP");

    return sock;
}

/**
 *  \brief Envoyer un message sur une socket UDP
 *  
 *  \param [in] clt client de destination
 *  \param [in] sock socket a utliser pour l'envoi
 *  \param [in] msg message à envoyer
 *  \return void
 */
void ecrireMsgUDP(struct sockaddr_in clt, int sock, char *msg)
{
    struct sockaddr_in moi;
    int LenMoi = sizeof(moi);
    // Ecriture du message
    printf("Envoi du message [%s]\n", msg);
    CHECK(sendto(sock, msg, strlen(msg) + 1, 0, (struct sockaddr *)&clt, sizeof clt), "Can't send");
    CHECK(getsockname(sock, (struct sockaddr *)&moi, &LenMoi), "--getsockname()--");
    printf("j'ai envoyé le message \"%s\" à %s:%d, via %s :%d\n", msg, inet_ntoa(clt.sin_addr), ntohs(clt.sin_port), inet_ntoa(moi.sin_addr), ntohs(moi.sin_port));
}

/**
 *  \brief Lire un message sur une socket
 *  
 *  \param [in] sock socket a utliser pour la lecture
 *  \param [in] clt client de reception
 *  \return requete
 */
req_t lireMsgUDP(int sock, struct sockaddr_in *clt)
{

    char reponse[MAX_BUFF];

    socklen_t cltLen = sizeof(clt);

    // Réception d’un message
    CHECK(recvfrom(sock, reponse, sizeof(reponse), 0, (struct sockaddr *)clt, &cltLen), "Can't receive");
    printf("Message reçu [%s] de [%s]\n", reponse, inet_ntoa(clt->sin_addr));
    req_t req;
    //rep.idRep=strTOrep(reponse);
    return req;
};
