#include "session.h"
#include "data.h"
#include "proto.h"
#include "cltSrv.h"
#include "graphisme.h"
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
        draw_ascii(empty_picture(' '));
        printf("Veuillé saisr votre pseudo pour vous conecter:\n");
        scanf("%s",myPseudo);
        while (choix!=3)
        { 
            draw_ascii(empty_picture(' '));
            printf("Menu:\n1)Lister les parties en cours\n2)Creer une partie\n3)Quiter\n");
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
