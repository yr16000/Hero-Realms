#ifndef EFFETASSOMERCHAMPION_HPP
#define EFFETASSOMERCHAMPION_HPP

#include "../Effet.hpp"

class EffetAssomerChampion : public Effet{
    public:
        EffetAssomerChampion();
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
