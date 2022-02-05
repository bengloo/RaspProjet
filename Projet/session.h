#ifndef __SESSION_H__
#define __SESSION_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "reqRep.h"
#include "basic_func.h"

	void getMyIp(char*str);

	//TCP->stream
    int creerSocketEcoute(unsigned int port);
    int creerSocketClient(unsigned int port, char *addrIp);
    void fermerSocket(int sock);

    // Unused
    void accepterConnexion(struct sockaddr_in clt, int sock);
    void etablirConexion(struct sockaddr_in clt, int sock);


    void ecrireMsgTCP(int sock, char *msg);
    ssize_t  lireMsgTCP(int sock, char *reponse, int taille_max);

    
//UDP->datagrame
    int creerSocketUDP(char* addrdest,int port,struct sockaddr_in * serv);
    int creerSocketUDPAdr(struct sockaddr_in *serv);
    void ecrireMsgUDP(struct sockaddr_in clt, int sock,char *msg);
    req_t lireMsgUDP(int sock,struct sockaddr_in *clt);

#endif