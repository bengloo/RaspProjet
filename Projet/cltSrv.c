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
        char myPseudo[255]
        int choix=4;
        printf("Veuillé saisr votre pseudo pour vous conecter:");
        scanf("%s",myPseudo);
        while (choix!=3)
        { 
            printf("Menu:\n1)Lister les parties en cours\n2)Creer une partie\n3)Quiter");
            scanf("%d",choix);
            switch (choix)
            {
            case 1:
                
                break;
            case 2:
                
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
#endif

#ifdef SERVER
int main(/*int argc, char const *argv[]*/)
{
    /* code */
    return 0;
}
#endif
