#ifndef EFFETNEXTACQUIREDTOHAND_HPP
#define EFFETNEXTACQUIREDTOHAND_HPP

#include "../Effet.hpp"

class EffetNextAcquiredToHand : public Effet{
    public:
        EffetNextAcquiredToHand();
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
