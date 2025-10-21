#include "../../include/effets/EffetAssomerChampion.hpp"
#include <iostream>

EffetAssomerChampion::EffetAssomerChampion() : Effet("Assomer champion") {}

void EffetAssomerChampion::activerEffet(Player& proprietaire, Game& game){
    std::cout << "EffetAssomerChampion: (stub) Choisir un champion cible pour assommer.\n";
    // TODO: demander au joueur cible et appliquer stun sur le champion
}
