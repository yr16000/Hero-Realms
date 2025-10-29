#ifndef EFFETGAINPVPARCHAMPION_HPP
#define EFFETGAINPVPARCHAMPION_HPP

#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetGainPvParChampion : public Effet{
    private:
        int parChampion;
    public:
        EffetGainPvParChampion(int parChampion=1);
        void activerEffet(Player& proprietaire, Game& game) override;
        std::string toString() const override;
};

#endif
