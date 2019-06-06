# IMAC TOURELLE DÉFENSE
##  ALGO INFOGRAPHIE S2
=======

## Description générale

_

---

Jeu de Tower Defense en C avec OpenGL sur le thème de l'informatique et nommé "Antivirus".
Des virus essaient d'endommager ou pirater un système informatique. Votre but est donc d'empêcher ces virus de faire de mauvaises actions grâce à des composants éléectroniques à rajouter sur le circuit imprimé ! 

Les virus se déplacent le long de chemins sur la carte pour atteindre une sortie. Pour ne pas perdre, vous devez éliminer tous les virus avant qu’ils n’atteignent la sortie. Si des virus atteignent la sortie vous perdez de l'argent nécessaire pour construire des tours de défense (ici des composants électroniques). Il faut donc bien faire attention à son argent pour ne pas être pris au dépourvu lors d’une grande vague d’ennemis.

Attention, la difficulté augmente avec le temps !

## Compiler et lancer Antivirus

make itd
bin/itd [nom de carte dans data]
	exemple : bin/itd infox.itd

## Les commandes de jeu

Q pour quitter
P pour mettre en pause, puis pour revenir au jeu
B + clic gauche pour placer une tour bleue
R + clic gauche pour placer une tour rouge
V + clic gauche pour placer une tour verte
J + clic gauche pour placer une tour jaune
Non implémenté :
S + clic gauche pour supprimer une tour 
