#ifndef EFFETNEXTACQUIREDTOHAND_HPP
#define EFFETNEXTACQUIREDTOHAND_HPP

#include "../Effet.hpp"
#include "../Enums.h"

class EffetNextAcquiredToHand : public Effet {
private:
    TypeCarte typeRecherche;
public:
    EffetNextAcquiredToHand(TypeCarte type);
    void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif
