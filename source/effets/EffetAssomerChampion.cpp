#include "../../include/effets/EffetAssomerChampion.hpp"
#include <iostream>

EffetAssomerChampion::EffetAssomerChampion() : Effet("Assomer champion") {}

void EffetAssomerChampion::activerEffet(Player& proprietaire, Game& game){
    Player* adversaire = nullptr;
    for(auto& player: game.getPlayers()){
        if(player.getId() != proprietaire.getId()){
            adversaire = &player;
            break;
        }
    }
    if(adversaire->getChampionsEnJeu().empty()) {
        std::cout << "L'adversaire n'a pas de champions en jeu à assommer.\n";
        return;
    }
    adversaire->afficherChampionsEnJeu();
    std::cout << "Sélectionnez le numéro du champion à assommer : ";
    int index;
    std::cin >> index;
    while(index < 0 || index >= adversaire->getChampionsEnJeu().size()) {
        std::cout << "Index invalide, choisissez un autre champion.\n";
        std::cout << "Choisissez :\n";
        std::cin >> index;
    }
    adversaire->getDefausse().push_back(std::move(adversaire->getChampionsEnJeu()[index-1]));
    adversaire->getChampionsEnJeu().erase(adversaire->getChampionsEnJeu().begin()+(index-1));
    std::cout<< "Carte assomée.\n";
    
}
