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
std::vector<Liaison*> chemin_le_plus_court(Jeu& jeu, int villeDepart, int villeDestination, int idJoueur, Liaison lFictive);

//verifie si le chemin donner est toujours disponible pour l'id du joueur en parametre
//true si le chemin est disponible, false sinon
bool chemin_est_disponible(std::vector<Liaison*> chemin, int idJoueur);

#endif
