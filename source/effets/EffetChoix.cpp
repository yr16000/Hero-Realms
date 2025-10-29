#include "../../include/effets/EffetChoix.hpp"
#include <iostream>

EffetChoix::EffetChoix(std::vector<std::unique_ptr<Effet>>&& opts)
    : Effet("Choix"), options(std::move(opts)) {}

void EffetChoix::activerEffet(Player& proprietaire, Game& game){
    if(options.empty()) return;
    std::cout << "Choisissez une option :\n";
    for(size_t i=0;i<options.size();++i){
        std::cout << i+1 << ": " << options[i]->toString() << "\n";
    }
    int choix=1;
    std::cin >> choix;
    if(choix<1 || choix>(int)options.size()) choix=1;
    options[choix-1]->activerEffet(proprietaire, game);
}

std::string EffetChoix::toString() const {
    std::string s = "Choisissez un seul: ";
    for(size_t i=0;i<options.size();++i){
        if(i) s += " / ";
        if(options[i]) s += options[i]->toString();
    }
    return s;
}
