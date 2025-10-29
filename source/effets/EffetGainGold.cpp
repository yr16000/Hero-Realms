#include "../../include/effets/EffetGainGold.hpp"
#include "../../include/Player.hpp"

EffetGainGold::EffetGainGold(int montant) : Effet("Gain d'or"), montant(montant) {}

void EffetGainGold::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.modiffGold(montant);
}

std::string EffetGainGold::toString() const {
    return std::string("Gain de ") + std::to_string(montant) + " gold";
}
