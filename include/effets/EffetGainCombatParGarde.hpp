#ifndef EFFETGAINGAINCOMBATPARGARDE_HPP
#define EFFETGAINGAINCOMBATPARGARDE_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetGainCombatParGarde : public Effet{
    private:
        int montantParGarde;
    public:
        EffetGainCombatParGarde(int montantParGarde);
        void activerEffet(Player& proprietaire, Game& game) override;
        std::string toString() const override;
};

#endif