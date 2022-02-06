#include "data.h"
#include "cltSrv.h"
#include "basic_func.h"


const char *statutPartieTxt[] = {"RUN ", "STOP", "FAIL", "WAIT"};

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

void listePartieTOStr(statPartie_t *listePartie, char *dest)
{
    char *str = dest;
    DEBUG_S1("listePartieTOStr : nbPartie <%d>\n", nbPartie);
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
    DEBUG_S1("listePartieTOStr : str <%s>\n", dest);
}

void StrTOlistePartie(statPartie_t *listePartie, char *dest)
{
    char *str = dest;
    nbPartie = 0;
    while (strlen(str) != 0)
    {
        statPartie_t *partie = &listePartie[nbPartie];
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
        nbPartie++;
    }
}

void reqTOstr(req_t *req, buffer_t buff)
{
    sprintf(buff, "%3d:%d:", req->idReq, req->lgreq);
	char *ch=buff;
	while ((*ch != '\0') && (*ch != ':')) ch++;
	ch++;
	CHECK((*ch != '\0'), "Erreur format de message");
	while ((*ch != '\0') && (*ch != ':')) ch++;
	ch++;
	CHECK((*ch != '\0'), "Erreur format de message");
	strncpy(ch, req->msgReq, req->lgreq);
	ch[req->lgreq]='\0';
}
void strTOreq(req_t *req, buffer_t buff)
{
    sscanf(buff, FMT_REQ, (short int *)&req->idReq, &req->lgreq, req->msgReq);
    DEBUG_S4("strTOreq str <%s> Id <%d> len <%d> msg <%s>\n", buff, req->idReq, req->lgreq, req->msgReq);
}

void repTOstr(rep_t *rep, buffer_t buff)
{
    sprintf(buff, "%d:%hd:", rep->idRep, rep->lgrep);
	char *ch=buff;
	while ((*ch != '\0') && (*ch != ':')) ch++;
	ch++;
	CHECK((*ch != '\0'), "Erreur format de message");
	while ((*ch != '\0') && (*ch != ':')) ch++;
	ch++;
	CHECK((*ch != '\0'), "Erreur format de message");
	strncpy(ch, rep->msgRep, rep->lgrep);
	ch[rep->lgrep]='\0';
}
void strTOrep(rep_t *rep, buffer_t buff)
{
    sscanf(buff, "%d:%hd:%s", (int *)&rep->idRep, &rep->lgrep, rep->msgRep);
    DEBUG_S4("strTOrep str <%s> Id <%d> len <%d> msg <%s>\n", buff, rep->idRep, rep->lgrep, rep->msgRep);
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
	int i=0;
	char *ch=dest;
	for (i = 0; i < NBMAXOBSTACLES; ++i)
	{
		sprintf(ch, "%d:", partie->obstacles[i]);
		//DEBUG_S3("partieTOstring 1 i <%d> ch <%s> len <%d>\n", i, ch, (int)strlen(dest));
		ch+=strlen(ch);
	}
	for (i = 0; i < NBMAXLIGNES; ++i)
	{
		sprintf(ch, "%d:%d:", partie->dist[i], partie->turn[i]);
		//DEBUG_S3("partieTOstring 2 i <%d> ch <%s> len <%d>\n", i, ch, (int)strlen(dest));
		ch+=strlen(ch);
	}
}

void stringTOpartie(partieGraphique_t *partie, const char *partiech)
{
	DEBUG_S1("stringTOpartie debut partiech <%s>\n", partiech);
	int i=0;
	char *ch=(char *)partiech;
	DEBUG_S1("stringTOpartie debut ch <%s>\n", ch);
	for (i = 0; i < NBMAXOBSTACLES; ++i)
	{
		sscanf(ch, "%d:", &partie->obstacles[i]);
		//DEBUG_S2("stringTOpartie 1 obs <%d> ch <%s>\n", partie->obstacles[i], ch);
		while ((*ch != '\0') && (*ch != ':')) ch++;
		ch++;
		CHECK((*ch != '\0'), "Erreur format de message");
	}

	//DEBUG_S2("stringTOpartie 2 ch <%s> dest <%s>\n", ch, partiech);
	for (i = 0; i < NBMAXLIGNES; ++i)
	{
		sscanf(ch, "%d:%d:", &partie->dist[i], &partie->turn[i]);
		//DEBUG_S2("stringTOpartie 3 ch <%s> dest <%s>\n", ch, partiech);
		while ((*ch != '\0') && (*ch != ':')) ch++;
		ch++;
		CHECK((*ch != '\0'), "Erreur format de message");
		while ((*ch != '\0') && (*ch != ':')) ch++;
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
	DEBUG_S1("initPartiTOString <%s>", dataTxt);
}

void StringinitTOParti(time_t *temps, partieGraphique_t *partie, char *dataTxt)
{
    char topdepart[200];
    sscanf(dataTxt, "%[^:]:", topdepart);
    stringToTime(temps, topdepart);
	char *ch=dataTxt;
	while ((*ch != '\0') && (*ch != ':')) ch++;
	ch++;
	CHECK((*ch != '\0'), "Erreur format de message");
    stringTOpartie(partie, ch);
}


//UNUSED
/*fonction caste des data imbriqué*/

void strutToString(statPartie_t *tableau, char ch[NBMAXOBSTACLES])
{
    char aux[300];

    // ajout de l'id a la chaine
    sprintf(ch, "%d", tableau->id);
    strcat(ch, ";");

    // ajout du statut a la chaine
    sprintf(aux, "%d", tableau->statut);
    strcat(ch, aux);
    strcat(ch, ";");

    // ajout adresse ip maitre
    strcat(ch, tableau->addrMaitre.ip);
    strcat(ch, ";");

    // ajout du port du maitre
    sprintf(aux, "%d", tableau->addrMaitre.port);
    strcat(ch, aux);
    strcat(ch, ";");

    // ajout du pseudo du maitre
    strcat(ch, tableau->addrMaitre.pseudo);
    strcat(ch, ";");

    // ajout adresse ip adverse
    strcat(ch, tableau->addrAdverse.ip);
    strcat(ch, ";");

    // ajout du port de l'adverse
    sprintf(aux, "%d", tableau->addrAdverse.port);
    strcat(ch, aux);
    strcat(ch, ";");

    // ajout du pseudo de l'adverse
    strcat(ch, tableau->addrAdverse.pseudo);
    strcat(ch, ";");

    // ajout du score du maitre
    sprintf(aux, "%d", tableau->scoreMaitre);
    strcat(ch, aux);
    strcat(ch, ";");

    // ajout du score de l'adverse
    sprintf(aux, "%d", tableau->scoreAdverse);
    strcat(ch, aux);
    strcat(ch, ";");
}

void stringToStruct(statPartie_t *tableau2, char ch[NBMAXOBSTACLES])
{

    char aux[100];
    int pos = 0;

    // récupération de l'id
    int i;
    for (i = 0; ch[i] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    char *str;
    int value;
    value = strtol(aux, &str, 10);
    tableau2->id = value;

    // le statut
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    value = strtol(aux, &str, 10);
    tableau2->statut = value;

    // l'ip maitre
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    strcpy(tableau2->addrMaitre.ip, aux);

    // le port du maitre
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    value = strtol(aux, &str, 10);
    tableau2->addrMaitre.port = value;

    // le pseudo du maitre
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    strcpy(tableau2->addrMaitre.pseudo, aux);

    // l'ip adverse
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    strcpy(tableau2->addrAdverse.ip, aux);

    // le port de l'adverse
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    value = strtol(aux, &str, 10);
    tableau2->addrAdverse.port = value;

    // le pseudo de l'adverse
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    strcpy(tableau2->addrAdverse.pseudo, aux);

    // score maitre
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    value = strtol(aux, &str, 10);
    tableau2->scoreMaitre = value;

    // score adverse
    for (i = 0; ch[pos] != ';'; i++, pos++)
    {
        aux[i] = ch[pos];
    }
    aux[i] = '\0';
    pos++;

    value = strtol(aux, &str, 10);
    tableau2->scoreAdverse = value;
}
