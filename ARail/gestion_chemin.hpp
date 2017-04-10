#ifndef GESTIONCHEMIN
#define GESTIONCHEMIN

#include "jeu.hpp"

#include <vector>

struct TablePoids {
    int ville;
    int poids;
    int dejaParcouru;
};

struct TableAntecedent {
    int ville;
    int villeAntecedent;
};

//permet de ordonner une priority_queue par ordre du poids dans une variable de type TablePoids
struct Comparateur{
    bool operator()(const TablePoids& a, const TablePoids& b){
        return a.poids > b.poids;
    }
};

//initialisation des tableau TablePoids et TableAntecedent
void init_table(Jeu& jeu, TablePoids tp2[], TableAntecedent ta2[], const int villeDepart);

//renvoi un tableau de pointeur des Liaisons qu'il faut pour faire le plus court chemin entre villeDepart et villeDestination
//la fonction prend juste les liaisons qui n'ont pas de proprietaire ou sont possede par le joueur (id joueur donner en param)
std::vector<Liaison*> chemin_le_plus_court(Jeu& jeu, int villeDepart, int villeDestination, int idJoueur, Liaison main_carte_couleur);

//renvoi un tableau de pointeur des Liaisons qu'il faut pour faire le plus court chemin entre villeDepart et villeDestination
//la fonction prend juste les liaisons qui n'ont pas de proprietaire ou sont possede par le joueur (id joueur donner en param)
//prend en compte l'inventaire des cartes de couleur du joueur
std::vector<Liaison*> chemin_le_plus_court_opportuniste(Jeu& jeu, int villeDepart, int villeDestination, int idJoueur, int table_couleur[], Liaison liaison_a_enlever);

//verifie si le chemin donner est toujours disponible pour l'id du joueur en parametre
//true si le chemin est disponible, false sinon
bool chemin_est_disponible(std::vector<Liaison*> chemin, int idJoueur);

//verifie si le joueur (donner par sont id) possede toute les liaison dans le chemin donner en parametre
//envoie false si le chemin est vide ou si le joueur ne possede pas toute les liaison
bool joueur_possede_chemin(std::vector<Liaison*> chemin, int idJoueur);

//retourne le cout du plus court chemin donner en parametre
int cout_chemin(std::vector<Liaison*> chemin_obj);

#endif
