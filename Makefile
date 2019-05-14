CC		= gcc
CFLAGS	= -Wall #-O2 -g
LDFLAGS	= -lSDL -lSDL_image -lGLU -lGL -lm

BINREP	= bin/
SRCREP	= src/
OBJREP	= obj/
ENTREP  = include/

# Fichiers

# Fichiers principaux
OBJ_MAIN = main.o
##
OBJ_POINT = point.o
OBJ_GRAPHE = graphe.o
OBJ_CARTE = carte.o
OBJ_COULEUR = couleur.o
OBJ_VERIF_CARTE =  verif_carte.o
OBJ_LECTURE_FICHIER_CARTE = lecture_fichier_carte.o
##
OBJ_PPM_IMAGE = ./PPM_image/PPM_image.o
OBJ_PPM_ENTETE = ./PPM_image/PPM_entete.o
##
OBJ_TOUR = tour.o
OBJ_MONSTRE = monstre.o
OBJ_VAGUE = vague.o
OBJ_PARCOURS = parcours.o
##
OBJ_JEU = jeu.o
## Affiche carte
OBJ_CARTE= afficheCarte.o

### TOTAL
SRC = $(wildcard $(SRCREP)*.c)
OBJ = $(SRC:.c =.o)

LISTE_OBJ = $(OBJREP)$(OBJ_POINT) $(OBJREP)$(OBJ_COULEUR) $(OBJREP)$(OBJ_GRAPHE) $(OBJREP)$(OBJ_CARTE) $(OBJREP)$(OBJ_VERIF_CARTE) $(OBJREP)$(OBJ_LECTURE_FICHIER_CARTE) $(OBJREP)$(OBJ_PPM_IMAGE) $(OBJREP)$(OBJ_PPM_ENTETE) $(OBJREP)$(OBJ_TOUR) $(OBJREP)$(OBJ_MONSTRE) $(OBJREP)$(OBJ_VAGUE) $(OBJREP)$(OBJ_PARCOURS) $(OBJREP)$(OBJ_CARTE) $(OBJREP)$(OBJ_JEU)

#Fichiers annexes
ENT_COMMUN = commun

##Exécutable
EXEC_ITD = itd



# Regles compilation TD 05

all :

itd : $(OBJREP)$(OBJ_MAIN) $(LISTE_OBJ)
	$(CC) $(CFLAGS) $^ -o $(BINREP)$(EXEC_ITD) $(LDFLAGS)

clean :
	rm -rf *~
	#rm -rf $(SRCREP)*/*~
	rm -rf $(OBJREP)
	rm -rf $(BINREP)*

$(OBJREP)%.o: $(SRCREP)%.c $(ENTREP)%.h $(ENTREP)$(ENT_COMMUN).h
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
