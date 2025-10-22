#ifndef EFFETCARTEDEFAUSSETOPDECK_HPP
#define EFFETCARTEDEFAUSSETOPDECK_HPP

#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetCarteDefausseTopDeck : public Effet{
    public:
	    EffetCarteDefausseTopDeck();
	    void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
