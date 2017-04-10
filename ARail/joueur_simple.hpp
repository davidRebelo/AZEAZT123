#ifndef ARAIL_JOUEUR_SIMPLE_HPP
#define ARAIL_JOUEUR_SIMPLE_HPP
#include <iostream>
#include <vector>

#define NMB_COULEUR 3

#include "gestion_chemin.hpp"
#include "jeu.hpp"


struct JoueurSimple {
	int mdp; //mot de passe du joueur
	int id; //numero unique du joueur
	int table_couleur[NMB_COULEUR]; //tableau du nombre de cartes de couleur que possede le joueur pour chaque couleur
	std::vector<Carte> cartesCouleurEnMain; //les cartes couleur que le joueur possede
	Objectif main_obj; //carte objectif du joueur (joueur simple n'en possede que 1), quand le joueur na pas de objectif on considere que le poids est de -1
	std::vector<Liaison*> chemin_obj; //tableau (de pointeur) des liaisons que le joueur doit avoir pour aller a l'objectif
	std::vector<Carte> cartesPourChemin; //tableau des cartes dont a besoin le joueur
	int score;
};

//initialisation
void joueur_simple_init(JoueurSimple& joueur) ;

//suppression
void joueur_simple_suppr(JoueurSimple& joueur);

//acces au mot de passe a fournir au jeu. Le mot de passe sert a s'assurer que
//le joueur ne tente pas de se faire passer pour un autre. Il sera utilise par
//le joueur pour toutes les actions sur le jeu qui verifient l'identite du
//joueur.
int joueur_simple_mdp(JoueurSimple& joueur) ;

//prise en compte de l'indice du joueur fourni par le jeu. Le joueur pourra
//utiliser cet indice pour s'identifier aupres du jeu pour les actions qui le
//demandent : piocher, defausser, prendre des liaisons, ...
void joueur_simple_config_indice(JoueurSimple& joueur, int indice) ;

//un tour de jeu pour le joueur simple
void joueur_simple_tour(Jeu& jeu, JoueurSimple& joueur);

//le joueur pioche les cartes visible ou cache
void joueur_simple_pioche(Jeu& jeu, JoueurSimple& joueur, bool liaisonPrise);

//verifie si le joueur peut prendre une liaison avec ses cartes et la prend dans le cas positif
//reenvoie true si une liaison a ete prise, false sinon
bool joueur_simple_prendre_liaison(Jeu& jeu, JoueurSimple& joueur);

#endif
