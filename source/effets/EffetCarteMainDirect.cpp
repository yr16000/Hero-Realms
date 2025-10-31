#include "../../include/effets/EffetCarteMainDirect.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>
#include <algorithm>

EffetCarteMainDirect::EffetCarteMainDirect() 
    : Effet("Carte en main directe") {}

void EffetCarteMainDirect::activerEffet(Player& proprietaire, Game& game) {
    std::cout << "La prochaine carte acquise ira directement dans la main\n";
    proprietaire.setNextAcquiredToHand(true);
}

std::string EffetCarteMainDirect::toString() const {
    return "La prochaine carte acquise ira directement dans la main";
}