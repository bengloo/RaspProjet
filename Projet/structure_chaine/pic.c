#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define Y_PIX 4	
#define X_PIX 3


void picToString(char ** pic, char ch[1000000], int mon_score, int son_score);
void stringToPic(char ** pic, char ch[1000000], int *mon_score, int *son_score);

    
    
char **empty_picture(char empty_char) {
	char **pic;
	pic = malloc(sizeof(char *) * Y_PIX);
	for (int i = 0; i < Y_PIX; ++i) {
		pic[i] = malloc(sizeof(char *) * X_PIX);
		for (int j = 0; j < X_PIX; ++j) {
			pic[i][j] = empty_char;
		}
	}
	return pic;
}

    
int main() {
    int mon_score=10;
	int son_score=11;
	char **pic = empty_picture(' ');
    //pic[Y_PIC][X_PIC]
    //pic vaut soit c, soit o, soit x, soit espace 

    pic[0][0]='c';
    pic[0][1]='c';
    pic[0][2]='x';

    pic[1][0]='o';
    pic[1][1]=' ';
    pic[1][2]='o';

    pic[2][0]=' ';
    pic[2][1]='c';
    pic[2][2]='c';

    pic[3][0]='c';
    pic[3][1]='c';
    pic[3][2]='c';




    printf("\n");
    for (int i = 0; i < Y_PIX; ++i) {
		for (int j = 0; j < X_PIX; ++j) {
			printf("%c", pic[i][j]);
		}
        printf("\n");
	}


    //char ch[Y_PIX*X_PIX];
    char ch[1000000];

    picToString(pic, ch, mon_score, son_score);
  


    printf("\n la chaine est %s", ch);

    return 0;

}


void picToString(char ** pic, char ch[1000000], int mon_score, int son_score) {

    char aux[10];

    sprintf(ch, "%d", mon_score);  
    strcat(ch,";");


    sprintf(aux, "%d", son_score);
    strcat(ch,aux);
    strcat(ch,";");



    int occ=1;
    char flag='b';




    for (int i = 0; i < Y_PIX; ++i) {
		for (int j = 0; j < X_PIX; ++j) {


            if (j == 0 && i ==0)
                flag = pic[0][0];


            else {
                if(flag==pic[i][j])
                    occ++;

                if (j == (X_PIX-1) && i == (Y_PIX-1)) {
                    sprintf(aux, "%d", occ);  
                    strcat(ch,aux);
                    strcat(ch,&flag);
                }
            

                if(flag!=pic[i][j]) {
                    sprintf(aux, "%d", occ);  
                    strcat(ch,aux);
                    strcat(ch,&flag);
                    flag=pic[i][j];
                    occ=1;
                }
            }
        }

	}

}


void stringToPic(char ** pic, char ch[1000000], int *mon_score, int *son_score) 
{
    char aux[100];
    int pos=0;

    // récupération mon_score
    int i;
    for(i=0; ch[i]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    char *str;
    int value;
    value = strtol(aux,&str,10);

    *mon_score=value;

    // récupération son_score
    int i;
    for(i=0; ch[pos]!=';';i++, pos++) {
        aux[i]=ch[pos];
    }
    aux[i]='\0';
    pos++;

    value = strtol(aux,&str,10);

    *son_score=value;

}