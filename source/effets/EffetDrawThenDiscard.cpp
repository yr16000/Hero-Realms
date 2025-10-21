#include "../../include/effets/EffetDrawThenDiscard.hpp"
#include <iostream>

EffetDrawThenDiscard::EffetDrawThenDiscard(int nb): Effet("Draw then discard"), nb(nb){}

void EffetDrawThenDiscard::activerEffet(Player& proprietaire, Game& game){
    std::cout << "EffetDrawThenDiscard: (stub) piocher "<< nb <<" puis defausser autant\n";
    proprietaire.piocherCarte(nb);
    // TODO: demander quelles cartes defausser
}
