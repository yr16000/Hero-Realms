#include "../include/Action.hpp"
#include <iostream>

Action::Action(const std::string& nom, int cout, const Faction faction,
    bool sacrificeFlag,
    std::vector<std::unique_ptr<Effet>>&& effetsCarte,
    std::vector<std::unique_ptr<Effet>>&& effetsFaction,
    const SousTypeCarte typeSecondaire,
    const SousTypeCarte typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Action, std::move(effetsCarte), std::move(effetsFaction), typeSecondaire, typeTertiaire) {
    if(sacrificeFlag) {
        // handle sacrifice flag if needed
    }
}

Action::~Action() = default;

void Action::activer(Player& proprietaire, Game& game) {
    std::cout << "Activer action: " << getNom() << "\n";
}

void Action::sacrifier(Player& proprietaire, Game& game) {
    // stub
}
