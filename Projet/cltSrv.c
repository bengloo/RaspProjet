#ifndef SESSION_H
    #define SESSION_H
    #include "session.h"
#endif
#ifndef DATA_H
    #define DATA_H    
    #include "data.h"
#endif
#ifndef PROTO_H
    #define PROTO_H
    #include "proto.h"
#endif
#ifndef CLTSRV_H
    #define CLTSRV_H
    #include "cltSrv.h"
#endif

#ifdef CLIENT
int main(/*int argc, char const *argv[]*/)
{
    //TODO
        char myPseudo[255];
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
    int sock creerSocketUDP(ADDRSERVERENR,PORT_SVC);
    //void ecrireMsgUDP(struct sockaddr_in clt, sock);
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
