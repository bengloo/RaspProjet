#Makfile conçus le 29/6/2020 par benjamin engloo 
#a pour bute de simplifier la gestion des library et la compilation."
# all: mkpile par defaut
# Debug: active les printf spécifique au mod debug
# clean: retire les.o du dossier

#////////recette par defaut//////////
c=gcc #type de compilation
op=#-Wall -Wextra #opertion et retoure de la compilation
EXECCLIENT=client #nom de l'executable final
EXECSERVER=serverEnregistrement #nom de l'executable final2
MAINSOURCE=cltSrv.o#fichier source de l'executable
LIBFICCLIENT= session.o data.o proto.o graphisme.o#nom des library en .o
LIBFICSERV= session.o data.o proto.o#nom des library en .o
LIBSYSTEM= -lpthread 
HEADERFIC=cltSrv.h#nom des header

debug: op += -DDEBUG
debug : all

#création de l'executable
all:clean client server
	


#activer mode client server défini 
client:CLTSERV= -D'CLIENT'
client:cleanlib comp1 

server:CLTSERV= -D'SERVER'
server:cleanlib comp2 


comp1: $(MAINSOURCE) $(LIBFICCLIENT)
	rm -f cltSrv.o
	rm -f proto.o
	$(c) -c cltSrv.c -o cltSrv.o $(op) $(CLTSERV)
	$(c) -c proto.c -o proto.o $(op) $(CLTSERV) $(LIBSYSTEM)
	$(c) $(MAINSOURCE) $(LIBFICCLIENT) -o $(EXECCLIENT) $(LIBSYSTEM) -lX11 -lm 

comp2: $(MAINSOURCE) $(LIBFICSERV)
	rm -f cltSrv.o
	rm -f proto.o
	$(c) -c cltSrv.c -o cltSrv.o $(op) $(CLTSERV)
	$(c) -c proto.c -o proto.o $(op) $(CLTSERV)
	$(c) $(MAINSOURCE) $(LIBFICSERV) -o $(EXECSERVER) $(LIBSYSTEM) 

#inclusion des headers
$(MAINSOURCE): $(HEADERFIC)

#compilation des library
%.o: %.c
	$(c) -c $< -o $@ $(op) $(CLTSERV)

# suppression des fichiers temporaires
cleanlib: 
	rm -f *.o
	rm -f *.bak
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
clean: cleanlib
	rm -f $(EXECCLIENT)
	rm -f $(EXECSERVER)

zip: clean
	zip ProjetLE3MW_ENGLOO_MORCQ_LUX_MARQUILLY.zip *.c *.h makefile README*

