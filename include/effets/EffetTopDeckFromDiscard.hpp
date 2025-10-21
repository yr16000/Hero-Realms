#ifndef EFFETTOPDECKFROMDISCARD_HPP
#define EFFETTOPDECKFROMDISCARD_HPP

#include "../Effet.hpp"

class EffetTopDeckFromDiscard : public Effet{
    public:
        EffetTopDeckFromDiscard();
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
