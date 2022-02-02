#include "data.h"
const char *statutPartieTxt[]={"RUN ", "STOP", "FAIL", "WAIT"};

void adresseTOstr(adresse_t *adr,char *dest)
{
    sprintf(dest,"%s:%d:%s",adr->ip, adr->port, adr->pseudo);
}

void strTOadresse(adresse_t *adr,char *dest)
{
    sscanf(dest,"%[^:]:%d:%[^:]",adr->ip, &adr->port, adr->pseudo);
}

void reqTOstr(req_t *req,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",req->idReq,req->lgreq,req->msgReq);
}
void strTOreq(req_t *req,buffer_t buff){
    sscanf(buff,FMT_REQ,(short int*)&req->idReq,&req->lgreq,req->msgReq);
}

void repTOstr(rep_t *rep,buffer_t buff){
    sprintf(buff,"%3d:%d:%s",rep->idRep,rep->lgrep,rep->msgRep);
}
void strTOrep(rep_t *rep,buffer_t buff){
    sscanf(buff,FMT_REQ,&rep->idRep,&rep->lgrep,rep->msgRep);
}

/*fonction caste des data imbriqué*/
void obstTOstring(char *dest,const int * obstacles){
	for(int i=0;i<NBMAXOBSTACLES;i++){
		dest[i]=obstacles[i]+'0';
	}
	dest[NBMAXOBSTACLES]='\0';

}

void stringTOobst(int *dest,const char * obstaclesch){
	for(int i=0;i<(int)strlen(obstaclesch);i++){
		dest[i]=obstaclesch[i]-'0';
	}
}

void strutToString(statPartie_t* tableau, char ch[NBMAXOBSTACLES])
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

    // ajout du pseudo du maitre
    strcat(ch,tableau->addrMaitre.pseudo);
    strcat(ch,";");

    // ajout adresse ip adverse
    strcat(ch,tableau->addrAdverse.ip);
    strcat(ch,";");

    // ajout du port de l'adverse
    sprintf(aux,"%d", tableau->addrAdverse.port);
    strcat(ch,aux);
    strcat(ch,";");

    // ajout du pseudo de l'adverse
    strcat(ch,tableau->addrAdverse.pseudo);
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

void stringToStruct(statPartie_t* tableau2, char ch[NBMAXOBSTACLES])
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

    
    // le pseudo du maitre 
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    strcpy(tableau2->addrMaitre.pseudo,aux);
    
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

    // le pseudo de l'adverse
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    strcpy(tableau2->addrAdverse.pseudo,aux);

    


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

void timeTostring(char *timeDataRep, time_t temps){
	sprintf(timeDataRep,"%lu",temps);
}

void stringToTime(time_t *temps,char *timeDataRep){
	*temps= atol(timeDataRep);
}