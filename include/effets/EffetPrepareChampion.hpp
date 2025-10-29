#ifndef EFFETPREPARECHAMPION_HPP
#define EFFETPREPARECHAMPION_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetPrepareChampion : public Effet{
    public:
        EffetPrepareChampion();
        void activerEffet(Player& proprietaire, Game& game) override;
        std::string toString() const override;
};

#endif