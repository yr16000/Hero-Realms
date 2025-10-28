#ifndef ACTION_HPP
#define ACTION_HPP
#include "Carte.hpp"
#include <vector>
#include "Effet.hpp"

class Action : public Carte{
    private:
        std::vector<std::unique_ptr<Effet>> effetSacrifice;
    public:
        Action(const std::string& nom, int cout, const Faction faction,
            bool sacrificeFlag,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            std::vector<std::unique_ptr<Effet>>&& effetsSacrifice = {},
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        ~Action();
        void activer(Player& proprietaire, Game& game) override;
        void onSacrifice(Player& proprietaire, Game& game) override;

};

#endif