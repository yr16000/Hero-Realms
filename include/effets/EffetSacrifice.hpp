#ifndef EFFETSACRIFICE_HPP
#define EFFETSACRIFICE_HPP

#include "../Effet.hpp"

class EffetSacrifice : public Effet{
    private:
        int montant;
    public:
        EffetSacrifice(int montant);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
