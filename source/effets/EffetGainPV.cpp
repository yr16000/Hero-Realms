#include "../../include/effets/EffetGainPV.hpp"

EffetGainPV::EffetGainPV(int montant) : Effet("Gain de PV"), montant(montant) {}

void EffetGainPV::activerEffet(Player& proprietaire, Game& game) {
    proprietaire.soigner(montant);
}
