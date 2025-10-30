#include "../../include/effets/EffetDrawThenDiscard.hpp"
#include "../../include/Player.hpp"
#include <iostream>

EffetDrawThenDiscard::EffetDrawThenDiscard(int nb): Effet("Draw then discard"), nb(nb){}

void EffetDrawThenDiscard::activerEffet(Player& proprietaire, Game& game){
    std::cout << "Piocher "<< nb <<" puis defausser autant\n";
    proprietaire.piocherCarte(nb);
    int n=nb;
    proprietaire.afficherMain();
    while(n>0){
        std::cout << "Choisissez une carte a defausser:\n";
        proprietaire.afficherMain();
        int choix=1;
        std::cin >> choix;
        while(choix<1 || choix>(int)proprietaire.getMain().size()){
            std::cout << "Choix invalide, reessayez: ";
            std::cin >> choix;
        }
        proprietaire.getDefausse().push_back(std::move(proprietaire.getMain()[choix-1]));
        // remove the moved-from card from hand
        proprietaire.getMain().erase(proprietaire.getMain().begin() + (choix-1));
        // decrement remaining discards
        --n;
    }
}

std::string EffetDrawThenDiscard::toString() const {
    return std::string("Piocher ") + std::to_string(nb) + " puis défausser " + std::to_string(nb);
}
