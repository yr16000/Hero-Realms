#include "../../include/effets/EffetAssomerChampion.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Champion.hpp"

EffetAssomerChampion::EffetAssomerChampion()
    : Effet("Assomer le champion") {}


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
    bool aGarde = false;
    for(const auto& champPtr : adversaire->getChampionsEnJeu()) {
        auto champ = dynamic_cast<Champion*>(champPtr.get());
        if(champ && champ->getEstGarde()) {
            aGarde = true;
            break;
        }
    }
    if(aGarde) {
        std::cout << "L'adversaire a un champion garde en jeu. Vous ne pouvez pas assommer un autre champion.\n";
        adversaire->afficherChampionsEnJeu();
        std::vector<int> gardesIndices;
        for(size_t i = 0; i < adversaire->getChampionsEnJeu().size(); ++i) {
            auto champ = dynamic_cast<Champion*>(adversaire->getChampionsEnJeu()[i].get());
            if(champ && champ->getEstGarde()) {
                gardesIndices.push_back(i);
            }
        }
        std::cout << "Sélectionnez le numéro du champion garde à assommer : ";
        int index;
        std::cin >> index;
        while(std::find(gardesIndices.begin(), gardesIndices.end(), index - 1) == gardesIndices.end()) {
            std::cout << "Index invalide, choisissez un champion garde.\n";
            std::cout << "Choisissez :\n";
            std::cin >> index;
        }
    adversaire->getDefausse().push_back(std::move(adversaire->getChampionsEnJeu()[index-1]));
    adversaire->getChampionsEnJeu().erase(adversaire->getChampionsEnJeu().begin()+(index-1));
        std::cout<< "Carte assomée.\n";
        return;
    }
    adversaire->afficherChampionsEnJeu();
    std::cout << "Sélectionnez le numéro du champion à assommer : ";
    int index;
    std::cin >> index;
    while(index < 0 || index >= (int)adversaire->getChampionsEnJeu().size()) {
        std::cout << "Index invalide, choisissez un autre champion.\n";
        std::cout << "Choisissez :\n";
        std::cin >> index;
    }
    adversaire->getDefausse().push_back(std::move(adversaire->getChampionsEnJeu()[index-1]));
    adversaire->getChampionsEnJeu().erase(adversaire->getChampionsEnJeu().begin()+(index-1));
    std::cout<< "Carte assomée.\n";
    
}

std::string EffetAssomerChampion::toString() const {
    return std::string("Assommer un champion adverse (ciblez un champion)");
}
