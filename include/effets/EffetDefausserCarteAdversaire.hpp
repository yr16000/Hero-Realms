#ifndef EFFETDEFAUSSERCARTEADVERSAIRE_HPP
#define EFFETDEFAUSSERCARTEADVERSAIRE_HPP

#include "../Effet.hpp"

class EffetDefausserCarteAdversaire : public Effet{
    private:
        int nb;
    public:
        EffetDefausserCarteAdversaire(int nb=1);
        void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
