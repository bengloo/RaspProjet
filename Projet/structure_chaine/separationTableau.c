#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// dans data.h 

#define ADDR_MAITRE "127.0.0.0"
#define ADDR_ADVERSE "127.0.0.1"


#define PORT_MAITRE 5000
#define PORT_ADVERSE 5001

typedef struct {
    char ip[10];
    int port;
}adresse_t;

typedef enum{
    RUNNING = 1,
    STOPPED = 2,
    FAILED = 3,
    WAITTINGADVERSE = 4
} statutPartie_t;

typedef struct {
  int id;
  statutPartie_t statut;
  adresse_t addrMaitre;
  adresse_t addrAdverse;
  int scoreMaitre;
  int scoreAdverse;

} statPatie_t;


void initstatPartie(statPatie_t * tableau);
void strutToString(statPatie_t * tableau, char ch[300]);
void stringToStruct(statPatie_t * tableau, char ch[300]);


int main() 
{
    statPatie_t tableau, tableau2; 
    initstatPartie(&tableau);


    // affichage de la structure créer dans l'init
    printf("\n Structure statutPartie_t initialisée : \n");
    printf("\n l'id : %d \n", tableau.id);
    printf("\n le statut : %d \n", tableau.statut);

    printf("\n l'adresse ip maitre est : %s\n",tableau.addrMaitre.ip);
    printf("\n le port du maitre est %d\n", tableau.addrMaitre.port);

    printf("\n l'adresse ip adverse est : %s\n",tableau.addrAdverse.ip);
    printf("\n le port de l'adverse est %d\n", tableau.addrAdverse.port);


    printf("\n le score du maitre est : %d \n", tableau.scoreMaitre);
    printf("\n le score de l'adverse est : %d \n", tableau.scoreAdverse);


    char ch[300];

    strutToString(&tableau, ch);
    
    // affichage de la chaine creer a partir de la structure
    // id;statut;ipMaitre;portMaitre;ipAdverse;portAdverse;scoreMaitre;scoreAdverse
    printf("\n la chaine créée à partir de la structure : %s \n", ch);

    stringToStruct(&tableau2, ch);

    // affiche de la structure creer a partir de la chaine 
    printf("\n La structure créée à partir de la chaine: \n");

    printf("\n l'ip : %d \n", tableau2.id);
    printf("\n le statut: %d \n", tableau2.statut);

    printf("\n l'adresse ip maitre est : %s \n", tableau2.addrMaitre.ip);
    printf("\n le port du maitre est : %d \n", tableau2.addrMaitre.port);

    printf("\n l'adresse ip adverse est : %s \n", tableau2.addrAdverse.ip);
    printf("\n le port de l'adverse est : %d \n", tableau2.addrAdverse.port);

    printf("\n le score du maitre est : %d \n", tableau2.scoreMaitre);
    printf("\n le score du maitre est : %d \n", tableau2.scoreAdverse);


}


// initialisation de la structure statPatie_t
void initstatPartie(statPatie_t * tableau) 
{
    struct sockaddr_in serv, serv2;
    statutPartie_t statut;
    adresse_t addrM, addrA;

    /* creation de données de socket à copier */ 
    serv.sin_addr.s_addr = inet_addr(ADDR_MAITRE);
    serv.sin_port = htons(PORT_MAITRE);

    serv2.sin_addr.s_addr = inet_addr(ADDR_ADVERSE);
    serv2.sin_port = htons(PORT_ADVERSE);

    

    /* copie des données des sockets */
    strcpy(addrM.ip,inet_ntoa(serv.sin_addr));
    addrM.port=ntohs(serv.sin_port);

    strcpy(addrA.ip,inet_ntoa(serv2.sin_addr));
    addrA.port=ntohs(serv2.sin_port);


    tableau->id=1;
    tableau->statut=1;
    strcpy(tableau->addrMaitre.ip,addrM.ip);
    tableau->addrMaitre.port=addrM.port;
    strcpy(tableau->addrAdverse.ip,addrA.ip);
    tableau->addrAdverse.port=addrA.port;
    tableau->scoreMaitre=100;
    tableau->scoreAdverse=101;



}


// transformation de la structure statPatie_t en une chaine 
void strutToString(statPatie_t * tableau, char ch[300])
{
    char aux[300];

    // ajout de l'id a la chaine
    sprintf(ch, "%d", tableau->id);  
    strcat(ch,";");

    // ajout du statut a la chaine
    sprintf(aux, "%d", tableau->statut);
    strcat(ch,aux);
    strcat(ch,";");


    // ajout adresse ip maitre 
    strcat(ch,tableau->addrMaitre.ip);
    strcat(ch,";");

    // ajout du port du maitre
    sprintf(aux,"%d", tableau->addrMaitre.port);
    strcat(ch,aux);
    strcat(ch,";");

    // ajout adresse ip adverse
    strcat(ch,tableau->addrAdverse.ip);
    strcat(ch,";");

    // ajout du port du adverse
    sprintf(aux,"%d", tableau->addrAdverse.port);
    strcat(ch,aux);
    strcat(ch,";");

    // ajout du score du maitre
    sprintf(aux,"%d", tableau->scoreMaitre);
    strcat(ch,aux);
    strcat(ch,";");

    // ajout du score de l'adverse
    sprintf(aux,"%d", tableau->scoreAdverse);
    strcat(ch,aux);
    strcat(ch,";");

}


// transformation de la chaine en une structure statPatie_t
void stringToStruct(statPatie_t * tableau2, char ch[300])
{

    char aux[100];
    int pos=0;

    // récupération de l'id
    int i;
    for(i=0; ch[i]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    
    char *str;
    int value;
    value = strtol(aux,&str,10);
    tableau2->id=value;
    




    // le statut
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;
    
    value = strtol(aux,&str,10);
    tableau2->statut=value;




    // l'ip maitre 
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    strcpy(tableau2->addrMaitre.ip,aux);


    // le port du maitre 
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;
    
    value = strtol(aux,&str,10);
    tableau2->addrMaitre.port=value;

    

    
    
    // l'ip adverse
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    strcpy(tableau2->addrAdverse.ip,aux);
    

    // le port de l'adverse
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;
    
    value = strtol(aux,&str,10);
    tableau2->addrAdverse.port=value;

    


    // score maitre 
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    value = strtol(aux,&str,10);
    tableau2->scoreMaitre=value;
    


    // score adverse 
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    value = strtol(aux,&str,10);
    tableau2->scoreAdverse=value;
    


}