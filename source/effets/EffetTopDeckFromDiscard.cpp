#include "../../include/effets/EffetTopDeckFromDiscard.hpp"
#include <iostream>

EffetTopDeckFromDiscard::EffetTopDeckFromDiscard(): Effet("Top deck from discard"){}

void EffetTopDeckFromDiscard::activerEffet(Player& proprietaire, Game& game){
    std::cout << "choisit une carte dans la defausse pour la mettre sur le dessus du deck\n";
    for(size_t i=0;i<proprietaire.getDefausse().size();++i){
        std::cout << i+1 << ": " << proprietaire.getDefausse()[i]->getNom() << "\n";
    }
    int choix=1;
    std::cin >> choix;
    while(choix<1 || choix>(int)proprietaire.getDefausse().size()){
        std::cout << "Choix invalide, reessayez: ";
        std::cin >> choix;
    }
    proprietaire.getDeck().push_back(std::move(proprietaire.getDefausse()[choix-1]));
    proprietaire.getDefausse().erase(proprietaire.getDefausse().begin()+(choix-1));
    std::cout<< "Carte mise sur le dessus du deck.\n";
}
