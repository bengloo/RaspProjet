  
#include "session.h"
//TCP
    int creerSocketEcoute(unsigned int port){

        int sock;
        struct sockaddr_in serv;
        
        // Création de la socket de réception d’écoute des appels
        CHECK(sock=socket(PF_INET, SOCK_STREAM, 0), "Can't create");
        
        // Préparation de l’adressage du service (d’appel)
        serv.sin_family = PF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = INADDR_ANY;
        memset(&serv.sin_zero, 0, 8);
        
        // Association de l’adressage préparé avec la socket d’écoute
        CHECK(bind(sock, (struct sockaddr *) &serv, sizeof serv) , "Can't bind TCP");
        
        // Mise en écoute de la socket
        CHECK(listen(sock, MAX_SOCK_BACKLOG), "Can't calibrate");
        
        return sock;

    };

    // Unsued
    void accepterConnexion(struct sockaddr_in clt, int sock){

       	int sd;
        socklen_t cltLen;
	    cltLen = sizeof(clt);
    
	    CHECK(sd=accept(sock, (struct sockaddr *)&clt, &cltLen) , "Can't connect");

    };

    int creerSocketClient(unsigned int port, char *addrIp){
        int sock;
        struct sockaddr_in svc;

        // Création de la socket d’appel et de dialogue
        CHECK(sock=socket(PF_INET, SOCK_STREAM, 0), "Can't create");

        // Préparation de l’adressage du service à contacter
        svc.sin_family = PF_INET;
        svc.sin_port = htons (port);
        svc.sin_addr.s_addr = inet_addr(addrIp);
        memset(&svc.sin_zero, 0, 8);

        // Demande d’une connexion au service
        CHECK(connect(sock, (struct sockaddr *)&svc, sizeof svc) , "Can't connect");
        return sock;

    };

    // Unsued
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
    int creerSocketUDP(char* addrdest,int port,struct sockaddr_in *serv){
        int sock;
        // Création de la socket de réception des requêtes
        CHECK(sock=socket(PF_INET, SOCK_DGRAM, 0), "Can't create");
        
        // Préparation de l’adressage du service
        serv->sin_family = PF_INET;
        serv->sin_port = htons(port);
        serv->sin_addr.s_addr = inet_addr(addrdest);
        memset(&(serv->sin_zero), 0, 8);
        printf("Client installé avec l'addr %s:%d\n",inet_ntoa(serv->sin_addr),ntohs(serv->sin_port));
        return sock;
    };


    int creerSocketUDPAdr(struct sockaddr_in *serv){
        int sock;
        
        // Création de la socket de réception des requêtes
        CHECK(sock=socket(PF_INET, SOCK_DGRAM, 0), "Can't create");
        
        // Préparation de l’adressage du service
        serv->sin_family = PF_INET;
        serv->sin_port = htons(PORT_SVC);
        serv->sin_addr.s_addr = INADDR_ANY;
        memset(&(serv->sin_zero), 0, 8);
        printf("Server installé avec l'addr %s:%d\n",inet_ntoa(serv->sin_addr),ntohs(serv->sin_port));
        //printf("serveur en écoute  en :%i,%s,%s\n",port,serv->sin_addr.s_addr ,"127.0.0.1");
        // Association de l’adressage préparé avec la socket créée
        CHECK(bind(sock, (struct sockaddr *) serv, sizeof *serv) , "Can't bind UDP");
        
        return sock;
    };


    void ecrireMsgUDP(struct sockaddr_in clt, int sock,char *msg){
    struct sockaddr_in moi;
    int LenMoi=sizeof(moi);
        // Ecriture du message
	printf("Envoi du message [%s]\n", msg);
	CHECK(sendto(sock, msg, strlen(msg)+1, 0, (struct sockaddr *)&clt, sizeof clt), "Can't send");
    CHECK(getsockname(sock,(struct sockaddr*)&moi,&LenMoi),"--getsockname()--");
    printf("j'ai envoyé le message \"%s\" à %s:%d, via %s :%d\n",msg,inet_ntoa(clt.sin_addr),ntohs(clt.sin_port),inet_ntoa(moi.sin_addr),ntohs(moi.sin_port));
    };


    req_t lireMsgUDP(int sock,struct sockaddr_in *clt){

        char reponse[MAX_BUFF]; 

        socklen_t cltLen = sizeof(clt);
            
        // Réception d’un message
        CHECK(recvfrom(sock, reponse, sizeof(reponse), 0,(struct sockaddr *)clt, &cltLen) , "Can't receive");
        printf("Message reçu [%s] de [%s]\n",reponse,inet_ntoa(clt->sin_addr)); 
        req_t req;
        //rep.idRep=strTOrep(reponse);
        return req;
    };


