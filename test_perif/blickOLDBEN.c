#define BTN 6
#define BUZZER 1

#define PUD_UP 
int main (int argc,char ** argv){
    int ledPin=0;

    if (argc!=1){
        ledPin=atoi(argv[1]);
    }
    wiringPiSetup();
    pinMode(BTN,INPUT);
    pinMode(BUZZER,OUTPUT);
    //pinMode(LEDPIN,OUTPUT);

    pullUpDnControl(BTN,PUD_UP);

    for(;;){

        //TODO
    }

}