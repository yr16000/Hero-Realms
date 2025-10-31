#ifndef EFFETCARTEMAINDIRECT_HPP
#define EFFETCARTEMAINDIRECT_HPP

#include "../Effet.hpp"


class EffetCarteMainDirect : public Effet {
public:
    EffetCarteMainDirect();

    void activerEffet(Player& proprietaire, Game& game) override;

    std::string toString() const override;
};

#endif
