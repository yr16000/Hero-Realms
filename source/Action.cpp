#include "../include/Action.hpp"
#include <iostream>

Action::Action(const std::string& nom, int cout, const Faction faction,
    bool sacrificeFlag,
    std::vector<std::unique_ptr<Effet>>&& effetsCarte,
    std::vector<std::unique_ptr<Effet>>&& effetsFaction,
    std::vector<std::unique_ptr<Effet>>&& effetsSacrifice,
    const TypeSecondaire typeSecondaire,
    const TypeTertiaire typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Action, std::move(effetsCarte), std::move(effetsFaction), typeSecondaire, typeTertiaire),
      effetSacrifice(std::move(effetsSacrifice)) {
    if(sacrificeFlag) {
        // handle sacrifice flag if needed
    }
}

Action::~Action() = default;

void Action::activer(Player& proprietaire, Game& game) {
    std::cout << "Activer action: " << getNom() << "\n";
    // set activation context so effects can detect the source
    game.setCarteEnActivation(this);
    for (const auto& effet : effetCarte) {
        if (effet) effet->activerEffet(proprietaire, game);
    }

    // clear activation context
    game.setCarteEnActivation(nullptr);

    int countAtStart = proprietaire.getFactionCount(this->getFaction());
    const int threshold = 2;
    if (countAtStart >= threshold) {
        for(const auto& effetFaction : effetFaction) {
            effetFaction->activerEffet(proprietaire, game);
        }
    }
}

const std::vector<std::unique_ptr<Effet>>& Action::getEffetsSacrifice() const {
    return effetSacrifice;
}

void Action::onSacrifice(Player& proprietaire, Game& game){
    // when this Action is sacrificed, run its sacrifice effects
    for (const auto &e : effetSacrifice) {
        if (e) e->activerEffet(proprietaire, game);
    }
}

