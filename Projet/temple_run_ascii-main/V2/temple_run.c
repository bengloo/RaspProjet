
#include "graphisme.h"
#include <string.h>
#include <pthread.h>


//parametre réglable à metre dans basic fonction
#define PATH_WIDTH 1
#define Y_BORDER 0.7
#define SIGHT 10	// how far you can see (roughly)
#define GRAVITY 30
#define JUMP_SPEED 8
#define SPEED_INCREASE 0.1
#define NBMAXOBSTACLES 300

//pour data.c
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
void timeTostring(char *timeDataRep, time_t temps){
	sprintf(timeDataRep,"%lu",temps);
};

void stringToTime(time_t *temps,char *timeDataRep){
	*temps= atol(timeDataRep);
};

int main(void) {
	//------CLIENT----MAITRE-------
	//generation des obsacle et top depart
	//time_t now = time( NULL);
	int * obstaclesInitiaux=init_obstacles(NBMAXOBSTACLES);
	//caste data
	char obstDataRep[NBMAXOBSTACLES+1];
	char timeDataRep[10];
	for(int i=0;i<NBMAXOBSTACLES;i++){
		printf("%c",obstaclesInitiaux[i]);
	}
	/*obstTOstring(obstDataRep,obstaclesInitiaux);
	timeTostring(timeDataRep,now+9);

	//------CLIENT----ADVERSE-------
	//uncastdata
	int obstRecus[NBMAXOBSTACLES+1];
	time_t timeRecus;
	stringTOobst(obstRecus,obstDataRep);
	printf("ch:%s\n",obstDataRep);
	stringToTime(&timeRecus,timeDataRep);
	//init variable globale servant au req
	int mon_score=0;
	int son_score=0;
	/*char **pic = empty_picture(' ');
	//on lence la partie 
	system("./scriptZoom.sh -m");
	//draw_ascii_score(empty_picture('?'),mon_score,son_score);
	partie(obstRecus,&mon_score,&son_score,pic,&timeRecus);
	system("./scriptZoom.sh -p");
	printf("mon score:%d son score:%d\n", mon_score,son_score);*/
}

