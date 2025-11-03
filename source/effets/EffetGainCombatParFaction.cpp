#include "../../include/effets/EffetGainCombatParFaction.hpp"
#include "../../include/Champion.hpp"
#include <iostream>

EffetGainCombatParFaction::EffetGainCombatParFaction(int montantPar, Faction factionCible, bool inclureCetteCarte)
    : Effet("Gain combat par faction"), montantPar(montantPar), factionCible(factionCible), inclureCetteCarte(inclureCetteCarte) {}

void EffetGainCombatParFaction::activerEffet(Player& proprietaire, Game& game) {
    int compteur = 0;
    Carte* activante = game.getCarteEnActivation();

    // We count champions in play of the target faction (consistent with other "par" effects)
    for (const auto& ptr : proprietaire.getChampionsEnJeu()){
        Champion* champ = dynamic_cast<Champion*>(ptr.get());
        if (!champ) continue;
        if (champ->getFaction() != factionCible) continue;

        // if activante is this same card and we should exclude it, skip
        if (!inclureCetteCarte && activante != nullptr && ptr.get() == activante) continue;

        compteur++;
    }
    for(const auto& ptr : proprietaire.getMain()){
        if (ptr->getFaction() != factionCible) continue;
        // if activante is this same card and we should exclude it, skip
        if (!inclureCetteCarte && activante != nullptr && ptr.get() == activante) continue;

        compteur++;
    }

    int combatGagne = compteur * montantPar;
    if (combatGagne != 0) proprietaire.modiffCombat(combatGagne);
    std::cout << combatGagne << " de combat gagnés\n";
}

std::string EffetGainCombatParFaction::toString() const {
    return std::string("Gain de ") + std::to_string(montantPar) + " combat par " + to_string(factionCible) + (inclureCetteCarte ? " (incluant cette carte)" : " (excluant cette carte)");
}
