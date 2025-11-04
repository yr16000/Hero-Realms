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
        // gérer le flag de sacrifice si nécessaire
    }
}

Action::~Action() = default;

void Action::activer(Player& proprietaire, Game& game) {
    std::cout << "Activer action: " << getNom() << "\n";
    // mettre cette carte comme la carte actuellement activée
    game.setCarteEnActivation(this);
    for (const auto& effet : effetCarte) {
        if (effet) effet->activerEffet(proprietaire, game);
    }

    // effacer le contexte d'activation
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
    //quand l'action est sacrifiée, activer les effets de sacrifice
    for (const auto &e : effetSacrifice) {
        if (e) e->activerEffet(proprietaire, game);
    }
}

