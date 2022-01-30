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
#endif 