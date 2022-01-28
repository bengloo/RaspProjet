#define ADDRSERVERENR "127.0.0.1"
#define FMT_REQ "%3hd:%hd:%[^n]"
#define MAX_BUFF 1024
#define OK 1;
#define NOP 0;
typedef char buffer_t[MAX_BUFF];
//typedef char* buffer_t;
typedef struct 
{
    short idReq;//0 à 999
    short lgreq;//0 à 1024
    buffer_t msgReq;
} req_t;

typedef struct 
{
    short idRep;//0 à 999
    short lgrep;//0 à 1024
    buffer_t msgRep;
} rep_t;
