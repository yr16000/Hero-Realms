#include "../../include/effets/EffetGainCombatParChampion.hpp"
#include <iostream>

EffetGainCombatParChampion::EffetGainCombatParChampion(int montantParChampion): Effet("Gain combat par champion"), montantParChampion(montantParChampion){}

void EffetGainCombatParChampion::activerEffet(Player& proprietaire, Game& game){
    int nbChampions=proprietaire.getChampionsEnJeu().size();
    int combatGagne=nbChampions * montantParChampion;
    proprietaire.modiffCombat(combatGagne);
    std::cout<<combatGagne<<" de combat gagnés\n";
}

std::string EffetGainCombatParChampion::toString() const {
    return std::string("Gain de ") + std::to_string(montantParChampion) + " combat par champion";
}
