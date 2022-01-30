#include <pthread.h>
#include "cltSrv.h"


//#define CLIENT
#ifdef CLIENT
int main(/*int argc, char const *argv[]*/)
{
    //TODO
    char myPseudo[MAX_LEN];
    //system("./scriptZoom.sh -m");
    //system("./scriptZoom.sh -p");
    int choix = 4;

    // est ce qu'on fait une fonction pour la récupération d'ip ?
    //char ip[30]=recupererIp();

    draw_ascii(empty_picture(' '));
    printf("veuillez saisir votre pseudo pour vous connecter:\n");
    scanf("%s", myPseudo);
    choix = 1;

    while (choix != 3)
    {
        //draw_ascii(empty_picture(' '));
        printf("Menu:\n1)Lister les parties en cours\n2)Creer une partie\n3)Quitter\n");
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            clientAdverse(myPseudo);
            break;
        case 2:
            clientMaitre(myPseudo);
            break;
        case 3:
            break;

        default:

            break;
        }
    }

    return 0;
}

void clientMaitre(char *myPseudo)
{
    req_t req;
    int sock=0;
    //envois creation party dgram
    DEBUG_S("Debut client maitre\n");
    sock=creerSocketClient(PORT_SERVER, ADDRSERVERENR);

    createPartyReq(sock, myPseudo);

};

void clientAdverse(char *myPseudo){
    //envois creation getpartie dgram

    //SI liste
    //recupererliste  des partie (pseudo adresse)-> liste parties
    //aficher la liste des partie
    //choisire une partie via un indice dans la liste ou
    //0)REVENIR au menu principal
    //-1)Refresh
    //si >0
    //MENUE
    //1)STREAM
    //2)JOUER
    //SI CREER

    /*
    printf("debut client adverse\n");
    struct sockaddr_in serv;
    int sock =creerSocketUDP(ADDRSERVERENR,PORT_SVC,&serv);
    req_t req;


    // Récupération des parties (requete d'id 2) afin de les afficher   
    getPartiesReq(2,ip,myPseudo);
    //afficherParties();

    int idPartie, streamOuJoueur;

    printf("Choisissez une partie dans la liste ou tapez 0 pour revenir au menu principal ou bien -1 pour Refresh \n");
    scanf("%d",&idPartie); // choix déjà initialisé avant 

    if(idPartie<=0) {
        switch (idPartie)
        {
            case -1:
                // Refresh
                    
                break;
            case 0:
                // revenir au menu 
                
                break;
                
            default:

                break;
        }
    }

    else {

        printf("Menu : \n 1) pour regarder la partie n° %d \n 2) pour jouer dans la partie n° %d \n", idPartie, idPartie);
        scanf("%d",&idstreamOuJoueur);  
        switch (idstreamOuJoueur)
        {
            case 1:
                // Stream, requete d'id 9 
                streamReq(9,ip); 
                    
                break;
            case 2:
                // Joueur, requête d'id 3 
                joinPartieReq(3, ip, idPartie);
                //getStart();
                break;
                
            default:

                break;
        }


    }
        
    buffer_t buff;
    reqTOstr(&req,buff);
    ecrireMsgUDP(serv, sock,buff);
    printf("fin client adverse\n");

        */

};
#endif
//define SERVER
#ifdef SERVER
int main(/*int argc, char const *argv[]*/)
{
    int socketEcoute = 0;
    int socketClient = 0;
    socklen_t cltLen;
    struct sockaddr_in clt;

    pthread_t tid[NBMAXCLIENT];
    long i=0;
	int idxThread[NBMAXCLIENT];
    double * status;


    // On se met en ecoute sur le port Serveur
    socketEcoute = creerSocketEcoute(PORT_SERVER);

    while (1)
    {
        cltLen = sizeof(clt);
        CHECK(socketClient = accept(socketEcoute, (struct sockaddr *)&clt, &cltLen), "Can't connect"); //accept de recevoir mess
        DEBUG_S1("Nouvelle connexion <%i>\n", socketClient);
        CHECK_T(pthread_create (&tid[i], NULL, (pf_t)lireReqServ,
                                (void *)(&socketClient))==0, "Erreur pthread_create()");
        
        i++;
    }

}
#endif
