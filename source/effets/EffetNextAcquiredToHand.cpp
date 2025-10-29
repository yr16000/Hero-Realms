#include "../../include/effets/EffetNextAcquiredToHand.hpp"
#include <iostream>

EffetNextAcquiredToHand::EffetNextAcquiredToHand(): Effet("Next acquired to hand"){}

void EffetNextAcquiredToHand::activerEffet(Player& proprietaire, Game& game){
    std::cout << "La prochaine carte acquise ira dans la main\n";
    proprietaire.setNextAcquiredToHand(true);
}

std::string EffetNextAcquiredToHand::toString() const {
    return std::string("La prochaine carte acquise ira dans votre main");
}
