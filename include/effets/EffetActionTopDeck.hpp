#ifndef EFFETACTIONTOPDECK_HPP
#define EFFETACTIONTOPDECK_HPP

#include "../Effet.hpp"

class EffetActionTopDeck : public Effet{
    public:
	    EffetActionTopDeck();
	    void activerEffet(Player& proprietaire, Game& game) override;
        std::string toString() const override;
};

#endif
