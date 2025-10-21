#include "../../include/effets/EffetGainPvParChampion.hpp"
#include <iostream>

EffetGainPvParChampion::EffetGainPvParChampion(int perChampion): Effet("Gain PV par champion"), perChampion(perChampion){}

void EffetGainPvParChampion::activerEffet(Player& proprietaire, Game& game){
    int nbChampions=proprietaire.getChampionsEnJeu().size();
    int soinGagne=nbChampions * perChampion;
    proprietaire.soigner(soinGagne);
    std::cout<<soinGagne<<" PV gagnés";
}
