#include "cltSrv.h"
//#define CLIENT
#ifdef CLIENT
int main(/*int argc, char const *argv[]*/)
{
    //TODO
        char myPseudo[255];
        //system("./scriptZoom.sh -m");
        //system("./scriptZoom.sh -p");
        int choix=4;

        // est ce qu'on fait une fonction pour la récupération d'ip ? 
        //char ip[30]=recupererIp(); 

     
        draw_ascii(empty_picture(' '));
        printf("veuillez saisir votre pseudo pour vous connecter:\n");
        scanf("%s",myPseudo);
        choix=1;

        while (choix!=3)
        { 
            //draw_ascii(empty_picture(' '));
            printf("Menu:\n1)Lister les parties en cours\n2)Creer une partie\n3)Quitter\n");
            scanf("%d",&choix);
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

void clientMaitre(char * myPseudo){
    //envois creation party dgram
    printf("debut client maitre\n");
    struct sockaddr_in serv;
    int sock =creerSocketUDP(ADDRSERVERENR,PORT_SVC,&serv);
    req_t req;
    createPartyReq(&req,myPseudo);
    buffer_t buff;
    reqTOstr(&req,buff);
    ecrireMsgUDP(serv, sock,buff);
    printf("fin client maitre\n");
    //void lireMsgUDP(struct sockaddr_in clt, int sock);
};


void clientAdverse(char* myPseudo){
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
#define SERVER
#ifdef SERVER
    int main(/*int argc, char const *argv[]*/)
    {
        adresse_t clients[NBMAXCLIENT];

        //socket d'ecoute
        struct sockaddr_in serv;
        int sock =creerSocketUDPAdr(&serv);
        //client emeteur
        struct sockaddr_in clt;
        while (1)
        {
            req_t req = lireMsgUDP(sock,&clt);
            lireReqServ(req,&clt,sock);
        }
        return 0;
    }
#endif
