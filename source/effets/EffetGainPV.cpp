#include "../../include/effets/EffetGainPV.hpp"
#include "../../include/Player.hpp"

EffetGainPV::EffetGainPV(int montant) : Effet("Gain de PV"), montant(montant) {}

void EffetGainPV::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.soigner(montant);
}

std::string EffetGainPV::toString() const {
    return std::string("Gagner ") + std::to_string(montant) + " PV";
}
