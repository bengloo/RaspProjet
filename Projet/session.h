#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/in.h> 

#define PORT_SVC 5000 
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);} 
#define MSG "100:Je dis que \"le fond de l’eau est clair par ici ! Où ça ?\"" 
#define MAX_BUFF 1024

    //TCP
    void creerSocketEcoute(int Long);
    void accepterConnexion(struct sockaddr_in clt, int sock);
    void etablirConexion(struct sockaddr_in clt, int sock);
    void ecrireMsgTCP(int sock);
    void lireMsgTCP(int sock);
//UDP
    void creerSocketUDP(int Long);
    void creerSocketUDPAdr(int Long);
    void ecrireMsgUDP(struct sockaddr_in clt, int sock);
    void lireMsgUDP(struct sockaddr_in clt, int sock);
