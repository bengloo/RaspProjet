#ifndef SESSIONH
    #include "session.h"
    #define  SESSIONH
#endif
#ifndef DATAH
    #include "data.h"
    #define DATAH
#endif
#include "proto.h"
#include "graphisme.h"
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#define NBMAXCLIENT 100

void clientMaitre(char *myPseudo);
void clientAdverse();
