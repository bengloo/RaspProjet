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
MAINSOURCE2=serverEnr.o#fichier source de l'executable
LIBFIC= session.o data.o proto.o#nom des library en .o
LIBFIC2= session.o data.o proto.o#nom des library en .o
HEADERFIC=cltSrv.h#nom des header
HEADERFIC2=serverEnr.h#nom des header
#création de l'executable
all : $(MAINSOURCE) $(MAINSOURCE2) $(LIBFIC)
	$(c) $(MAINSOURCE) $(LIBFIC) -o $(EXEC) 
	$(c) $(MAINSOURCE2) $(LIBFIC2) -o $(EXEC2) 
#activer mode debug défini dans ma lib utile
debug:DEBUG=-D'__DEBUG__'
debug:all

#inclusion des headers
$(MAINSOURCE): $(HEADERFIC)

$(MAINSOURCE2): $(HEADERFIC2)
#compilation des library
%.o: %.c
	$(c) -c $< -o $@ $(op) $(DEBUG)

# suppression des fichiers temporaires
clean: 
	rm -rf *.o
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
	rm -rf $(EXEC)


