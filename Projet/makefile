#Makfile conçus le 29/6/2020 par benjamin engloo 
#a pour bute de simplifier la gestion des library et la compilation."
# all: mkpile par defaut
# Debug: active les printf spécifique au mod debug
# clean: retire les.o du dossier

#////////recette par defaut//////////
c=gcc #type de compilation
op=-Wall -Wextra #opertion et retoure de la compilation
EXECCLIENT=client #nom de l'executable final
EXECSERVER=serverEnregistrement #nom de l'executable final2
MAINSOURCE=cltSrv.o#fichier source de l'executable
LIBFICCLIENT= session.o data.o proto.o graphisme.o#nom des library en .o
LIBFICSERV= session.o data.o proto.o#nom des library en .o
HEADERFIC=cltSrv.h#nom des header

#création de l'executable
all:client 
	rm -rf *.o

#activer mode client server défini 
client:CLTSERV= -D'CLIENT'
client:clean comp1 

server:CLTSERV= -D'SERVER'
server:clean comp2 


comp1: $(MAINSOURCE) $(LIBFICCLIENT)
	$(c) $(MAINSOURCE) $(LIBFICCLIENT) -o $(EXECCLIENT) -lX11 -lm 

comp2: $(MAINSOURCE) $(LIBFICSERV)
	$(c) $(MAINSOURCE) $(LIBFICSERV) -o $(EXECSERVER)

#inclusion des headers
$(MAINSOURCE): $(HEADERFIC)

#compilation des library
%.o: %.c
	$(c) -c $< -o $@ $(op) $(CLTSERV)

# suppression des fichiers temporaires
clean: 
	rm -rf *.o
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
proper: clean
	rm -rf $(EXECCLIENT)
	rm -rf $(EXECSERVER)

zip: proper
	zip ProjetLE3MW_ENGLOO_MORCQ_LUX_MARQUILLY.zip *.c *.h makefile README*

