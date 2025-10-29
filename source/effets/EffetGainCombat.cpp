#include "../../include/effets/EffetGainCombat.hpp"

EffetGainCombat::EffetGainCombat(int montant) : Effet("Gain de combat"), montant(montant) {}

void EffetGainCombat::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.modiffCombat(montant);
}

std::string EffetGainCombat::toString() const {
    return std::string("Gain de ") + std::to_string(montant) + " combat";
}
