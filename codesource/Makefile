#Makfile conçus le 29/6/2020 par benjamin engloo 
#a pour bute de simplifier la gestion des library et la compilation."
# all: mkpile par defaut
# Debug: active les printf spécifique au mod debug
# clean: retire les.o du dossier

#////////recette par defaut//////////

op=#-Wall -Wextra #opertion et retoure de la compilation
EXECCLIENT=client#nom de l'executable final
EXECSERVER=serverEnregistrement#nom de l'executable final2
MAINSOURCE=cltSrv.o#fichier source de l'executable
LIBFICCLIENT= session.o data.o proto.o graphisme.o#nom des library en .o
LIBFICSERV= session.o data.o proto.o#nom des library en .o
LIBSYSTEM= -lpthread 
HEADERFIC= basic_func.h  data.h  graphisme.h  proto.h  reqRep.h  session.h#nom des header

#repertoir compilatoir croisé
TARGET_PC=../target_pc
CFLAGS_PC=-I$(TARGET_PC)/include
LDFLAGS_PC=-L$(TARGET_PC)/lib
c_PC=gcc

TARGET_PI=../target_pi
CFLAGS_PI=-I$(TARGET_PI)/include
LDFLAGS_PI=-L$(TARGET_PI)/lib
c_PI=../../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc



#création de l'executable

all:clean PC PI

debug: op += -DDEBUG
debug: all

PC:client_pc server_pc

#activer mode client server défini 
client_pc:CLTSERV= -D'CLIENT'
client_pc:cleanlib comp1_pc 

server_pc:CLTSERV= -D'SERVER'
server_pc:cleanlib comp2_pc 


comp1_pc: $(MAINSOURCE) $(LIBFICCLIENT)
	rm -f cltSrv.o
	rm -f proto.o
	$(c_PC) -c $(CFLAGS_PC) $(LDFLAGS_PC) cltSrv.c -o cltSrv.o $(op) $(CLTSERV) -lncurses
	$(c_PC) -c $(CFLAGS_PC) $(LDFLAGS_PC) proto.c -o proto.o $(op) $(CLTSERV) $(LIBSYSTEM) -lncurses
	$(c_PC) $(CFLAGS_PC) $(LDFLAGS_PC) $(MAINSOURCE) $(LIBFICCLIENT) -o $(EXECCLIENT)_pc $(LIBSYSTEM) -lX11 -lm -lncurses

comp2_pc: $(MAINSOURCE) $(LIBFICSERV)
	rm -f cltSrv.o
	rm -f proto.o
	$(c_PC) -c $(CFLAGS_PC) $(LDFLAGS_PC) cltSrv.c -o cltSrv.o $(op) $(CLTSERV) -lncurses
	$(c_PC) -c $(CFLAGS_PC) $(LDFLAGS_PC) proto.c -o proto.o $(op) $(CLTSERV) -lncurses
	$(c_PC) $(MAINSOURCE) $(LIBFICSERV) -o $(EXECSERVER)_pc $(LIBSYSTEM) -lncurses

PI:client_pi server_pi

#activer mode client server défini 
client_pi:CLTSERV= -D'CLIENT'
client_pi:cleanlib comp1_pi 

server_pi:CLTSERV= -D'SERVER'
server_pi:cleanlib comp2_pi 


comp1_pi: $(MAINSOURCE) $(LIBFICCLIENT)
	rm -f cltSrv.o
	rm -f proto.o
	$(c_PI) -c $(CFLAGS_PI) $(LDFLAGS_PI) cltSrv.c -o cltSrv.o $(op) $(CLTSERV) -lncurses
	$(c_PI) -c $(CFLAGS_PI) $(LDFLAGS_PI) proto.c -o proto.o $(op) $(CLTSERV) $(LIBSYSTEM) -lncurses
	$(c_PI) $(CFLAGS_PI) $(LDFLAGS_PI) $(MAINSOURCE) $(LIBFICCLIENT) -o $(EXECCLIENT)_pi $(LIBSYSTEM) -lX11 -lm -lncurses

comp2_pi: $(MAINSOURCE) $(LIBFICSERV)
	rm -f cltSrv.o
	rm -f proto.o
	$(c_PI) -c $(CFLAGS_PI) $(LDFLAGS_PI) cltSrv.c -o cltSrv.o $(op) $(CLTSERV) -lncurses
	$(c_PI) -c $(CFLAGS_PI) $(LDFLAGS_PI) proto.c -o proto.o $(op) $(CLTSERV) -lncurses
	$(c_PI) $(CFLAGS_PI) $(LDFLAGS_PI) $(MAINSOURCE) $(LIBFICSERV) -o $(EXECSERVER)_pi $(LIBSYSTEM) -lncurses

#inclusion des headers
$(MAINSOURCE): $(HEADERFIC)

#compilation des library #TODO comment distingué la generation des .o des pc et pi?
%.o: %.c
	$(c_PC) $(CFLAGS_PC) $(LDFLAGS_PC) -c $< -o $@ $(op) $(CLTSERV) -lncurses

# suppression des fichiers temporaires
cleanlib: 
	rm -f *.o
	rm -f *.bak
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
clean: cleanlib 
	rm -f $(EXECCLIENT)_pc
	rm -f $(EXECSERVER)_pc
	rm -f $(EXECCLIENT)_pi
	rm -f $(EXECSERVER)_pi

zip: clean
	zip ProjetLE3MW_ENGLOO_MORCQ_LUX_MARQUILLY.zip *.c *.h makefile README*

