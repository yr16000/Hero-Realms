#include "../include/Objet.hpp"
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>

//Constructeur de la classe Objet
Objet::Objet(const std::string& nom,
             int cout,
             const Faction faction,
             std::vector<std::unique_ptr<Effet>>&& effetsCarte,
             std::vector<std::unique_ptr<Effet>>&& effetsFaction,
             std::vector<std::unique_ptr<Effet>>&& effetsSacrifice,
             const TypeSecondaire typeSecondaire,
             const TypeTertiaire typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Objet,
            std::move(effetsCarte),
            std::move(effetsFaction),
            typeSecondaire, typeTertiaire),
      effetsSacrifice(std::move(effetsSacrifice)) {}

// Méthode d'activation de l'objet
void Objet::activer(Player& proprietaire, Game& game) {
    std::cout << "Activation de l'objet : " << getNom() << "\n";
    game.setCarteEnActivation(this);

    
    for (const auto& effet : effetCarte) {
        if (effet) effet->activerEffet(proprietaire, game);
    }

    game.setCarteEnActivation(nullptr);
}

// getter pour les effets de sacrifice
const std::vector<std::unique_ptr<Effet>>& Objet::getEffetsSacrifice() const {
    return effetsSacrifice;
}


// Méthode appelée lors du sacrifice de l'objet
void Objet::onSacrifice(Player& proprietaire, Game& game) {
    std::cout << "Sacrifice de l'objet : " << getNom() << "\n";
    for (const auto& effet : effetsSacrifice) {
        if (effet) effet->activerEffet(proprietaire, game);
    }


}
