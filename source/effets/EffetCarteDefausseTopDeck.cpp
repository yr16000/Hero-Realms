#include "../../include/effets/EffetCarteDefausseTopDeck.hpp"
#include <iostream>

EffetCarteDefausseTopDeck::EffetCarteDefausseTopDeck(): Effet("Carte defausse top deck"){}

void EffetCarteDefausseTopDeck::activerEffet(Player& proprietaire, Game& game){
    std::cout << "Choisir une carte dans la defausse et la mettre sur le dessus de la pioche\n";
    for(size_t i=0;i<proprietaire.getDefausse().size();++i){
        proprietaire.getDefausse()[i]->afficherCarte();
        std::cout << "\n";
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
