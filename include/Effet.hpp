#ifndef EFFET_HPP
#define EFFET_HPP
#include "Player.hpp"
#include "Game.hpp"

class Effet{
    private:

    public:
        virtual void activerEffet(Player& proprietaire,Game& game)=0;
};

#endif