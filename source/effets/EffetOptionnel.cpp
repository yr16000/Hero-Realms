#include "../../include/effets/EffetOptionnel.hpp"
#include "../../include/ai/HeuristicAI.hpp"
#include <iostream>

EffetOptionnel::EffetOptionnel(std::unique_ptr<Effet> e)
    : Effet(std::string("Optionnel: ") + e->toString())
    , effet(std::move(e)) {}

void EffetOptionnel::activerEffet(Player& proprietaire, Game& game) {
    bool activate = false;
    
    // Vérifier si c'est l'IA qui joue
    if (game.isAIPlayer(proprietaire.getId())) {
        HeuristicAI* ai = game.getAIPlayer();
        if (ai) {
            activate = ai->shouldActivateOptionalEffect(effet.get(), proprietaire, game);
        }
    } else {
        // Joueur humain : demander via console
        std::cout << "Voulez-vous activer l'effet : " << effet->toString() << " ? (1: Oui, 0: Non)\n";
        int choix = 0;
        std::cin >> choix;
        activate = (choix == 1);
    }
    
    if(activate) {
        effet->activerEffet(proprietaire, game);
    }
}

std::string EffetOptionnel::toString() const {
    if(!effet) return "EffetOptionnel vide";
    std::string base = "Vous pouvez: ";
    base += effet->toString();
    return base;
}