#include "../../include/effets/EffetDrawThenDiscard.hpp"
#include "../../include/Player.hpp"
#include <iostream>

EffetDrawThenDiscard::EffetDrawThenDiscard(int nb): Effet("Draw then discard"), nb(nb){}

void EffetDrawThenDiscard::activerEffet(Player& proprietaire, Game& game){
    std::cout << "Piocher "<< nb <<" puis defausser autant\n";
    int n=nb;
    bool stop=false;
    while(n>0 && !stop){
        proprietaire.piocherCarte(1);
        proprietaire.afficherMainDetaillee();
        std::cout << "Choisissez une carte a defausser:\n";
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
        if(n>0){
            std::cout << "Voulez-vous arrêter de défausser des cartes? (1=oui, 0=non): ";
            int reponse;
            std::cin >> reponse;
            if(reponse==1){
                stop=true;
            }
        }
    }
}

std::string EffetDrawThenDiscard::toString() const {
    return std::string("Piocher ") +((nb==1)?std::to_string(nb):"jusqu'à " + std::to_string(nb)) + " puis défausser autant";
}
