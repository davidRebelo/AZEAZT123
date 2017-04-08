#include "jeu.hpp"
#include "jeu_io.hpp"
#include "joueur_simple.hpp"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {

  //chargement du fichier
  std::ifstream file("config_sujet.arail") ;

  //initialisation
  Jeu jeu ;
  jeu_init(jeu) ;

  //lecture de la configuration
  jeu_import(jeu, file) ;

  //finalisation
  jeu_debut_partie(jeu) ;

  cout << endl;

  for(auto it = jeu.liaisons.begin(); it != jeu.liaisons.end(); ++it){
    cout << "" << it->ville1 << "-" << it->ville2;
    cout << " " << jeu.noms_villes.at(it->ville1) << "-" << jeu.noms_villes.at(it->ville2) << endl;
  }

/*
  //joueur simple
  JoueurSimple joueur ;
  joueur_simple_init(joueur) ;

  //ajout du joueur à la partie
  int index = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueur)) ;
  joueur_simple_config_indice(joueur, index) ;

  //jeu
  for(int i = 0; i < 30; ++i) {
    joueur_simple_tour(jeu, joueur) ;
  }

  //historique
  jeu_affiche_historique(jeu) ;

  //menage
  joueur_simple_suppr(joueur) ;*/
  jeu_suppr(jeu) ;

  return 0 ;
}

