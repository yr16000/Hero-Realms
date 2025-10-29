#ifndef EFFETCARTEMAINDIRECT_HPP
#define EFFETCARTEMAINDIRECT_HPP

#include "../Effet.hpp"
#include "../Enums.h"

class EffetCarteMainDirect : public Effet {
private:
    TypeCarte typeRecherche;
public:
    EffetCarteMainDirect(TypeCarte type);
    void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif
