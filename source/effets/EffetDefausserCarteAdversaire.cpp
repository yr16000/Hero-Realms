#include "../../include/effets/EffetDefausserCarteAdversaire.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>

EffetDefausserCarteAdversaire::EffetDefausserCarteAdversaire(int nb): Effet("Defausser adversaire"), nb(nb){}

void EffetDefausserCarteAdversaire::activerEffet(Player& proprietaire, Game& game){
    std::cout << "Forcer l'adversaire a defausser "<< nb << " carte(s)\n";
    Player* adversaire = nullptr;
    for(auto& player: game.getPlayers()){
        if(player.getId() != proprietaire.getId()){
            adversaire = &player;
            break;
        }
    }
    if(adversaire == nullptr){
        std::cout << "Aucun adversaire trouve.\n";
        return;
    }
    for(int i=0;i<nb;++i){
        if(adversaire->getMain().empty()){
            std::cout << "L'adversaire n'a plus de cartes en main.\n";
            break;
        }
        std::cout << "Adversaire, choisissez une carte a defausser:\n";
        proprietaire.afficherMainDetaillee();
        int choix=1;
        std::cin >> choix;
        while(choix<1 || choix>(int)adversaire->getMain().size()){
            std::cout << "Choix invalide, reessayez: ";
            std::cin >> choix;
        }
        adversaire->getDefausse().push_back(std::move(adversaire->getMain()[choix-1]));
        adversaire->getMain().erase(adversaire->getMain().begin()+(choix-1));
        std::cout<< "Carte defaussee.\n";
    }
}

std::string EffetDefausserCarteAdversaire::toString() const {
    return std::string("Forcer l'adversaire à défausser ") + std::to_string(nb) + (nb > 1 ? " cartes" : " carte");
}
