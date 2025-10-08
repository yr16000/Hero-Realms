#ifndef EFFET_HPP
#define EFFET_HPP
#include "Player.hpp"
#include "Game.hpp"
#include <string>

class Effet{
    private:
        std::string description;
    public:
        virtual void activerEffet(Player& proprietaire,Game& game)=0;
        std::string getDescription();
};

#endif