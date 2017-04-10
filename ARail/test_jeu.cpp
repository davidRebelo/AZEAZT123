#include "jeu.hpp"
#include "jeu_io.hpp"
#include "joueur_simple.hpp"
#include "joueur_organise.hpp"
#include "joueur_opportuniste.hpp"

#include <time.h>
#include <sstream>
#include <iostream>
#include <fstream>

#define JOUEUR_A_TEST 5

using namespace std;

void test_simple(char** argv);
void test_simple_2_joueurs(char** argv);
void test_organise(char** argv);
void test_organise_2_joueurs(char** argv);
void test_opportuniste_2_joueurs(char** argv);
void test_opportuniste(char** argv);

void afficher_villes(Jeu &jeu){
    int i = 0;
    for(auto it = jeu.noms_villes.begin(); it != jeu.noms_villes.end(); it++){
        std::cout << "" << i << " " << *it << std::endl;
        i++;
    }
}

void afficher_liaison(Jeu &jeu){
    for(auto it = jeu.liaisons.begin(); it != jeu.liaisons.end(); it++){
        std::cout << "" << it->ville1 << "->" << it->ville2 << std::endl;
    }
}

int main(int argc, char** argv) {

    srand (time(NULL));
    //verification qu'un fichier et un type de joueur est fourni

    if(argc != 2) {
        cout << "usage : " << argv[0] << " <config file> " << "<type de joueur>" << endl;
        return 1;
    }

    switch(JOUEUR_A_TEST){
        case 1:
            test_simple(argv);
            break;
        case 2:
            test_organise(argv);
            break;
        case 3:
            test_opportuniste(argv);
            break;
        case 4:
            test_simple_2_joueurs(argv);
            break;
        case 5:
            test_organise_2_joueurs(argv);
            break;
        case 6:
            test_opportuniste_2_joueurs(argv);
            break;
        default:
            cout << "simple= 1 ; organise= 2 ; opportuniste= 3" << endl;
            cout << "simple 2 joueurs= 4 ; organise 2 joueurs= 5 ; opportuniste 2 joueurs= 6" << endl;
            break;
    }

    return 0;
}

void test_simple(char** argv) {

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur simple
    JoueurSimple joueur;
    joueur_simple_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueur));
    joueur_simple_config_indice(joueur, index);

    //jeu
    for(int i = 0; i < 60 && joueur.score < 40; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);
        joueur_simple_tour(jeu, joueur);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << "score joueur: " << joueur.score << endl;

    //menage
    joueur_simple_suppr(joueur);

    jeu_suppr(jeu);
}

void test_simple_2_joueurs(char** argv) {

    int nbrObjectifARemplir;

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur simple
    JoueurSimple joueur;
    joueur_simple_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueur));
    joueur_simple_config_indice(joueur, index);

    //joueur simple
    JoueurSimple joueur2;
    joueur_simple_init(joueur2);

    //ajout du joueur à la partie
    int index2 = jeu_ajout_joueur(jeu, joueur_simple_mdp(joueur2));
    joueur_simple_config_indice(joueur2, index2);

    nbrObjectifARemplir = jeu_nb_objectifs(jeu);

    //jeu
    for(int i = 0; i < 60 && nbrObjectifARemplir > 0; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);
        jeu_nouveau_tour_joueur(jeu, joueur.id, i+1);
        joueur_simple_tour(jeu, joueur);

        jeu_nouveau_tour_joueur(jeu, joueur2.id, i+1);
        joueur_simple_tour(jeu, joueur2);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << "score joueur: " << joueur.score << endl;
    cout << "score joueur2: " << joueur2.score << endl;

    //menage
    joueur_simple_suppr(joueur);
    joueur_simple_suppr(joueur2);

    jeu_suppr(jeu);
}

void test_organise(char** argv) {

    int nbrObjectifARemplir;

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur organise
    JoueurOrganise joueur;
    joueur_organise_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_organise_mdp(joueur));
    joueur_organise_config_indice(joueur, index);

    nbrObjectifARemplir = jeu_nb_objectifs(jeu);

    //jeu
    for(int i = 0; i < 60 && nbrObjectifARemplir > 0; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);

        jeu_nouveau_tour_joueur(jeu, joueur.id, i+1);
        joueur_organise_tour(jeu, joueur, nbrObjectifARemplir);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << endl << "score joueur: " << joueur.score << endl;

    //menage
    joueur_organise_suppr(joueur);

    jeu_suppr(jeu);
}

void test_organise_2_joueurs(char** argv) {

    int nbrObjectifARemplir;

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur organise
    JoueurOrganise joueur;
    joueur_organise_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_organise_mdp(joueur));
    joueur_organise_config_indice(joueur, index);

    JoueurOrganise joueur2;
    joueur_organise_init(joueur2);

    //ajout du joueur à la partie
    int index2 = jeu_ajout_joueur(jeu, joueur_organise_mdp(joueur2));
    joueur_organise_config_indice(joueur2, index2);

    nbrObjectifARemplir = jeu_nb_objectifs(jeu);

    //jeu
    for(int i = 0; i < 60 && nbrObjectifARemplir > 0; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);

        jeu_nouveau_tour_joueur(jeu, joueur.id, i+1);
        joueur_organise_tour(jeu, joueur, nbrObjectifARemplir);

        jeu_nouveau_tour_joueur(jeu, joueur2.id, i+1);
        joueur_organise_tour(jeu, joueur2, nbrObjectifARemplir);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << endl << "score joueur: " << joueur.score << endl;
    cout << "score joueur2: " << joueur2.score << endl;

    //menage
    joueur_organise_suppr(joueur);
    joueur_organise_suppr(joueur2);

    jeu_suppr(jeu);
}

void test_opportuniste(char** argv) {

    int nbrObjectifARemplir;

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur opportuniste
    JoueurOpportuniste joueur;
    joueur_opportuniste_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_opportuniste_mdp(joueur));
    joueur_opportuniste_config_indice(joueur, index);

    nbrObjectifARemplir = jeu_nb_objectifs(jeu);

    //jeu
    for(int i = 0; i < 60 && nbrObjectifARemplir > 0; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);

        jeu_nouveau_tour_joueur(jeu, joueur.id, i+1);
        joueur_opportuniste_tour(jeu, joueur, nbrObjectifARemplir);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << endl << "score joueur: " << joueur.score << endl;

    //menage
    joueur_opportuniste_suppr(joueur);

    jeu_suppr(jeu);
}

void test_opportuniste_2_joueurs(char** argv) {

    int nbrObjectifARemplir;

    //chargement du fichier
    std::ifstream file("config_sujet.arail");

    //initialisation
    Jeu jeu;
    jeu_init(jeu);

    //lecture de la configuration
    jeu_import(jeu, file);

    //finalisation
    jeu_debut_partie(jeu);

    //joueur opportuniste
    JoueurOpportuniste joueur;
    joueur_opportuniste_init(joueur);

    //ajout du joueur à la partie
    int index = jeu_ajout_joueur(jeu, joueur_opportuniste_mdp(joueur));
    joueur_opportuniste_config_indice(joueur, index);

    JoueurOpportuniste joueur2;
    joueur_opportuniste_init(joueur2);

    //ajout du joueur à la partie
    int index2 = jeu_ajout_joueur(jeu, joueur_opportuniste_mdp(joueur2));
    joueur_opportuniste_config_indice(joueur2, index2);

    nbrObjectifARemplir = jeu_nb_objectifs(jeu);

    //jeu
    for(int i = 0; i < 60 && nbrObjectifARemplir > 0; ++i) {
        jeu_nouveau_tour(jeu, joueur.id, i+1);

        jeu_nouveau_tour_joueur(jeu, joueur.id, i+1);
        joueur_opportuniste_tour(jeu, joueur, nbrObjectifARemplir);

        jeu_nouveau_tour_joueur(jeu, joueur2.id, i+1);
        joueur_opportuniste_tour(jeu, joueur2, nbrObjectifARemplir);
    }

    //historique
    jeu_affiche_historique(jeu);

    cout << endl << "score joueur: " << joueur.score << endl;
    cout << "score joueur2: " << joueur2.score << endl;

    //menage
    joueur_opportuniste_suppr(joueur);
    joueur_opportuniste_suppr(joueur2);

    jeu_suppr(jeu);
}

