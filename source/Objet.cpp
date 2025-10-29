#include "../include/Objet.hpp"
#include "../include/Player.hpp"
#include "../include/Game.hpp"

Objet::Objet(const std::string& nom, int cout,
    int gainGold, int gainCombat,
    std::vector<std::unique_ptr<Effet>>&& effetsSacrifice,
    const SousTypeCarte typeSecondaire,
    const SousTypeCarte typeTertiaire)
    : Carte(nom, cout, Faction::Guilde, TypeCarte::Objet, {}, {}, typeSecondaire, typeTertiaire),
      gainGold(gainGold), gainCombat(gainCombat),
      effetSacrifice(std::move(effetsSacrifice)) {}

void Objet::activer(Player& proprietaire, Game& game) {
    proprietaire.modiffGold(gainGold);
    proprietaire.modiffCombat(gainCombat);
}
void Objet::onSacrifice(Player& proprietaire, Game& game) {
    for(const auto& effet : effetSacrifice){
        if(effet){
            effet->activerEffet(proprietaire, game);
        }
    }
}


