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
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        void activer(Player& proprietaire, Game& game) override;
        void onSacrifice(Player& proprietaire, Game& game) override;

};

#endif