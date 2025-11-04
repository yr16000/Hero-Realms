#include "../include/Champion.hpp"
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>

//Constructeur de la classe Champion
Champion::Champion(const std::string& nom, int cout, const Faction faction,
    int pv, bool estGarde,
    std::vector<std::unique_ptr<Effet>>&& effetsCarte,
    std::vector<std::unique_ptr<Effet>>&& effetsFaction,
    const TypeSecondaire typeSecondaire,
    const TypeTertiaire typeTertiaire)
    : Carte(nom, cout, faction, TypeCarte::Champion, std::move(effetsCarte), std::move(effetsFaction), typeSecondaire, typeTertiaire),
      pv(pv), estGarde(estGarde) {}

void Champion::activer(Player& proprietaire, Game& game) {
    // Si ce champion est déjà activé pour ce tour, ne rien faire.
    if (getEstActiver()) {
        std::cout << "Champion deja activé: " << getNom() << "\n";
        return;
    }
    // S'assurer que le champion est en jeu (sinon le déplacer de la main vers le jeu)
    bool inPlay = false;
    for (const auto &ptr : proprietaire.getChampionsEnJeu()){
        if (ptr.get() == this) { inPlay = true; break; }
    }

    if (!inPlay) {
        // Essayer de trouver et de déplacer depuis la main
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
    // Marquer cette carte comme la carte actuellement activée afin que les effets puissent détecter la source
    game.setCarteEnActivation(this);
    for(const auto& effet : effetCarte) {
        effet->activerEffet(proprietaire, game);
    }
    // Effacer le contexte d'activation
    game.setCarteEnActivation(nullptr);


    int countAtStart = proprietaire.getFactionCount(this->getFaction());
    const int threshold = 2;
    if (countAtStart >= threshold) {
        for(const auto& effetFaction : effetFaction) {
            effetFaction->activerEffet(proprietaire, game);
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