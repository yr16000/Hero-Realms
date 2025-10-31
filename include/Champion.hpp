#ifndef CHAMPION_HPP
#define CHAMPION_HPP
#include "Carte.hpp"

class Champion : public Carte{
    private:
        int pv;
        bool estGarde;
        bool estActiver=false;
    public:
        Champion(const std::string& nom, int cout, const Faction faction,
            int pv, bool estGarde,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const TypeSecondaire typeSecondaire=TypeSecondaire::Aucun,
            const TypeTertiaire typeTertiaire=TypeTertiaire::Aucun);
        void activer(Player& proprietaire, Game& game) override;
        void onSacrifice(Player& proprietaire, Game& game) override;
        void subirDegat(int nb);
        bool getEstGarde() const;
        bool getEstActiver();
        void setEstActiver(bool val);
        int getPv() const;
};


#endif