#ifndef EFFETGAINGOLD_HPP
#define EFFETGAINGOLD_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetGainGold : public Effet{
    private:
        int montant;
    public:
        EffetGainGold(int montant);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
