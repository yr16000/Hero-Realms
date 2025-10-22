#include "../../include/effets/EffetGainCombatParGarde.hpp"
#include "../../include/Champion.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>

EffetGainCombatParGarde::EffetGainCombatParGarde(int montantParGarde): Effet("Gain combat par garde"), montantParGarde(montantParGarde){}

void EffetGainCombatParGarde::activerEffet(Player& proprietaire, Game& game){
    int nbGardes=0;
    for(const auto& champion : proprietaire.getChampionsEnJeu()){
        if(champion->getEstGarde()){
            nbGardes++;
        }
    }
    int combatGagne=nbGardes * montantParGarde;
    proprietaire.modiffCombat(combatGagne);
    std::cout<<combatGagne<<" de combat gagnés\n";
}