#ifndef EFFETTOPDECKNEXTACQUIRED_HPP
#define EFFETTOPDECKNEXTACQUIRED_HPP

#include "../Effet.hpp"

class EffetTopDeckNextAcquired : public Effet{
    public:
        EffetTopDeckNextAcquired();
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
