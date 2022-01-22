#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#define PORT_SVC 5000 
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);} 
#define MSG "100:Je dis que \"le fond de l’eau est clair par ici ! Où ça ?\"" 
#define MAX_BUFF 1024 
//TCP->stream
    int creerSocketEcoute(int Long);
    void accepterConnexion(struct sockaddr_in clt, int sock);
    void etablirConexion(struct sockaddr_in clt, int sock);
    void ecrireMsgTCP(int sock);
    void lireMsgTCP(int sock);
//UDP->datagrame
    int creerSocketUDP(in_addr_t addrdest,int port,struct sockaddr_in * serv);
    int creerSocketUDPAdr();
    void ecrireMsgUDP(struct sockaddr_in clt, int sock,char *msg);
    void lireMsgUDP(struct sockaddr_in clt, int sock);
