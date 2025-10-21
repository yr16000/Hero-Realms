#ifndef EFFETGAINGCOMBATPARCHAMPION_HPP
#define EFFETGAINGCOMBATPARCHAMPION_HPP
#include "../Effet.hpp"

class EffetGainCombatParChampion : public Effet{
    private:
        int montantParChampion;
    public:
        EffetGainCombatParChampion(int montantParChampion);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif