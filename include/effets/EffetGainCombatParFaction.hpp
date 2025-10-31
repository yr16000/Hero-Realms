#ifndef EFFETGAINCOMBATPARFACTION_HPP
#define EFFETGAINCOMBATPARFACTION_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"
#include "../Enums.h"

class EffetGainCombatParFaction : public Effet {
    private:
        int montantPar;
        Faction factionCible;
        bool inclureCetteCarte;
    public:
        EffetGainCombatParFaction(int montantPar, Faction factionCible, bool inclureCetteCarte = true);
        void activerEffet(Player& proprietaire, Game& game) override;
        std::string toString() const override;
};

#endif
