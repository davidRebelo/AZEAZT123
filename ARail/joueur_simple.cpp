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
	joueur.score = 0;
	joueur.main_obj.points = -1;
	joueur.table_couleur[BLEU] = 0;
	joueur.table_couleur[VERT] = 0;
	joueur.table_couleur[ORANGE] = 0;
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
    int nmbCarteObjPris = 0;
    bool liaisonPrise = false;
    Liaison l_fictif;
    l_fictif.longueur = 0;
    l_fictif.ville1 = -1;
    l_fictif.ville2 = -1;

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
                jeu_defausse_objectif(jeu, joueur.main_obj, joueur.id, joueur.mdp);
                joueur.main_obj = jeu_pioche_objectif(jeu, joueur.id, joueur.mdp);
                nmbCarteObjPris++;
            }
        }while(joueur.chemin_obj.empty() && nmbCarteObjPris < 3);
    }

    liaisonPrise = joueur_simple_prendre_liaison(jeu, joueur);

    joueur_simple_pioche(jeu, joueur, liaisonPrise);

    //si le joueur a pris toute les liaisons necessaire pour son objectif alors il gagne des points et defausse sa carte objectif
    if(joueur_possede_chemin(joueur.chemin_obj, joueur.id)){
        joueur.score += joueur.main_obj.points;
        jeu_defausse_objectif(jeu, joueur.main_obj, joueur.id, joueur.mdp);
        joueur.main_obj.points = -1;
    }
}

void joueur_simple_pioche(Jeu& jeu, JoueurSimple& joueur, bool liaisonPrise){
    int nmbCarteCouleurPiochee = 0;
    //si le joueur n'a pas pris de liaison alors il peux piocher des cartes
    if(!liaisonPrise){
        //il commence par choisir parmis la pioche des cartes visible et verifie si la couleur d'une des cartes correspond a celles d'une des liaisons dont il a besoin
        for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end() && nmbCarteCouleurPiochee < 2; it++){
            //verification si la liaison a deja ete acheter
            if((*it)->proprietaire != joueur.id){
                for (int i = 1; i <= 5 && nmbCarteCouleurPiochee < 2; i++) {
                    if ((jeu_carte_visible(jeu, i)).couleur == (*it)->couleur) {
                        joueur.cartesCouleurEnMain.push_back(jeu_pioche_visible(jeu, i, joueur.id, joueur.mdp));
                        int couleur_piocher = joueur.cartesCouleurEnMain.back().couleur;
                        switch(couleur_piocher){
                            case BLEU:
                                joueur.table_couleur[BLEU]++;
                                nmbCarteCouleurPiochee++;
                                break;
                            case VERT:
                                joueur.table_couleur[VERT]++;
                                nmbCarteCouleurPiochee++;
                                break;
                            case ORANGE:
                                joueur.table_couleur[ORANGE]++;
                                nmbCarteCouleurPiochee++;
                                break;
                            default: //apparait juste en cas de bug avec la pioche de carte
                                std::cout << "carte visible pas normal (couleur existe pas)" << couleur_piocher << std::endl;
                                nmbCarteCouleurPiochee++;
                                break;
                        }

                    }
                }
            }
        }
        //si le joueur n'a toujours pas piocher 2 cartes alors il va en piocher dans la pioche cache
        while(nmbCarteCouleurPiochee < 2){
            joueur.cartesCouleurEnMain.push_back(jeu_pioche_cache(jeu, joueur.id, joueur.mdp));
            int couleur_piocher = joueur.cartesCouleurEnMain.back().couleur;
            switch(couleur_piocher){
                case BLEU:
                    joueur.table_couleur[BLEU]++;
                    nmbCarteCouleurPiochee++;
                    break;
                case VERT:
                    joueur.table_couleur[VERT]++;
                    nmbCarteCouleurPiochee++;
                    break;
                case ORANGE:
                    joueur.table_couleur[ORANGE]++;
                    nmbCarteCouleurPiochee++;
                    break;
                default: //apparait juste en cas de bug avec la pioche de carte
                    std::cout << "carte cache pas normal (couleur existe pas)" << couleur_piocher << std::endl;
                    nmbCarteCouleurPiochee++;
                    break;
            }
        }
    }
}

bool joueur_simple_prendre_liaison(Jeu& jeu, JoueurSimple& joueur){
    // Verification si le joueur peut s'approprier une liaison avec les cartes qu'il possède
    for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end(); it++){
        //verification si la liaison a deja ete acheter
        if((*it)->proprietaire != joueur.id){
            switch((*it)->couleur){
                case BLEU:
                    if(joueur.table_couleur[BLEU] >= (*it)->longueur && (*it)->proprietaire != joueur.id){
                        jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                        joueur.table_couleur[BLEU] -= (*it)->longueur;

                        //defausse des cartes du joueur dans la pioche
                        int nbrCarteADefausser = (*it)->longueur;
                        for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                            if(joueur.cartesCouleurEnMain[i].couleur == BLEU){
                                jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                                joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                                nbrCarteADefausser--;
                                i--;
                            }
                        }

                        return true;
                    }

                    break;
                case VERT:
                    if(joueur.table_couleur[VERT] >= (*it)->longueur && (*it)->proprietaire != joueur.id){
                        jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                        joueur.table_couleur[VERT] -= (*it)->longueur;

                        //defausse des cartes du joueur dans la pioche
                        int nbrCarteADefausser = (*it)->longueur;
                        for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                            if(joueur.cartesCouleurEnMain[i].couleur == VERT){
                                jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                                joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                                nbrCarteADefausser--;
                                i--;
                            }
                        }

                        return true;
                    }
                    break;
                case ORANGE:
                    if(joueur.table_couleur[ORANGE] >= (*it)->longueur && (*it)->proprietaire != joueur.id){
                        jeu_prendre_liaison(jeu, (*it)->ville1, (*it)->ville2, joueur.id, joueur.mdp);
                        joueur.table_couleur[ORANGE] -= (*it)->longueur;

                        //defausse des cartes du joueur dans la pioche
                        int nbrCarteADefausser = (*it)->longueur;
                        for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                            if(joueur.cartesCouleurEnMain[i].couleur == ORANGE){
                                jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                                joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                                nbrCarteADefausser--;
                                i--;
                            }
                        }

                        return true;
                    }
                    break;
            }
        }
    }

    return false;
}


