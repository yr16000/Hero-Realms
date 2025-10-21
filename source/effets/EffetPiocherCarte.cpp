#include "../../include/effets/EffetPiocherCarte.hpp"

EffetPiocherCarte::EffetPiocherCarte(int nb) : Effet("Piocher carte"), nb(nb) {}

void EffetPiocherCarte::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.piocherCarte(nb);
}
