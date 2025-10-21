#ifndef EFFET_HPP
#define EFFET_HPP
#include "Player.hpp"
#include "Game.hpp"
#include <string>

class Effet{
    private:
        std::string description;
    public:
        Effet(const std::string& desc);
        virtual ~Effet();
        virtual void activerEffet(Player& proprietaire, Game& game)=0;
        std::string getDescription() const;
};

#endif