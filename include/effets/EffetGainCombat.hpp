#ifndef EFFETGAINGCOMBAT_HPP
#define EFFETGAINGCOMBAT_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetGainCombat : public Effet{
    private:
        int montant;
    public:
        EffetGainCombat(int montant);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif