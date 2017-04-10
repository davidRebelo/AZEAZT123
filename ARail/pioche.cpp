#include <iostream>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include "pioche.hpp"

void pioche_init(Pioche& pioche, int taille_elt) {
	pioche.tailleElement = taille_elt;
	pioche.taillePioche = 1;
	pioche.tailleDefausse = 1;
	pioche.nbrElemPioche = 0;
	pioche.nbrElemDefausse = 0;
	pioche.pointeurPioche = (char*) malloc(taille_elt);
	pioche.pointeurDefausse = (char*) malloc(taille_elt);
}

void pioche_defausse(Pioche& pioche, const void* elt) {
    //chaque fois que la taille maximal est atteint on la augmente de 1
	if (pioche.nbrElemDefausse == pioche.tailleDefausse) {
		pioche.tailleDefausse ++;
		pioche.pointeurDefausse = (char*) realloc(pioche.pointeurDefausse, pioche.tailleDefausse * pioche.tailleElement);
	}

	pioche.nbrElemDefausse += 1;
	std::memcpy(pioche.pointeurDefausse + (pioche.tailleElement * (pioche.nbrElemDefausse - 1)), elt, pioche.tailleElement);
}

void pioche_pioche(Pioche& pioche, void* target) {
	//quand il ya plus de carte dans la pioche on fait un melange avec les carte de defausse
	if(pioche.nbrElemPioche == 0) {
		pioche_melange(pioche);
	}

	//la carte piocher est celle qui est a la fin de la pioche
	std::memcpy(target, pioche.pointeurPioche + pioche.tailleElement * (pioche.nbrElemPioche-1), pioche.tailleElement);

	pioche.nbrElemPioche -= 1;
}

int nbr_cartes_dans_pioche(Pioche& pioche){
    return pioche.nbrElemPioche;
}

void pioche_melange(Pioche& pioche) {
    char *temp = (char*) malloc(pioche.tailleElement);
    int nbrElemTotal = pioche.nbrElemDefausse + pioche.nbrElemPioche; //nombre total de carte existant

    //redimensionne la pioche si elle ne peut pas contenir toute les cartes
    if (nbrElemTotal > pioche.taillePioche) {
		pioche.pointeurPioche = (char*) realloc(pioche.pointeurPioche, nbrElemTotal * pioche.tailleElement);
		pioche.taillePioche = nbrElemTotal;
	}

    //copie apres la derniere carte de la pioche toute les cartes de la defausse
	std::memcpy(pioche.pointeurPioche + pioche.tailleElement * pioche.nbrElemPioche, pioche.pointeurDefausse, pioche.tailleElement * pioche.nbrElemDefausse);
    pioche.nbrElemPioche = nbrElemTotal;

    //boucle echange aleatoire des cartes en utilisant les curseurs i, j et temp
	for (int i = 0; i < pioche.nbrElemPioche - 1; i++) {
		int j = rand() % pioche.nbrElemPioche;

		//on fait une sauvegarde de la carte qui est a l'emplacement i dans temp
		std::memcpy(temp, pioche.pointeurPioche + pioche.tailleElement * (pioche.nbrElemPioche - i - 1), pioche.tailleElement);

		//on echange j et i
		//j dans i
		if(pioche.pointeurPioche + pioche.tailleElement * (pioche.nbrElemPioche - i - 1) != pioche.pointeurPioche + pioche.tailleElement * j)
		std::memcpy(pioche.pointeurPioche + pioche.tailleElement * (pioche.nbrElemPioche - i - 1), pioche.pointeurPioche + pioche.tailleElement * j, pioche.tailleElement);
		//temp (ancien i) dans j
		std::memcpy(pioche.pointeurPioche + pioche.tailleElement * j, temp, pioche.tailleElement);
	}

	pioche.nbrElemDefausse = 0;
	free(temp);
}

void pioche_suppr(Pioche& pioche) {
	free(pioche.pointeurPioche);
	free(pioche.pointeurDefausse);
}
