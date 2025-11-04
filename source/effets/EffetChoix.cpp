#include "../../include/effets/EffetChoix.hpp"
#include <iostream>

EffetChoix::EffetChoix(std::vector<std::unique_ptr<Effet>>&& opts)
    : Effet("Choix"), options(std::move(opts)) {}

void EffetChoix::activerEffet(Player& proprietaire, Game& game){
    if(options.empty()) return;
    
    int choix = 0;
    
    // Vérifier si c'est l'IA qui joue
    if (game.isAIPlayer(proprietaire.getId())) {
        HeuristicAI* ai = game.getAIPlayer();
        if (ai) {
            choix = ai->chooseFromEffects(options, proprietaire, game);
        } else {
            choix = 0; // Par défaut la première option
        }
    } else {
        // Joueur humain : demander via console
        std::cout << "Choisissez une option :\n";
        for(size_t i=0;i<options.size();++i){
            std::cout << i+1 << ": " << options[i]->toString() << "\n";
        }
        choix = 1;
        std::cin >> choix;
        if(choix<1 || choix>(int)options.size()) choix=1;
        choix--; // Convertir en index 0-based
    }
    
    if(choix >= 0 && choix < (int)options.size()) {
        options[choix]->activerEffet(proprietaire, game);
    }
}

std::string EffetChoix::toString() const {
    std::string s = "Choisissez un seul: ";
    for(size_t i=0;i<options.size();++i){
        if(i) s += " / ";
        if(options[i]) s += options[i]->toString();
    }
    return s;
}
