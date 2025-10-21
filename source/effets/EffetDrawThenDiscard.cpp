#include "../../include/effets/EffetDrawThenDiscard.hpp"
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
    }
}
