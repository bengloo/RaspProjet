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
        //saisir pseudo
        //menu
          //1)lister les parte 
          //2)crer une partie
          //3)QUITER 

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
