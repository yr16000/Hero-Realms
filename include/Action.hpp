#ifndef ACTION_HPP
#define ACTION_HPP
#include "Carte.hpp"
#include <vector>
#include "Effet.hpp"

class Action : public Carte{
    private:
        std::vector<std::unique_ptr<Effet>> sacrifice;
    public:
        Action(const std::string& nom, int cout, const Faction faction,
            bool sacrifice,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        ~Action();
        void activer(Player& proprietaire, Game& game) override;
        void sacrifier(Player& proprietaire, Game& game);

};

#endif