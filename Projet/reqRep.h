#define MAX_BUFF 10
#define FMT_REQ "%3d:%d:%[^n]"
//typedef buffer_t char[MAX_BUFF];
typedef char* buffer_t;
typedef struct req
{
    short idReq;//0 à 999
    short lgreq;//0 à 1024
    buffer_t msgReq;
} req_t;

typedef struct rep
{
    short idRep;//0 à 999
    short lgrep;//0 à 1024
    buffer_t msgRep;
} rep_t;
