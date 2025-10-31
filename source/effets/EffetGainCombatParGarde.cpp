#include "../../include/effets/EffetGainCombatParGarde.hpp"
#include "../../include/Champion.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>

EffetGainCombatParGarde::EffetGainCombatParGarde(int montantParGarde): Effet("Gain combat par garde"), montantParGarde(montantParGarde){}

void EffetGainCombatParGarde::activerEffet(Player& proprietaire, Game& game){
    int nbGardes=-1;
    for(const auto& champPtr : proprietaire.getChampionsEnJeu()){
        auto champion = dynamic_cast<Champion*>(champPtr.get());
        if(champion && champion->getEstGarde()){
            nbGardes++;
        }
    }
    int combatGagne=nbGardes * montantParGarde;
    proprietaire.modiffCombat(combatGagne);
    std::cout<<combatGagne<<" de combat gagnés\n";
}

std::string EffetGainCombatParGarde::toString() const {
    return std::string("Gain de ") + std::to_string(montantParGarde) + " combat par garde";
}