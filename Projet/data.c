/* ------------------------------------------------------------------------ */
/**
 *  \file       data.c
 *  \brief      Programme réalisant un executable serveur d'enregistrement et un executable client
 *              permettant de gerer des parties temple_run multijoueurs 
 *				table de villes chargée à partir d'un fichier.
 *
 *  \author     Morcq Alexandre B2
 *
 *  \date       08/02/2022
 *
 *	\version    1.0
 * *
 *	\remark		fichier contenant nos fonctions, ...
 *
 */
#include "data.h"
#include "basic_func.h"

const char *statutPartieTxt[] = {"EN COURS", "TERMINEE", "EN ECHEC", "ATTENTE ", "FERMEE  "};

void reqTOstr(req_t *req, buffer_t buff)
{
    sprintf(buff, FMT_REQSTR, req->idReq, req->lgreq);
    char *ch = buff;
    while ((*ch != '\0') && (*ch != ':'))
        ch++;
    ch++;
    CHECK((*ch != '\0'), "Erreur format de message");
    while ((*ch != '\0') && (*ch != ':'))
        ch++;
    ch++;
    CHECK((*ch != '\0'), "Erreur format de message");
    strncpy(ch, req->msgReq, req->lgreq);
    ch[req->lgreq] = '\0';
}
void strTOreq(req_t *req, buffer_t buff)
{
    sscanf(buff, FMT_REQ, (short int *)&req->idReq, &req->lgreq, req->msgReq);
}

void repTOstr(rep_t *rep, buffer_t buff)
{
    sprintf(buff, FMT_REPSTR, rep->idRep, rep->lgrep);
    char *ch = buff;
    while ((*ch != '\0') && (*ch != ':'))
        ch++;
    ch++;
    CHECK((*ch != '\0'), "Erreur format de message");
    while ((*ch != '\0') && (*ch != ':'))
        ch++;
    ch++;
    CHECK((*ch != '\0'), "Erreur format de message");
    strncpy(ch, rep->msgRep, rep->lgrep);
    ch[rep->lgrep] = '\0';
}

void strTOrep(rep_t *rep, buffer_t buff)
{
    sscanf(buff, FMT_REP, (int *)&rep->idRep, &rep->lgrep, rep->msgRep);
}

void adresseTOstr(adresse_t *adr, char *dest)
{
    sprintf(dest, "%s:%d:%s", adr->ip, adr->port, adr->pseudo);
}

void strTOadresse(adresse_t *adr, char *dest)
{
    sscanf(dest, "%[^:]:%d:%[^:]", adr->ip, &adr->port, adr->pseudo);
}

void statutReqTOstr(statutReq_t *adr, char *dest)
{
    sprintf(dest, "%d", adr->statut);
}

void strTOstatutReq(statutReq_t *adr, char *dest)
{
    sscanf(dest, "%d", &adr->statut);
}

void listePartieTOStr(statPartie_t *listePartie, unsigned nbPartie, char *dest)
{
    char *str = dest;
    //DEBUG_S1("listePartieTOStr : nbPartie <%d>\n", nbPartie);
    for (int i = 0; i < nbPartie; i++)
    {
        statPartie_t *partie = &listePartie[i];
        sprintf(str, "%d:%d:%s:%d:%s:%s:%d:%s:%d:%d:",
                partie->id,
                partie->statut,
                partie->addrMaitre.ip,
                partie->addrMaitre.port,
                partie->addrMaitre.pseudo,
                partie->addrAdverse.ip,
                partie->addrAdverse.port,
                partie->addrAdverse.pseudo,
                partie->scoreMaitre,
                partie->scoreAdverse);
        str += strlen(str);
    }
    //DEBUG_S1("listePartieTOStr : str <%s>\n", dest);
}
void partieTOstr(char *dest, statPartie_t *partie)
{
    sprintf(dest, "%d:%d:%s:%d:%s:%s:%d:%s:%d:%d:",
            partie->id,
            partie->statut,
            partie->addrMaitre.ip,
            partie->addrMaitre.port,
            partie->addrMaitre.pseudo,
            partie->addrAdverse.ip,
            partie->addrAdverse.port,
            partie->addrAdverse.pseudo,
            partie->scoreMaitre,
            partie->scoreAdverse);
}
void strTOpartie(statPartie_t *partie, char *dataTxt)
{
    sscanf(dataTxt, "%d:%d:%[^:]:%d:%[^:]:%[^:]:%d:%[^:]:%d:%d:",
           &partie->id,
           (int *)&partie->statut,
           partie->addrMaitre.ip,
           &partie->addrMaitre.port,
           partie->addrMaitre.pseudo,
           partie->addrAdverse.ip,
           &partie->addrAdverse.port,
           partie->addrAdverse.pseudo,
           &partie->scoreMaitre,
           &partie->scoreAdverse);
}

int StrTOlistePartie(statPartie_t *listePartie, char *dest)
{
    char *str = dest;
    unsigned nb = 0;
    while (strlen(str) != 0)
    {
        statPartie_t *partie = &listePartie[nb];
        sscanf(str, "%d:%d:%[^:]:%d:%[^:]:%[^:]:%d:%[^:]:%d:%d:",
               &partie->id,
               (int *)&partie->statut,
               partie->addrMaitre.ip,
               &partie->addrMaitre.port,
               partie->addrMaitre.pseudo,
               partie->addrAdverse.ip,
               &partie->addrAdverse.port,
               partie->addrAdverse.pseudo,
               &partie->scoreMaitre,
               &partie->scoreAdverse);

        // On avance jusqu'au 10eme :
        int cpt = 10;
        while ((*str != '\0') && (cpt > 0))
        {
            if (*str == ':')
                cpt--;
            str++;
        }
        nb++;
    }
    return nb;
}

void timeTostring(char *timeDataRep, time_t temps)
{
    sprintf(timeDataRep, "%lu", temps);
}

void stringToTime(time_t *temps, char *timeDataRep)
{
    *temps = atol(timeDataRep);
}
void partieTOstring(char *dest, const partieGraphique_t *partie)
{
    int i = 0;
    char *ch = dest;
    for (i = 0; i < NBMAXOBSTACLES; ++i)
    {
        sprintf(ch, "%d:", partie->obstacles[i]);
        ch += strlen(ch);
    }
    for (i = 0; i < NBMAXLIGNES; ++i)
    {
        sprintf(ch, "%d:%d:", partie->dist[i], partie->turn[i]);
        ch += strlen(ch);
    }
}

void stringTOpartie(partieGraphique_t *partie, const char *partiech)
{
    //DEBUG_S1("stringTOpartie debut partiech <%s>\n", partiech);
    int i = 0;
    char *ch = (char *)partiech;
    //DEBUG_S1("stringTOpartie debut ch <%s>\n", ch);
    for (i = 0; i < NBMAXOBSTACLES; ++i)
    {
        sscanf(ch, "%d:", &partie->obstacles[i]);
        //DEBUG_S2("stringTOpartie 1 obs <%d> ch <%s>\n", partie->obstacles[i], ch);
        while ((*ch != '\0') && (*ch != ':'))
            ch++;
        ch++;
        CHECK((*ch != '\0'), "Erreur format de message");
    }

    //DEBUG_S2("stringTOpartie 2 ch <%s> dest <%s>\n", ch, partiech);
    for (i = 0; i < NBMAXLIGNES; ++i)
    {
        sscanf(ch, "%d:%d:", &partie->dist[i], &partie->turn[i]);
        //DEBUG_S2("stringTOpartie 3 ch <%s> dest <%s>\n", ch, partiech);
        while ((*ch != '\0') && (*ch != ':'))
            ch++;
        ch++;
        CHECK((*ch != '\0'), "Erreur format de message");
        while ((*ch != '\0') && (*ch != ':'))
            ch++;
        ch++;
        CHECK((*ch != '\0'), "Erreur format de message");
    }
}

void initPartiTOString(char *dataTxt, time_t temps, partieGraphique_t *partie)
{
    char tmp[MAX_LEN];
    partieTOstring(tmp, partie);
    char topdepart[MAX_LEN];
    timeTostring(topdepart, temps);
    sprintf(dataTxt, "%s:%s:", topdepart, tmp);
}

void StringinitTOParti(time_t *temps, partieGraphique_t *partie, char *dataTxt)
{
    char topdepart[200];
    sscanf(dataTxt, "%[^:]:", topdepart);
    stringToTime(temps, topdepart);
    char *ch = dataTxt;
    while ((*ch != '\0') && (*ch != ':'))
        ch++;
    ch++;
    CHECK((*ch != '\0'), "Erreur format de message");
    stringTOpartie(partie, ch);
}

void scoreTOstr(char *dest, int score)
{
    sprintf(dest, "%d:", score);
};
void strTOscore(char *dest, int *score)
{
    sscanf(dest, "%d:", score);
};