#ifndef ARAIL_JOUEUR_ORGANISE_HPP
#define ARAIL_JOUEUR_ORGANISE_HPP
#include <iostream>
#include <vector>

#define NMB_COULEUR 3

#include "gestion_chemin.hpp"
#include "jeu.hpp"


struct JoueurOrganise {
	int mdp; //mot de passe du joueur
	int id; //numero unique du joueur
	int main_carte_couleur[NMB_COULEUR]; //tableau du nombre de cartes de couleur que possede le joueur pour chaque couleur
	Objectif main_obj; //carte objectif du joueur (joueur organise n'en possede que 1), quand le joueur na pas de objectif on considere que le poids est de -1
	std::vector<Liaison*> chemin_obj; //tableau (de pointeur) des liaisons que le joueur doit avoir pour aller a l'objectif
	std::vector<Carte> cartesPourChemin; //tableau des cartes dont a besoin le joueur
};

//initialisation
void joueur_organise_init(JoueurOrganise& joueur) ;

//suppression
void joueur_organise_suppr(JoueurOrganise& joueur);

//acces au mot de passe a fournir au jeu. Le mot de passe sert a s'assurer que
//le joueur ne tente pas de se faire passer pour un autre. Il sera utilise par
//le joueur pour toutes les actions sur le jeu qui verifient l'identite du
//joueur.
int joueur_organise_mdp(JoueurOrganise& joueur) ;

//prise en compte de l'indice du joueur fourni par le jeu. Le joueur pourra
//utiliser cet indice pour s'identifier aupres du jeu pour les actions qui le
//demandent : piocher, defausser, prendre des liaisons, ...
void joueur_organise_config_indice(JoueurOrganise& joueur, int indice) ;

//un tour de jeu pour le joueur organise
void joueur_organise_tour(Jeu& jeu, JoueurOrganise& joueur);

//retourne la liaison la plus importante parmit celle du plus court chemin vers son objectif
Liaison joueur_organise_liaison_importante(Jeu& jeu, JoueurOrganise& joueur);

#endif
