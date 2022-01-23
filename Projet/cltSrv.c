#include "session.h"
#include "data.h"
#include "proto.h"
#include "cltSrv.h"
#include <graphics.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
//#define CLIENT
#ifdef CLIENT
int main(/*int argc, char const *argv[]*/)
{
    //TODO
        char myPseudo[255];
        //system("ifconfig |grep broadcast |cut -d' ' -f10");
        int choix=4;

        // est ce qu'on fait une fonction pour la récupération d'ip ? 
        char ip[30]=recupererIp(); 


        draw_ascii(empty_picture(' '));
        printf("veuillez saisir votre pseudo pour vous connecter:\n");
        scanf("%s",myPseudo);
        while (choix!=3)
        { 
            draw_ascii(empty_picture(' '));
            printf("Menu:\n1)Lister les parties en cours\n2)Creer une partie\n3)Quitter\n");
            scanf("%d",&choix);
            switch (choix)
            {
            case 1:
                clientAdverse();
                break;
            case 2:
                clientMaitre();
                break;
            case 3:
                break;
            
            default:

                break;
            }
        }
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

        if(/*Liste*/) {  
            // Récupération des parties (requete d'id 2) afin de les afficher   
            getPartiesReq(2,ip,myPseudo);
            //afficherParties();

            int idPartie, streamOuJoueur;

            printf("Choisissez une partie dans la liste ou tapez 0 pour revenir au menu principal ou bien -1 pour Refresh \n");
            scanf("%d",&idPartie); // choix déjà initialisé avant 

            if(idPartie<0) {
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
        }

        if(/*creer*/) {
            // demander les paramètres de jeu 
            createPartieReq(1, ip, /*param*/);
        }
            
    
    return 0;
}

void clientMaitre(){
    struct sockaddr_in serv;
    int sock =creerSocketUDP((in_addr_t)ADDRSERVERENR,PORT_SVC,&serv);
    req_t req;
    createPartyReq(&req);
    buffer_t buff;
    reqTOstr(&req,buff);
    ecrireMsgUDP(serv, sock,buff);
    //void lireMsgUDP(struct sockaddr_in clt, int sock);
};
void clientAdverse(){

};
#endif

#ifdef SERVER
int main(/*int argc, char const *argv[]*/)
{
    /* code */
    return 0;
}
#endif
