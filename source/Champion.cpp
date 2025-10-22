#include "../include/Champion.hpp"
#include <iostream>

Champion::Champion(const std::string& nom, int cout, const Faction faction,
    int pv, bool estGarde,
    std::vector<std::unique_ptr<Effet>>&& effetsCarte,
    std::vector<std::unique_ptr<Effet>>&& effetsFaction,
    const SousTypeCarte typeSecondaire,
    const SousTypeCarte typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Champion, std::move(effetsCarte), std::move(effetsFaction), typeSecondaire, typeTertiaire),
      pv(pv), estGarde(estGarde) {}

void Champion::activer(Player& proprietaire, Game& game) {
    std::cout << "Activer champion: " << getNom() << "\n";
    for(const auto& effet : effetCarte) {
        effet->activerEffet(proprietaire, game);
    }
    for(const auto& carte : proprietaire.getMain()){
        if (carte->getFaction() == this->getFaction()) {
            for(const auto& effetFaction : effetFaction) {
                effetFaction->activerEffet(proprietaire, game);
            }
            break;
        }
    }
    setEstActiver(true);
}

void Champion::subirDegat(int nb) {
    pv -= nb;
}

bool Champion::getEstGarde() { return estGarde; }
