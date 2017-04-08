#include "joueur_organise.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>


#define BLEU 0
#define VERT 1
#define ORANGE 2

void joueur_organise_init(JoueurOrganise& joueur) {
	joueur.mdp = rand()%(1000-1)+1;
	joueur.id = -1;
	joueur.main_obj.points = -1;
	joueur.main_carte_couleur[BLEU] = 0;
	joueur.main_carte_couleur[VERT] = 0;
	joueur.main_carte_couleur[ORANGE] = 0;
}

void joueur_organise_suppr(JoueurOrganise& joueur) {
}

int joueur_organise_mdp(JoueurOrganise& joueur) {
	return joueur.mdp;
}

void joueur_organise_config_indice(JoueurOrganise& joueur, int indice) {
	joueur.id = indice;
}

/*
struct JoueurOrganise {
	int mdp; //mot de passe du joueur
	int id; //numero unique du joueur
	int main_carte_couleur[NMB_COULEUR]; //tableau du nombre de cartes de couleur que possede le joueur pour chaque couleur
	Objectif main_obj; //carte objectif du joueur (joueur organise n'en possede que 1), quand le joueur na pas de objectif on considere que le poids est de -1
	std::vector<Liaison*> chemin_obj; //tableau (de pointeur) des liaisons que le joueur doit avoir pour aller a l'objectif
	std::vector<Carte> cartesPourChemin; //tableau des cartes dont a besoin le joueur
};*/

Liaison joueur_organise_liaison_importante(Jeu& jeu, JoueurOrganise& joueur){
    int coutCheminOriginal = cout_chemin(joueur.chemin_obj);
    int coutChemin = 0, ecart = 0, max_longueur = 0, i = 0;
    Liaison lImportante;
    std::vector<int> ecartToutesLiaisons;
    std::vector<Liaison> idToutesLiaisons;

    for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end(); it++){
        std::vector<Liaison*> chemin_obj_2 = chemin_le_plus_court(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, *it);
        coutChemin = cout_chemin(chemin_obj_2);
        ecart = coutCheminOriginal - coutChemin;
        ecartToutesLiaisons.push_back(ecart);
        idToutesLiaisons.push_back(*it);
    }

    for(auto it = ecartToutesLiaisons.begin(); it != ecartToutesLiaisons.end(); it++){
        if(*it > max_longueur){
            max_longueur = *it;
            lImportante = idToutesLiaisons[i];
        }
        i++;
    }

    return lImportante;
}

int cout_chemin(std::vector<Liaison*> chemin_obj){
    int cout_chemin_original= 0;
    for(auto it = chemin_obj.begin(); it != chemin_obj.end(); it++){
        cout_chemin_original += (*it)->longueur;
    }
    return cout_chemin_original;
}

void joueur_organise_tour(Jeu& jeu, JoueurOrganise& joueur) {
    int nmbCarteObjPris = 0, nmbCarteCouleurPiochee = 0;
    bool liaisonPrise = false;
    Liaison l_fictif;
    l_fictif.longueur = 0;

    //verification si le joueur na pas de carte objectif, dans se cas il en pioche une et calcule le plus court chemin
    if (joueur.main_obj.points == -1){
        joueur.main_obj = jeu_pioche_objectif(jeu, joueur.id, joueur.mdp);
        nmbCarteObjPris++;
        joueur.chemin_obj = chemin_le_plus_court(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, l_fictif);
    }

    //verification si la carte objectif du joueur est toujours possible a faire
    if(joueur.chemin_obj.empty() || !chemin_est_disponible(joueur.chemin_obj, joueur.id)){
        do {
            joueur.chemin_obj = chemin_le_plus_court(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, l_fictif);
            if(joueur.chemin_obj.empty()){
                joueur.main_obj = jeu_pioche_objectif(jeu, joueur.id, joueur.mdp);
                nmbCarteObjPris++;
            }
        } while(joueur.chemin_obj.empty() && nmbCarteObjPris < 3);
    }

    // Verification si le joueur peut s'approprier une liaison avec les cartes qu'il possède
    Liaison lImportante = joueur_organise_liaison_importante(jeu, joueur);
        switch(lImportante->couleur){
            case BLEU:
                if(joueur.main_carte_couleur[BLEU] >= (*it)->longueur && !liaisonPrise && (*it)->proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                    liaisonPrise = true;
                }

                break;
            case VERT:
                if(joueur.main_carte_couleur[VERT] >= (*it)->longueur && !liaisonPrise && (*it)->proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                    liaisonPrise = true;
                }
                break;
            case ORANGE:
                if(joueur.main_carte_couleur[ORANGE] >= (*it)->longueur && !liaisonPrise && (*it)->proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                    liaisonPrise = true;
                }
                break;
        }

    if(!liaisonPrise){
        for (int i = 1; i <= 5 && nmbCarteCouleurPiochee < 2; i++) {
            if ((jeu_carte_visible(jeu, i)).couleur == lImportante->couleur) {
                switch((jeu_pioche_visible(jeu, i, joueur.id, joueur.mdp)).couleur){
                    case BLEU:
                        joueur.main_carte_couleur[BLEU]++;
                        nmbCarteCouleurPiochee++;
                        break;
                    case VERT:
                        joueur.main_carte_couleur[VERT]++;
                        nmbCarteCouleurPiochee++;
                        break;
                    case ORANGE:
                        joueur.main_carte_couleur[ORANGE]++;
                        nmbCarteCouleurPiochee++;
                        break;
                }

            }
        }

        while(nmbCarteCouleurPiochee < 2){
            switch((jeu_pioche_cache(jeu, joueur.id, joueur.mdp)).couleur){
                case BLEU:
                    joueur.main_carte_couleur[BLEU]++;
                    nmbCarteCouleurPiochee++;
                    break;
                case VERT:
                    joueur.main_carte_couleur[VERT]++;
                    nmbCarteCouleurPiochee++;
                    break;
                case ORANGE:
                    joueur.main_carte_couleur[ORANGE]++;
                    nmbCarteCouleurPiochee++;
                    break;
            }
        }
    }

}


