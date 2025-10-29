#include "../../include/effets/EffetOptionnel.hpp"
#include <iostream>

EffetOptionnel::EffetOptionnel(std::unique_ptr<Effet> e)
    : Effet(std::string("Optionnel: ") + e->toString())
    , effet(std::move(e)) {}

void EffetOptionnel::activerEffet(Player& proprietaire, Game& game) {
    std::cout << "Voulez-vous activer l'effet : " << effet->toString() << " ? (1: Oui, 0: Non)\n";
    int choix = 0;
    std::cin >> choix;
    if(choix == 1) {
        effet->activerEffet(proprietaire, game);
    }
}

std::string EffetOptionnel::toString() const {
    if(!effet) return "EffetOptionnel vide";
    std::string base = "Vous pouvez: ";
    base += effet->toString();
    return base;
}