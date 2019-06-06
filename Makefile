CC		= gcc
CFLAGS	= -Wall #-O2 -g
LDFLAGS	= -lSDL -lSDL_image -lGLU -lGL -lm -lglut

## RÉPERTOIRES
BINREP	= bin/
SRCREP	= src/
OBJREP	= obj/
ENTREP  = include/

REP_PPM = PPM_image/
##Exécutable
EXEC_ITD = itd


## LES CHEMINS (VPATH)
vpath %.c $(SRCREP)
vpath %.h $(ENTREP)

# Fichiers

# Fichiers principaux
OBJ_MAIN = main.o
##
OBJ_FICHIER = fichier.o
OBJ_COULEUR = couleur.o
OBJ_POINT = point.o
OBJ_FICHIER = fichier.o
OBJ_GRAPHE = graphe.o
OBJ_CARTE = carte.o

OBJ_VERIF_CARTE =  verif_carte.o
OBJ_LECTURE_CARTE = lecture_fichier_carte.o
#

OBJ_PPM_IMAGE = $(REP_PPM)PPM_image.o
OBJ_PPM_ENTETE = $(REP_PPM)PPM_entete.o
##
OBJ_TOUR = tour.o
OBJ_MONSTRE = monstre.o
OBJ_CITE = cite.o
OBJ_VAGUE = vague.o
OBJ_PARCOURS = parcours.o
##
OBJ_JEU = jeu.o
## Préparation de l'affichage
OBJ_GESTION_AFFICHAGE = gestion_affichage.o

## Affiche elements
OBJ_AFFICHAGE_ELEMENT = affichage_element.o

## Bouton
OBJ_BOUTON = bouton.o

LISTE_OBJ = $(OBJREP)$(OBJ_FICHIER) $(OBJREP)$(OBJ_PPM_IMAGE) $(OBJREP)$(OBJ_PPM_ENTETE) \
$(OBJREP)$(OBJ_POINT) $(OBJREP)$(OBJ_COULEUR) $(OBJREP)$(OBJ_GRAPHE)\
$(OBJREP)$(OBJ_CARTE) $(OBJREP)$(OBJ_VERIF_CARTE) $(OBJREP)$(OBJ_LECTURE_CARTE) \
$(OBJREP)$(OBJ_TOUR) $(OBJREP)$(OBJ_CITE) $(OBJREP)$(OBJ_MONSTRE) $(OBJREP)$(OBJ_VAGUE) $(OBJREP)$(OBJ_PARCOURS) \
$(OBJREP)$(OBJ_GESTION_AFFICHAGE) $(OBJREP)$(OBJ_AFFICHAGE_ELEMENT) $(OBJREP)$(OBJ_BOUTON) \
$(OBJREP)$(OBJ_JEU)


### DÉPENDENCES ###
$(OBJ_MAIN) : main.h
$(OBJ_COULEUR) : couleur.h
$(OBJ_POINT): point.h
$(OBJ_FICHIER) : fichier.h

$(OBJ_PPM_ENTETE) : couleur.h fichier.h
$(OBJ_PPM_IMAGE) : couleur.h point.h fichier.h $(REP_PPM)PPM_entete.h
$(OBJ_PPM_IMAGE) : couleur.h point.h fichier.h $(REP_PPM)PPM_entete.h
$(OBJ_GRAPHE) : point.h
$(OBJ_CARTE) : couleur.h fichier.h graphe.h
$(OBJ_LECTURE_CARTE) : couleur.h point.h fichier.h graphe.h carte.h
$(OBJ_VERIF_CARTE) : couleur.h point.h graphe.h carte.h

$(OBJ_MONSTRE) : couleur.h point.h graphe.h
$(OBJ_TOUR) : point.h carte.c monstre.h
$(OBJ_CITE) : point.h carte.h graphe.h tour.h

$(OBJ_PARCOURS) : point.h graphe.h carte.h monstre.h tour.h
$(OBJ_VAGUE) : graphe.h carte.h monstre.h tour.h cite.h parcours.h

$(OBJ_BOUTON) : point.h couleur.h

$(OBJ_GESTION_AFFICHAGE) : couleur.h point.h monstre.h tour.h bouton.h rang.h
$(OBJ_AFFICHAGE_ELEMENT) : gestion_affichage.h couleur.h point.h monstre.h tour.h cite.h vague.h bouton.h rang.h

$(OBJ_JEU) : carte.h verif_carte.h lecture_fichier_carte.h $(REP_PPM)PPM_image.h affichage_element.h gestion_affichage.h couleur.h point.h monstre.h tour.h cite.h vague.h bouton.h rang.h

all :

itd : $(OBJREP)$(OBJ_MAIN) $(LISTE_OBJ)
	@$(CC) $(CFLAGS) $^ -o $(BINREP)$(EXEC_ITD) $(LDFLAGS)
	@echo "---- ÉDITION DES LIENS TERMINÉE ----"

clean :
	@rm -rf *~
	@rm -rf $(OBJREP)
	@rm -rf $(BINREP)*
	@echo "--- SUPPRESSION DES FICHIERS OBJETS AVEC SUCCÈS ----"

$(OBJREP)%.o: %.c %.h
	@mkdir -p `dirname $@`
	@$(CC) -o $@ -c $< $(CFLAGS)
