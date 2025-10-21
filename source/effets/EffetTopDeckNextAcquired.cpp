#include "../../include/effets/EffetTopDeckNextAcquired.hpp"
#include <iostream>

EffetTopDeckNextAcquired::EffetTopDeckNextAcquired(): Effet("Top deck next acquired"){}

void EffetTopDeckNextAcquired::activerEffet(Player& proprietaire, Game& game){
    std::cout << "La prochaine carte acquise ira sur le dessus du deck\n";
    proprietaire.setNextAcquiredToTopDeck(true);
}
