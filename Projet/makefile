#Makfile conçus le 29/6/2020 par benjamin engloo 
#a pour bute de simplifier la gestion des library et la compilation."
# all: mkpile par defaut
# Debug: active les printf spécifique au mod debug
# clean: retire les.o du dossier

#////////recette par defaut//////////
c=gcc #type de compilation
op=-Wall -Wextra #opertion et retoure de la compilation
EXEC=client #nom de l'executable final
EXEC2=serverEnregistrement #nom de l'executable final2
MAINSOURCE=cltSrv.o#fichier source de l'executable
LIBFIC= session.o data.o proto.o#nom des library en .o
HEADERFIC=cltSrv.h#nom des header

#création de l'executable
all:client 
	rm -rf *.o

#activer mode client server défini 
client:CLTSERV= -D'CLIENT'
client:EXEC= client
client:clean comp 

server:CLTSERV= -D'SERVER'
server:EXEC= serverEnregistrement
server:clean comp 


comp: $(MAINSOURCE) $(LIBFIC)
	$(c) $(MAINSOURCE) $(LIBFIC) -o $(EXEC)

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
mrproper: clean
	rm -rf $(EXEC)


