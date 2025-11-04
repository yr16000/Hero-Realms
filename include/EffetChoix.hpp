#ifndef EFFETCHOIX_HPP
#define EFFETCHOIX_HPP
#include "Effet.hpp"
#include <vector>
#include <memory>
// Effet permettant de choisir un effet parmi plusieurs
class EffetChoix : public Effet{
    private:
        std::vector<std::unique_ptr<Effet>> choixEffets;
    public:
        void activerEffet(Player& proprietaire,Game& game) override;
};

#endif