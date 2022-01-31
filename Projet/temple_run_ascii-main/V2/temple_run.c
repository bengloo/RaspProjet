
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

int main(void) {
	//generation des obsacle
	int * obstaclesInitiaux=init_obstacles(NBMAXOBSTACLES);
	char obstDataRep[NBMAXOBSTACLES+1];
	obstTOstring(obstDataRep,obstaclesInitiaux);
	//printf("%s\n%ld obstacle\n",obstDataRep,strlen(obstDataRep));
	//on envois avec stream les data en chaine de caractére...

	int obstRecus[NBMAXOBSTACLES+1];
	stringTOobst(obstRecus,obstDataRep);
	/*for(int i=0;i<NBMAXOBSTACLES;i++){
		printf("%d",obstRecus[i]);
	}*/

	//on lence la partie client
	int mon_score=0;
	int son_score=0;
	//pthread_t tidpartie,serverpartie;
	//on encapsule la partie dans un thread
	system("./scriptZoom.sh -m");
	//draw_ascii_score(empty_picture('?'),123,456);
	partie(obstRecus,&mon_score,&son_score);
	system("./scriptZoom.sh -p");
	printf("mon score:%d son score:%d\n", mon_score,son_score);
}

