#ifndef CHAMPION_HPP
#define CHANPION_HPP
#include "Carte.hpp"

class Champion : public Carte{
    private:
        int pv;
        bool estGarde;
    public:
        Champion(const std::string& nom, int cout, const Faction faction,
            int pv, bool estGarde,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        void activer(Player proprietaire, Game& game) override;
        void subirDegat(int nb);
        bool getEstGarde();

};


#endif