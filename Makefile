CC		= gcc
CFLAGS	= -Wall #-O2 -g
LDFLAGS	= -lm

BINREP	= bin/
SRCREP	= src/
OBJREP	= obj/
ENTREP  = include/

# Fichiers

# Fichiers principaux
OBJ_MAIN = main.o
OBJ_FICHIER_CARTE = gestion_fichier_carte.o

#Fichiers annexes
ENT_COMMUN = commun

##Exécutable
EXEC_ITD = itd




# Regles compilation TD 05

all :

itd : $(OBJREP)$(OBJ_MAIN) $(OBJREP)$(OBJ_FICHIER_CARTE)
	$(CC) $(CFLAGS) $^ -o $(BINREP)$(EXEC_ITD) $(LDFLAGS)

clean :
	rm -rf *~
	#rm -rf $(SRCREP)*/*~
	rm -rf $(OBJREP)
	rm -rf $(BINREP)*

$(OBJREP)%.o: $(SRCREP)%.c $(ENTREP)%.h $(ENTREP)$(ENT_COMMUN).h
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
