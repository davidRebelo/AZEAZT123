#include "joueur_simple.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>


#define BLEU 0
#define VERT 1
#define ORANGE 2

void joueur_simple_init(JoueurSimple& joueur) {
	joueur.mdp = rand()%(1000-1)+1;
	joueur.id = -1;
	joueur.main_obj.points = -1;
	joueur.main_carte_couleur[BLEU] = 0;
	joueur.main_carte_couleur[VERT] = 0;
	joueur.main_carte_couleur[ORANGE] = 0;
}

void joueur_simple_suppr(JoueurSimple& joueur) {
}

int joueur_simple_mdp(JoueurSimple& joueur) {
	return joueur.mdp;
}

void joueur_simple_config_indice(JoueurSimple& joueur, int indice) {
	joueur.id = indice;
}

void joueur_simple_tour(Jeu& jeu, JoueurSimple& joueur) {
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
    for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end(); it++){
        switch((*it)->couleur){
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
    }

    if(!liaisonPrise){
        for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end(); it++){
            for (int i = 1; i <= 5 && nmbCarteCouleurPiochee < 2; i++) {
                if ((jeu_carte_visible(jeu, i)).couleur == (*it)->couleur) {
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


