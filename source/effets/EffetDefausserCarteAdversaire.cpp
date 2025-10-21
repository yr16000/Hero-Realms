#include "../../include/effets/EffetDefausserCarteAdversaire.hpp"
#include <iostream>

EffetDefausserCarteAdversaire::EffetDefausserCarteAdversaire(int nb): Effet("Defausser adversaire"), nb(nb){}

void EffetDefausserCarteAdversaire::activerEffet(Player& proprietaire, Game& game){
    std::cout << "EffetDefausserCarteAdversaire: (stub) forcer l'adversaire a defausser "<< nb << " carte(s)\n";
}
