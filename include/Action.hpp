#ifndef ACTION_HPP
#define ACTION_HPP
#include "Carte.hpp"

class Action : public Carte{
    private:
        bool sacrifice;
    public:
        void activer(Player proprietaire, Game& game){}
};

#endif