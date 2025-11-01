#ifndef OBJET_HPP
#define OBJET_HPP

#include "Carte.hpp"

class Objet : public Carte {
private:
    std::vector<std::unique_ptr<Effet>> effetsSacrifice;

public:
    Objet(const std::string& nom,
          int cout,
          const Faction faction,
          std::vector<std::unique_ptr<Effet>>&& effetsCarte,
          std::vector<std::unique_ptr<Effet>>&& effetsFaction,
          std::vector<std::unique_ptr<Effet>>&& effetsSacrifice,
          const TypeSecondaire typeSecondaire = TypeSecondaire::Aucun,
          const TypeTertiaire typeTertiaire = TypeTertiaire::Aucun);

    void activer(Player& proprietaire, Game& game) override;
    void onSacrifice(Player& proprietaire, Game& game) override;
};

#endif
