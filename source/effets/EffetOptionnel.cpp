#include "../../include/effets/EffetOptionnel.hpp"
#include <iostream>

EffetOptionnel::EffetOptionnel(std::unique_ptr<Effet> e)
    : Effet("Optionnel: " + e->getDescription())
    , effet(std::move(e)) {}

void EffetOptionnel::activerEffet(Player& proprietaire, Game& game) {
    std::cout << "Voulez-vous activer l'effet : " << effet->getDescription() << " ? (1: Oui, 0: Non)\n";
    int choix = 0;
    std::cin >> choix;
    if(choix == 1) {
        effet->activerEffet(proprietaire, game);
    }
}