  
#include "session.h"
//TCP
    int creerSocketEcoute(int Long){

        int sock;
        struct sockaddr_in serv;
        
        // Création de la socket de réception d’écoute des appels
        CHECK(sock=socket(PF_INET, SOCK_STREAM, 0), "Can't create");
        
        // Préparation de l’adressage du service (d’appel)
        serv.sin_family = PF_INET;
        serv.sin_port = htons(PORT_SVC);
        serv.sin_addr.s_addr = INADDR_ANY;
        memset(&serv.sin_zero, 0, 8);
        
        // Association de l’adressage préparé avec la socket d’écoute
        CHECK(bind(sock, (struct sockaddr *) &serv, sizeof serv) , "Can't bind");
        
        // Mise en écoute de la socket
        CHECK(listen(sock, Long), "Can't calibrate");
        
        return sock;

    };


    void accepterConnexion(struct sockaddr_in clt, int sock){

       	int sd;
        socklen_t cltLen;
	    cltLen = sizeof(clt);
    
	    CHECK(sd=accept(sock, (struct sockaddr *)&clt, &cltLen) , "Can't connect");

    };


    void etablirConexion(struct sockaddr_in clt, int sock){

        CHECK(connect(sock, (struct sockaddr *)&clt, sizeof clt) , "Can't connect");

    };

    void ecrireMsgTCP(int sock) {
        // Ecriture du message
        printf("Envoi du message [%s]\n", MSG);
        write(sock, MSG, strlen(MSG)+1);
    }


    void lireMsgTCP(int sock){

        char reponse[MAX_BUFF]; 

        // Réception d’un message
	    read(sock, reponse, sizeof(reponse));

    };


//UDP
    int creerSocketUDP(char *addrdest,int port){
        int sock;
        struct sockaddr_in serv;
        
        // Création de la socket de réception des requêtes
        CHECK(sock=socket(PF_INET, SOCK_DGRAM, 0), "Can't create");
        
        // Préparation de l’adressage du service
        serv.sin_family = PF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = addrdest;
        memset(&serv.sin_zero, 0, 8);
        
        return sock;
    };


    int creerSocketUDPAdr(){
        int sock;
        struct sockaddr_in serv;
        
        // Création de la socket de réception des requêtes
        CHECK(sock=socket(PF_INET, SOCK_DGRAM, 0), "Can't create");
        
        // Préparation de l’adressage du service
        serv.sin_family = PF_INET;
        serv.sin_port = htons(PORT_SVC);
        serv.sin_addr.s_addr = INADDR_ANY;
        memset(&serv.sin_zero, 0, 8);
        
        // Association de l’adressage préparé avec la socket créée
        CHECK(bind(sock, (struct sockaddr *) &serv, sizeof serv) , "Can't bind");
        
        return sock;
    };


    void ecrireMsgUDP(struct sockaddr_in clt, int sock){

        // Ecriture du message
	printf("Envoi du message [%s]\n", MSG);
	CHECK(sendto(sock, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&clt, sizeof clt), "Can't send");

    };


    void lireMsgUDP(struct sockaddr_in clt, int sock){

        char reponse[MAX_BUFF]; 

        socklen_t cltLen = sizeof(clt);
            
        // Réception d’un message
        CHECK(recvfrom(sock, reponse, sizeof(reponse), 0,
        (struct sockaddr *)&clt, &cltLen) , "Can't receive");
        
    };


