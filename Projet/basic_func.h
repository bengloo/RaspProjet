/* ------------------------------------------------------------------------ */
/**
 *  \file       basic_func.h
 *  \brief      
 *
 *  \date       28 Novembre 2021
 *  \author     Morcq Alexandre
 *  \version    1.0
 *
 */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>

#ifndef __BASIC_FUNC_H__
#define __BASIC_FUNC_H__


/* ------------------------------------------------------------------------ */
/*                                                           */
/* ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------ */




/* ------------------------------------------------------------------------ */
/*                      I N C L U D E S                                     */
/* ------------------------------------------------------------------------ */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>


/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/*      CONSTANTES                                                          */
/* ------------------------------------------------------------------------ */
#define OK 1
#define NOT_OK 0
#define MAX_LEN 255
#define NBMAXCLIENT 100
#define PORT_SVC 5000 //Unused
#define PORT_SERVER 8123 //port du server d'enregistrement pour parlé auc client
// #define PORT_CLIENTMAITRE 8124 //port de n'importe quel client pour parlé au server d'enregistrement
#define PORT_CLIENTMAITRE_PARTIE 8125 //por du client maitre pour parlé au autre client
// #define PORT_CLIENTADVERSE_PARTIE 8126 //INUTILE : le client adverse parle au client maitre sur la socket maitre //port du client adverse pour parlé au client maitre
#define MAX_BUFF 1024 
#define MAX_SOCK_BACKLOG 5 // Nb de connexion en attente possible pendant le listen
#define ADDRSERVERENR "127.0.0.1"

/*  Parametre graphique */
#define PATH_WIDTH 1
#define Y_BORDER 0.7
#define SIGHT 10	// how far you can see (roughly)
#define GRAVITY 30
#define JUMP_SPEED 8
#define SPEED_INCREASE 0.1
#define NBMAXOBSTACLES 300
#define ANSI_RESET "\033[0m"
#define ANSI_BLACK  "\033[30m"
#define ANSI_RED  "\033[31m"
#define ANSI_GREEN  "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE  "\033[34m"
#define ANSI_PURPLE  "\033[35m"
#define ANSI_CYAN  "\033[36m"
#define ANSI_WHITE  "\033[37m"


/* ------------------------------------------------------------------------ */
/*      M A C R O - F O N C T I O N S                                       */
/* ------------------------------------------------------------------------ */
#define CHECK_T(sts,msg) if (!(sts)) { fprintf(stderr,"%s\n",msg); exit(-1); }
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);} 
#ifdef DEBUG
    #define DEBUG_I(i) printf("%d\n",i);
    #define DEBUG_S(s) printf("%s\n",s);
    #define DEBUG_S1(s,p1) printf(s,p1);
    #define DEBUG_S2(s,p1,p2) printf(s,p1,p2);
    #define DEBUG_S3(s,p1,p2,p3) printf(s,p1,p2,p3);
    #define DEBUG_S4(s,p1,p2,p3,p4) printf(s,p1,p2,p3,p4);
#else
    #define DEBUG_I(i) 
    #define DEBUG_S(s) (void)0;
    #define DEBUG_S1(s,p1) (void)0;
    #define DEBUG_S2(s,p1,p2) (void)0;
    #define DEBUG_S3(s,p1,p2,p3) (void)0;
    #define DEBUG_S4(s,p1,p2,p3,p4) (void)0;
#endif
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*      D É F I N I T I O N S   D E   T Y P E S                             */
/* ------------------------------------------------------------------------ */
typedef void * (*pf_t)(void *);
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*      P R O T O T Y P E S   D E   F O N C T I O N S                       */
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */



#endif
/* ------------------------------------------------------------- THE END -- */