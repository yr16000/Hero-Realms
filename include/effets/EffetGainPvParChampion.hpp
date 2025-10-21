#ifndef EFFETGAINPVPARCHAMPION_HPP
#define EFFETGAINPVPARCHAMPION_HPP

#include "../Effet.hpp"

class EffetGainPvParChampion : public Effet{
    private:
        int perChampion;
    public:
        EffetGainPvParChampion(int perChampion=1);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
