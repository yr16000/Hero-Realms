#include "../../include/effets/EffetGainPvParChampion.hpp"
#include <iostream>

EffetGainPvParChampion::EffetGainPvParChampion(int parChampion): Effet("Gain PV par champion"), parChampion(parChampion){}

void EffetGainPvParChampion::activerEffet(Player& proprietaire, Game& game){
    int nbChampions=proprietaire.getChampionsEnJeu().size();
    int soinGagne=nbChampions * parChampion;
    proprietaire.soigner(soinGagne);
    std::cout<<soinGagne<<" PV gagnés";
}

std::string EffetGainPvParChampion::toString() const {
    return std::string("Gagner ") + std::to_string(parChampion) + " PV par champion en jeu";
}
