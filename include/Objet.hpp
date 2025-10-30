#ifndef OBJET_HPP
#define OBJET_HPP
#include "Carte.hpp"

class Objet : public Carte{
    private:
        int gainGold;
        int gainCombat;
        std::vector<std::unique_ptr<Effet>> effetSacrifice;
    public:
        Objet(const std::string& nom, int cout,
            int gainGold, int gainCombat,
            std::vector<std::unique_ptr<Effet>>&& effetsSacrifice = {},
            const TypeSecondaire typeSecondaire=TypeSecondaire::Aucun,
            const TypeTertiaire typeTertiaire=TypeTertiaire::Aucun);
        void activer(Player& proprietaire, Game& game) override;
        void onSacrifice(Player& proprietaire, Game& game) override;

};

#endif