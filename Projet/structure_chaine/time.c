// recuperation timestamp
// ajout de 5 secondes 
// le casté en chaine 
// le remettre en structure 
// savoir faire un décompte 

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
struct tm {
   int tm_sec;         // seconds,  range 0 to 59          
   int tm_min;         // minutes, range 0 to 59           
   int tm_hour;        // hours, range 0 to 23             
   int tm_mday;        // day of the month, range 1 to 31  
   int tm_mon;         // month, range 0 to 11             
   int tm_year;        // The number of years since 1900  
   int tm_wday;        // day of the week, range 0 to 6    
   int tm_yday;        // day in the year, range 0 to 365  
   int tm_isdst;       // daylight saving time            
};
*/
int main(void) 
{ 
    time_t t;
    struct tm *info;
    time(&t); 
    
    // transformation du time_t en une structure tm afin d'utiliser chacune des données
    info=localtime(&t);

    // info est une struct tm, asctime pour transformer info en une chaine 
    printf("time : %s", asctime(info));
  
    int min=0, sec;

    // stockage des secondes + 10 
    if(info->tm_sec<50)
        sec=info->tm_sec+10;

    else {
        int secAux = 59-info->tm_sec;
        min=info->tm_min+1;
        sec=9-secAux;
    }

    printf("time + 10 sec : %d min et %d sec", min, sec);

    int decompte=sec-info->tm_sec;
    int decompteAux=decompte;

    
    if(min==0) {
        
        printf("\nDépart dans %d secondes \n", decompte);
        // recupération tout le temps de l'heure actuelle
        while(decompte!=0) {
            time(&t);
            info=localtime(&t);

            // recalcul du decompte (avec les secondes) a chaque fois
            decompte=sec-info->tm_sec;

            // si le decompte à changer alors on l'affiche 
            if(decompteAux!=decompte)
                printf("Attention au départ dans %d secondes \n", decompte);

            decompteAux=decompte;

        }
    }

    else{
        // décomposition des minutes en seconde
        int timeSecAux=min*60+sec;
        int timeSec;
        int decompte=timeSecAux-(info->tm_min*60+info->tm_sec);
        int decompteAux=decompte;

        printf("\nDépart dans %d secondes \n", decompte);
        
        // recupération tout le temps de l'heure actuelle
        while(decompte!=0) {
            time(&t);
            info=localtime(&t);

            timeSec=info->tm_min*60+info->tm_sec;

            // recalcul du decompte (avec les secondes) a chaque fois
            decompte=timeSecAux-timeSec;

            // si le decompte à changer alors on l'affiche 
            if(decompteAux!=decompte)
                printf("Attention au départ dans %d secondes \n", decompte);

            decompteAux=decompte;

        }
    }
  
    return 0; 
}


