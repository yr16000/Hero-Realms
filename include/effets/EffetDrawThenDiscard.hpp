#ifndef EFFETDRAWTHENDISCARD_HPP
#define EFFETDRAWTHENDISCARD_HPP

#include "../Effet.hpp"

class EffetDrawThenDiscard : public Effet{
    private:
        int nb;
    public:
        EffetDrawThenDiscard(int nb=1);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
