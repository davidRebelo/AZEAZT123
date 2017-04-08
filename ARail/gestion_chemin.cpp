#include "gestion_chemin.hpp"

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

void init_table(Jeu& jeu, TablePoids tp[], TableAntecedent ta[], const int villeDepart){
    int nbrVilles = jeu_nb_villes(jeu);
    int i;

    //ajout des villes sur les tables et initialisation de leur valeurs
    for(i=0; i<nbrVilles; i++){
        tp[i].ville = i;
        tp[i].dejaParcouru = 0;

        //poids -1 = infini, 0 = ville de depart
        if(i != villeDepart) tp[i].poids = -1;
        else tp[i].poids = 0;

        ta[i].ville = i;
        ta[i].villeAntecedent = -1;
    }


}

std::vector<Liaison*> chemin_le_plus_court(Jeu& jeu, int villeDepart, int villeDestination, int idJoueur, Liaison liaison_a_enlever){
    int nbrVilles = jeu_nb_villes(jeu);
    int selected = villeDepart;
    std::vector<int> cheminLePlusCourtVille;
    std::vector<Liaison*> cheminLePlusCourtLiaison;

    TablePoids * tp = new TablePoids[nbrVilles];
    TableAntecedent * ta = new TableAntecedent[nbrVilles];

    //contient les villes dont le chemin a ete calcule, avec comme premier element le chemin le plus court
    std::priority_queue <TablePoids, std::vector<TablePoids>, Comparateur> file;

    init_table(jeu, tp, ta, villeDepart);

    //boucle jusqu'a etre arriver a la ville demander (et donc avoir finit de calculer le plus court chemin jusqu'a celui-ci)
    do{

        tp[selected].dejaParcouru = 1;

        //parcours des liaison de la ville ou on est
        for(int i=0; i<jeu_nb_villes(jeu); i++){
            Liaison it = jeu_liaison(jeu, tp[selected].ville, i);
            if(it.longueur != 0){
                if(tp[it.ville2].dejaParcouru == 0 && (it.proprietaire == idJoueur || it.proprietaire == 0)){
                    //verification si le chemin donner par les liaisons sont plus courtes que les anciens
                    if((tp[it.ville2].poids > (it.longueur)+(tp[selected].poids)) || tp[it.ville2].poids == -1){
                        tp[it.ville2].poids = (it.longueur)+(tp[selected].poids);
                        ta[it.ville2].villeAntecedent = selected;
                    }
                    file.push(tp[it.ville2]);
                }
            }
        }

        //selection du chemin le plus court (non marquer) pour une nouvelle interaction
        selected = file.top().ville;
        file.pop();

    }while(selected != villeDestination && !file.empty());

    //si aucun chemin est disponible une chemin vide est retourner
    if(tp[selected].poids == -1) return cheminLePlusCourtLiaison;

    //sauvegarde du plus court chemin (villes) dans un vector, le chemin est sauvegarder a l'envers (fin->debut)
    //donc on inverse l'ordre juste apres pour qu'il soit dans le bon ordre (debut->fin)
    selected = villeDestination;
    while(selected != -1){
        cheminLePlusCourtVille.push_back(selected);
        selected = ta[selected].villeAntecedent;
    }
    std::reverse(cheminLePlusCourtVille.begin(), cheminLePlusCourtVille.end());

    /*
    //TEST: affiche le chemin en mode texte
    printf("\nObjectif: %d  Destination: %d\n", villeDepart, villeDestination);

    selected = villeDestination;
    printf("DEBUT-");
    for(auto i = cheminLePlusCourtVille.begin(); i != cheminLePlusCourtVille.end(); i++){
        printf("%d-", *i);
    }
    printf("FIN\n");
    printf("cout: %d\n", tp[selected].poids);
    */

    //sauvegarde du plus court chemin (liaisons) dans un vector
    for(auto it = cheminLePlusCourtVille.begin(); it != cheminLePlusCourtVille.end()-1; it++){
        for(auto it2 = jeu.liaisons.begin(); it2 != jeu.liaisons.end(); it2++){
            if(it2->ville1 == *it && it2->ville2 == *(it+1)){
                cheminLePlusCourtLiaison.push_back(&*it2);
            }
        }
	}

    return cheminLePlusCourtLiaison;
}

bool chemin_est_disponible(std::vector<Liaison*> chemin, int idJoueur){
    for(auto it = chemin.begin(); it != chemin.end(); ++it){
		if((*it)->proprietaire != 0 && (*it)->proprietaire != idJoueur){
            return false;
        }
    }
    return true;
}
