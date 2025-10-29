#ifndef EFFETEXPENDCHAMPION_HPP
#define EFFETEXPENDCHAMPION_HPP

#include "../Effet.hpp"

class EffetChampionDefausseTopDeck : public Effet {
public:
    EffetChampionDefausseTopDeck();
    void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif
