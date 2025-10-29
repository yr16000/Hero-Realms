#include "../../include/effets/EffetActionTopDeck.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>

EffetActionTopDeck::EffetActionTopDeck(): Effet("Action top deck"){}

void EffetActionTopDeck::activerEffet(Player& proprietaire, Game& game){
    std::cout << "La prochaine action acquise ira sur le dessus du deck\n";
    proprietaire.setNextAcquiredActionToTopDeck(true);
}

std::string EffetActionTopDeck::toString() const {
    return std::string("La prochaine action acquise ira sur le dessus du deck");
}
