#include "../include/Champion.hpp"
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>

Champion::Champion(const std::string& nom, int cout, const Faction faction,
    int pv, bool estGarde,
    std::vector<std::unique_ptr<Effet>>&& effetsCarte,
    std::vector<std::unique_ptr<Effet>>&& effetsFaction,
    const TypeSecondaire typeSecondaire,
    const TypeTertiaire typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Champion, std::move(effetsCarte), std::move(effetsFaction), typeSecondaire, typeTertiaire),
      pv(pv), estGarde(estGarde) {}

void Champion::activer(Player& proprietaire, Game& game) {
    // Ensure this champion is in the owner's championsEnJeu (in play).
    bool inPlay = false;
    for (const auto &ptr : proprietaire.getChampionsEnJeu()){
        if (ptr.get() == this) { inPlay = true; break; }
    }

    if (!inPlay) {
        // Try to find and move from hand
        auto &main = proprietaire.getMain();
        for (size_t i = 0; i < main.size(); ++i) {
            if (main[i].get() == this) {
                proprietaire.getChampionsEnJeu().push_back(std::move(main[i]));
                main.erase(main.begin() + i);
                inPlay = true;
                break;
            }
        }
    }

    std::cout << "Activer champion: " << getNom() << "\n";
    for(const auto& effet : effetCarte) {
        effet->activerEffet(proprietaire, game);
    }

    // Trigger faction effects if owner has a card of the same faction in hand
    for (const auto &carte : proprietaire.getMain()){
        if (carte->getFaction() == this->getFaction()) {
            for(const auto& effetFaction : effetFaction) {
                effetFaction->activerEffet(proprietaire, game);
            }
            break;
        }
    }

    setEstActiver(true);
}

void Champion::onSacrifice(Player& proprietaire, Game& game) {
    // rien car les champions n'ont pas d'effet de sacrifice
}

void Champion::subirDegat(int nb) {
    pv -= nb;
}

bool Champion::getEstGarde() const{ return estGarde; }
bool Champion::getEstActiver() { return estActiver; }
void Champion::setEstActiver(bool val) { estActiver = val; }
int Champion::getPv() const { return pv; }