#include "../../include/effets/EffetGainCombatParChampion.hpp"

EffetGainCombatParChampion::EffetGainCombatParChampion(int montantParChampion): Effet("Gain combat par champion"), montantParChampion(montantParChampion){}

void EffetGainCombatParChampion::activerEffet(Player& proprietaire, Game& game){
    // stub: calculer nb champions et ajouter combat
}
