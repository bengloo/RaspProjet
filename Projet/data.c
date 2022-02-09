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

/**
 *  \brief Transforme requete en string 
 *  
 *  \param [in] req requete
 *  \param [out] buff buffer de destination
 *  \return void
 */
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

/**
 *  \brief Transforme string en requete
 *  
 *  \param [out] req requete
 *  \param [in] buff buffer
 *  \return void
 */
void strTOreq(req_t *req, buffer_t buff)
{
    sscanf(buff, FMT_REQ, (short int *)&req->idReq, &req->lgreq, req->msgReq);
}

/**
 *  \brief Transforme reponse en string 
 *  
 *  \param [in] rep reponse
 *  \param [out] buff buffer de destination
 *  \return void
 */
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

/**
 *  \brief Transforme string en reponse
 *  
 *  \param [out] rep reponse recuperee
 *  \param [in] buff string initiale
 *  \return void
 */
void strTOrep(rep_t *rep, buffer_t buff)
{
    sscanf(buff, FMT_REP, (int *)&rep->idRep, &rep->lgrep, rep->msgRep);
}

/**
 *  \brief Transforme adresse en string 
 *  
 *  \param [in] adr adresse
 *  \param [out] dest buffer de destination
 *  \return void
 */
void adresseTOstr(adresse_t *adr, char *dest)
{
    sprintf(dest, "%s:%d:%s", adr->ip, adr->port, adr->pseudo);
}

/**
 *  \brief Transforme string en adresse
 *  
 *  \param [out] adr adresse
 *  \param [in] dest string initiale
 *  \return void
 */
void strTOadresse(adresse_t *adr, char *dest)
{
    sscanf(dest, "%[^:]:%d:%[^:]", adr->ip, &adr->port, adr->pseudo);
}

/**
 *  \brief Transforme statutReq en string 
 *  
 *  \param [in] adr statutReq
 *  \param [out] dest buffer de destination
 *  \return void
 */
void statutReqTOstr(statutReq_t *adr, char *dest)
{
    sprintf(dest, "%d", adr->statut);
}

/**
 *  \brief Transforme string en statut requete
 *  
 *  \param [out] adr statut requete
 *  \param [in] dest string initiale
 *  \return void
 */
void strTOstatutReq(statutReq_t *adr, char *dest)
{
    sscanf(dest, "%d", &adr->statut);
}

/**
 *  \brief Transforme liste de partie en string 
 *  
 *  \param [in] listePartie liste de partie
 *  \param [in] nbPartie nombre de partie
 *  \param [out] dest buffer de destination
 *  \return void
 */
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

/**
 *  \brief Transforme partie en string 
 *  
 *  \param [out] dest buffer de destination
 *  \param [in] partie partie
 *  \return void
 */
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

/**
 *  \brief Transforme string en stat partie
 *  
 *  \param [out] partie stat partie
 *  \param [in] dataTxt string initiale
 *  \return void
 */
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

/**
 *  \brief Transforme string en liste partie
 *  
 *  \param [out] listePartie liste partie recupere
 *  \param [in] dest string initiale
 *  \return void
 */
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

/**
 *  \brief Transforme temps en string 
 *  
 *  \param [out] timeDataRep buffer de destination
 *  \param [in] temps temps
 *  \return void
 */
void timeTostring(char *timeDataRep, time_t temps)
{
    sprintf(timeDataRep, "%lu", temps);
}

/**
 *  \brief Transforme string en temps
 *  
 *  \param [out] temps temps recupere
 *  \param [in] timeDataRep string initiale
 *  \return void
 */
void stringToTime(time_t *temps, char *timeDataRep)
{
    *temps = atol(timeDataRep);
}

/**
 *  \brief Transforme partie graphique en string 
 *  
 *  \param [out] dest buffer de destination
 *  \param [in] partie partie graphique
 *  \return void
 */
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

/**
 *  \brief Transforme string en partie graphique
 *  
 *  \param [out] partie partie graphique
 *  \param [in] partiech string initiale
 *  \return void
 */
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

/**
 *  \brief Transforme requete en string 
 *  
 *  \param [out] dataTxt buffer de destination
 *  \param [in] temps temps de debut de partie
 *  \param [in] partie partie graphique
 *  \return void
 */
void initPartiTOString(char *dataTxt, time_t temps, partieGraphique_t *partie)
{
    char tmp[MAX_LEN];
    partieTOstring(tmp, partie);
    char topdepart[MAX_LEN];
    timeTostring(topdepart, temps);
    sprintf(dataTxt, "%s:%s:", topdepart, tmp);
}

/**
 *  \brief Transforme string en partie
 *  
 *  \param [out] temps temps de debut de partie
 *  \param [out] partie partie graphique
 *  \param [in] dataTxt string initiale
 *  \return void
 */
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

/**
 *  \brief Transforme score en string 
 *  
 *  \param [out] dest buffer de destination
 *  \param [in] score score joueur
 *  \return void
 */
void scoreTOstr(char *dest, int score)
{
    sprintf(dest, "%d:", score);
}

/**
 *  \brief Transforme string en score
 *  
 *  \param [in] dest string initiale
 *  \param [out] score score de destination
 *  \return void
 */
void strTOscore(char *dest, int *score)
{
    sscanf(dest, "%d:", score);
}