#include "session.h"
#include "data.h"
#include "proto.h"
#include "cltSrv.h"
#include "graphisme.h"
#include <unistd.h>
#include <netdb.h>
//#define CLIENT
#ifdef CLIENT
int main(/*int argc, char const *argv[]*/)
{
    //TODO
        char myPseudo[255];
        char monip[256];
        char s[256];
     
        if (!gethostname(s, sizeof s))
        {
            printf ("Machine: %s\n", s);
            {
                struct hostent *host= gethostbyname(s);
                
                if (host  != NULL)
                {
                    struct in_addr **adr;
                    
                    for (adr = (struct in_addr **)host->h_addr_list; *adr; adr++)
                    {
                        printf("IP : %s\n", inet_ntoa(**adr));
                    }
                }
            }
        } 
        /*
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
        }*/
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
    /*createPartyReq();
    struct sockaddr_in serv;
    int sock =creerSocketUDP((in_addr_t)ADDRSERVERENR,PORT_SVC,serv);
    req_t req= createPartyReq();
    buffer_t buff;
    reqTOstr(&req,buff);
    ecrireMsgUDP(serv, sock,buff);*/
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
