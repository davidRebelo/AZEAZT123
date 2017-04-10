#include "joueur_opportuniste.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#define BLEU 0
#define VERT 1
#define ORANGE 2

void joueur_opportuniste_init(JoueurOpportuniste& joueur) {
	joueur.mdp = rand()%(1000-1)+1;
	joueur.id = -1;
	joueur.main_obj.points = -1;
	joueur.table_couleur[BLEU] = 0;
	joueur.table_couleur[VERT] = 0;
	joueur.table_couleur[ORANGE] = 0;
	joueur.score = 0;
}

void joueur_opportuniste_suppr(JoueurOpportuniste& joueur) {
}

int joueur_opportuniste_mdp(JoueurOpportuniste& joueur) {
	return joueur.mdp;
}

void joueur_opportuniste_config_indice(JoueurOpportuniste& joueur, int indice) {
	joueur.id = indice;
}

Liaison joueur_opportuniste_liaison_importante(Jeu& jeu, JoueurOpportuniste& joueur){
    int coutCheminOriginal = cout_chemin(joueur.chemin_obj);
    int coutChemin = 0, ecart = 0, max_longueur = 0, i = 0;
    Liaison lImportante;
    lImportante.longueur = -1;
    bool auMoinUneLiaison = false;
    std::vector<int> ecartToutesLiaisons;
    std::vector<Liaison> idToutesLiaisons;

    //s'il n'existe pas de chemin on reenvoie une liaison vide
    if(coutCheminOriginal == -1) return lImportante;

    for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end(); it++){

        if((*it)->proprietaire == 0){
            std::vector<Liaison*> chemin_obj_2 = chemin_le_plus_court_opportuniste(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, joueur.table_couleur, *(*it));

            if(!chemin_obj_2.empty()){
                coutChemin = cout_chemin(chemin_obj_2);
                ecart = coutCheminOriginal - coutChemin;
                if(ecart < 0) ecart = ecart * (-1);

                ecartToutesLiaisons.push_back(ecart);
                idToutesLiaisons.push_back(*(*it));
                auMoinUneLiaison = true;
            }
        }
    }

    for(auto it = ecartToutesLiaisons.begin(); it != ecartToutesLiaisons.end(); it++){
        if(*it > max_longueur){
            max_longueur = *it;
            lImportante = idToutesLiaisons[i];
        }
        i++;
    }

    //si aucune des liaisons a de priorité on la reenvoi vide
    if(!auMoinUneLiaison) lImportante.longueur = -1;

    return lImportante;
}

void joueur_opportuniste_tour(Jeu& jeu, JoueurOpportuniste& joueur, int &nbrObjectifARemplir) {
    int nmbCarteObjPris = 0;
    bool liaisonPrise = false;
    Liaison l_fictif;
    l_fictif.longueur = 0;
    l_fictif.ville1 = -1;
    l_fictif.ville2 = -1;

    //verification si le joueur na pas de carte objectif, dans ce cas il en pioche une et calcule le plus court chemin
    if (joueur.main_obj.points == -1){
        joueur.main_obj = jeu_pioche_objectif(jeu, joueur.id, joueur.mdp);
        nmbCarteObjPris++;
        joueur.chemin_obj = chemin_le_plus_court_opportuniste(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, joueur.table_couleur, l_fictif);
    }

    //verification si la carte objectif du joueur est toujours possible a faire
    if(joueur.chemin_obj.empty() || !chemin_est_disponible(joueur.chemin_obj, joueur.id)){
        do {
            joueur.chemin_obj = chemin_le_plus_court_opportuniste(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, joueur.table_couleur, l_fictif);
            if(joueur.chemin_obj.empty()){
                jeu_defausse_objectif(jeu, joueur.main_obj, joueur.id, joueur.mdp);
                nbrObjectifARemplir--;

                //s'il n'y a plus d'objectif la partie est terminee!
                if(nbrObjectifARemplir <= 0) return;

                joueur.main_obj = jeu_pioche_objectif(jeu, joueur.id, joueur.mdp);
                nmbCarteObjPris++;
                joueur.chemin_obj = chemin_le_plus_court_opportuniste(jeu, joueur.main_obj.ville1, joueur.main_obj.ville2, joueur.id, joueur.table_couleur, l_fictif);
            }
        } while(joueur.chemin_obj.empty() && nmbCarteObjPris < 3);
    }

    //selection de la liaison a prendre en priorité
    Liaison lImportante = joueur_opportuniste_liaison_importante(jeu, joueur);

    //verification si le joueur peut prendre une liaison
    liaisonPrise = joueur_opportuniste_prendre_liaison(jeu, joueur, lImportante);

    //si aucune liaison a ete pris alors le joueur pioche des cartes
    if(!liaisonPrise) joueur_opportuniste_pioche(jeu, joueur, lImportante, liaisonPrise);



    //si le joueur a pris toute les liaisons necessaire pour son objectif alors il gagne des points et defausse sa carte objectif
    if(joueur_possede_chemin(joueur.chemin_obj, joueur.id)){
        joueur.score += joueur.main_obj.points;
        jeu_defausse_objectif(jeu, joueur.main_obj, joueur.id, joueur.mdp);
        nbrObjectifARemplir--;
        joueur.main_obj.points = -1;
    }
}

void joueur_opportuniste_pioche(Jeu& jeu, JoueurOpportuniste& joueur, Liaison lImportante, bool liaisonPrise){
    int nmbCarteCouleurPiochee = 0;

    if(lImportante.longueur == -1){
        //il commence par choisir parmis la pioche des cartes visible et verifie si la couleur d'une des cartes correspond a celles d'une des liaisons dont il a besoin
        for(auto it = joueur.chemin_obj.begin(); it != joueur.chemin_obj.end() && nmbCarteCouleurPiochee < 2; it++){
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
    else{
        //il commence par choisir parmis la pioche des cartes visible et verifie si la couleur d'une des cartes correspond a celle de la liaison dont il a besoin
        for (int i = 1; i <= 5 && nmbCarteCouleurPiochee < 2; i++) {
            //std::cout << "lImportante couleur " << lImportante.couleur << " ;lImportante ville1 " << lImportante.ville1 << " ;lImportante ville2 " << lImportante.ville2 << std::endl;
            if ((jeu_carte_visible(jeu, i)).couleur == lImportante.couleur) {
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

bool joueur_opportuniste_prendre_liaison(Jeu& jeu, JoueurOpportuniste& joueur, Liaison lImportante){

    //si le joueur na aucune liaison a prendre en priorité il choisit ses carte comme un joueur simple
    if(lImportante.longueur == -1){
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
    }
    else{
        // Verification si le joueur peut s'approprier une liaison avec les cartes qu'il possède
        switch(lImportante.couleur){
            case BLEU:
                if(joueur.table_couleur[BLEU] >= lImportante.longueur && lImportante.proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, lImportante.ville1, lImportante.ville2, joueur.id, joueur.mdp);
                    joueur.table_couleur[BLEU] -= lImportante.longueur;

                    //defausse des cartes du joueur dans la pioche
                    int nbrCarteADefausser = lImportante.longueur;
                    for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                        if(joueur.cartesCouleurEnMain[i].couleur == BLEU){
                            jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                            joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                            i--;
                            nbrCarteADefausser--;
                        }
                    }
                    return true;
                }

                break;
            case VERT:
                if(joueur.table_couleur[VERT] >= lImportante.longueur && lImportante.proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, lImportante.ville1, lImportante.ville2, joueur.id, joueur.mdp);
                    joueur.table_couleur[VERT] -= lImportante.longueur;

                    //defausse des cartes du joueur dans la pioche
                    int nbrCarteADefausser = lImportante.longueur;
                    for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                        if(joueur.cartesCouleurEnMain[i].couleur == VERT){
                            jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                            joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                            i--;
                            nbrCarteADefausser--;
                        }
                    }
                    return true;
                }
                break;
            case ORANGE:
                if(joueur.table_couleur[ORANGE] >= lImportante.longueur && lImportante.proprietaire != joueur.id){
                    jeu_prendre_liaison(jeu, lImportante.ville1, lImportante.ville2, joueur.id, joueur.mdp);
                    joueur.table_couleur[ORANGE] -= lImportante.longueur;

                    //defausse des cartes du joueur dans la pioche
                    int nbrCarteADefausser = lImportante.longueur;
                    for(unsigned int i=0; i<joueur.cartesCouleurEnMain.size() && nbrCarteADefausser != 0 ; i++){
                        if(joueur.cartesCouleurEnMain[i].couleur == ORANGE){
                            jeu_defausse(jeu, joueur.cartesCouleurEnMain[i], joueur.id, joueur.mdp);
                            joueur.cartesCouleurEnMain.erase(joueur.cartesCouleurEnMain.begin()+i);
                            i--;
                            nbrCarteADefausser--;
                        }
                    }
                    return true;
                }
                break;
        }
    }

    return false;
}
