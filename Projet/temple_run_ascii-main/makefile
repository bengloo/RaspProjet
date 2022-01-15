#Makfile conçus le 29/6/2020 par benjamin engloo 
#a pour bute de simplifier la gestion des library et la compilation."
# all: mkpile par defaut
# Debug: active les printf spécifique au mod debug
# clean: retire les.o du dossier

#////////recette par defaut//////////
c=gcc #type de compilation
op=-Wall -Wextra#opertion et retoure de la compilation
MAINSOURCE=temple_run.o#fichier source de l'executable
LIBFIC= graphisme.o#nom des library en .o
HEADERFIC=#nom des header
#création de l'executable
all : $(MAINSOURCE) $(LIBFIC)
	$(c) $(MAINSOURCE) $(LIBFIC) -lX11 -lm 
#activer mode debug défini dans ma lib utile
debug:DEBUG=-D'__DEBUG__'
debug:all

#inclusion des headers
$(MAINSOURCE): $(HEADERFIC)
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


