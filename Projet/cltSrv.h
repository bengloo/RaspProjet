#ifndef __CLTSRV_H__
#define __CLTSRV_H__
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include "data.h"
#include "session.h"
#include "proto.h"
#include "graphisme.h"
#include "basic_func.h"


void clientMaitre(char *myPseudo);
void clientAdverse(char* myPseudo);

// Gestion de signaux
void installDeroute(int numSig, void (*pfct)(int)); // deroute un signal vers la funct en 2ème param = pointeur sur une fontion
void deroute(int numSig); // exemple de fonction qui sera appelee en cas de signal
void terminerProcess(void);

#ifdef CLIENT
int sock=0; // Numero Socket client 
#endif

#ifdef SERVER
int socketEcoute = 0; // Numero Socket serveur
int continuer=1;
#endif



#endif 