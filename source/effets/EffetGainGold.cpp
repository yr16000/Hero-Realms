#include "../../include/effets/EffetGainGold.hpp"

EffetGainGold::EffetGainGold(int montant) : Effet("Gain d'or"), montant(montant) {}

void EffetGainGold::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.modiffGold(montant);
}
