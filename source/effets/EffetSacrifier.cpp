#include "../../include/effets/EffetSacrifier.hpp"
#include "../../include/Player.hpp"
#include <iostream>
#include <vector>
#include <string>

EffetSacrifier::EffetSacrifier(int nombre, std::unique_ptr<Effet>&& effetSiSacrifie)
    : Effet("You may sacrifice up to N cards"), nombre(nombre), effetSiSacrifie(std::move(effetSiSacrifie)) {}

void EffetSacrifier::activerEffet(Player& proprietaire, Game& game){
    int n=nombre;
    while(n>0){
        std::cout << "Vous pouvez sacrifier jusqu'à " << n << " cartes de votre main ou defausse.\n";
        std::cout<<"Entrez 0 pour arrêter de sacrifier des cartes ou 1 pour continuer:\n";
        int choix;
        std::cin >> choix;
        if(choix==0){
            break;
        }
        bool fromMain=true;
        std::cout << "Votre main:\n";
        proprietaire.afficherMainDetaillee();
        std::cout << "Votre defausse:\n";
        proprietaire.afficherDefausse();
        std::cout<<"Sacrifier de la main ou de la defausse? (1/0): ";
        int choixSource;
        std::cin >> choixSource;
        if(choixSource==0){
            fromMain=false;
        }
        (choixSource==1?proprietaire.afficherMainDetaillee():proprietaire.afficherDefausse());
        std::cout << "Entrez le numéro de la carte à sacrifier: ";
        int index;
        std::cin >> index;
        if(fromMain){
            while(index<1 || index>(int)proprietaire.getMain().size()){
                std::cout << "Index invalide, reessayez: ";
                std::cin >> index;
            }
            Carte* carteASacrifier = proprietaire.getMain()[index-1].get();
            proprietaire.sacrifierCarte(carteASacrifier, game);
        } else {
            while(index<1 || index>(int)proprietaire.getDefausse().size()){
                std::cout << "Index invalide, reessayez: ";
                std::cin >> index;
            }
            Carte* carteASacrifier = proprietaire.getDefausse()[index-1].get();
            proprietaire.sacrifierCarte(carteASacrifier, game);
        }
        if(effetSiSacrifie){
            effetSiSacrifie->activerEffet(proprietaire, game);
        }
        n--;
    }
}

std::string EffetSacrifier::toString() const {
    std::string s = "Vous pouvez sacrifier jusqu'à ";
    s += std::to_string(nombre);
    s += " carte(s) de votre main ou de votre défausse";
    if(effetSiSacrifie) {
        s += ". Si vous le faites, ";
        s += effetSiSacrifie->toString();
    }
    return s;
}
