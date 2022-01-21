#define FMT_REQ "%3hd:%hd:%[^n]"
//typedef buffer_t char[MAX_BUFF];
typedef char* buffer_t;
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
