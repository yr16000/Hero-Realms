#include "../../include/effets/EffetPiocherCarte.hpp"

EffetPiocherCarte::EffetPiocherCarte(int nb) : Effet("Piocher carte"), nb(nb) {}

void EffetPiocherCarte::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.piocherCarte(nb);
}

std::string EffetPiocherCarte::toString() const {
    return std::string("Piocher ") + std::to_string(nb) + (nb>1?" cartes":" carte");
}
