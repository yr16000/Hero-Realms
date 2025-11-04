#include "../../include/effets/EffetPiocherCarte.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"

EffetPiocherCarte::EffetPiocherCarte(int nb) : Effet("Piocher carte"), nb(nb) {}

void EffetPiocherCarte::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.piocherCarte(nb);
    std::cout << nb << " carte(s) piochée(s)\n";
}

std::string EffetPiocherCarte::toString() const {
    return std::string("Piocher ") + std::to_string(nb) + (nb>1?" cartes":" carte");
}
