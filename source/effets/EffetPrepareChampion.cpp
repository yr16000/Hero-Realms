#include "../../include/effets/EffetPrepareChampion.hpp"
#include <iostream>

EffetPrepareChampion::EffetPrepareChampion() : Effet("Préparer un champion") {}

void EffetPrepareChampion::activerEffet(Player& proprietaire, Game& game) {
    if(proprietaire.getChampionsEnJeu().empty()) {
        std::cout << "Aucun champion en jeu à préparer.\n";
        return;
    }
    proprietaire.afficherChampionsEnJeu();
    std::cout << "Sélectionnez le numéro du champion à préparer : ";
    int index;
    std::cin >> index;
    while(index < 0 || index >= proprietaire.getChampionsEnJeu().size() || 
        !proprietaire.getChampionsEnJeu()[index-1]->getEstActiver()) {
        std::cout << "Index invalide choisit un autre champion.\n";
        std::cout<<"Choisit :\n";
        std::cin >> index;
    }
    proprietaire.getChampionsEnJeu()[index-1]->setEstActiver(false);
    std::cout << "Champion " << proprietaire.getChampionsEnJeu()[index-1]->getNom() << " est maintenant préparé.\n";
}