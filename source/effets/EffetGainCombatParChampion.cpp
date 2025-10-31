#include "../../include/effets/EffetGainCombatParChampion.hpp"
#include "../../include/Champion.hpp"
#include <iostream>

EffetGainCombatParChampion::EffetGainCombatParChampion(int montantParChampion): Effet("Gain combat par champion"), montantParChampion(montantParChampion){}

void EffetGainCombatParChampion::activerEffet(Player& proprietaire, Game& game){
    int nbChampions = (int)proprietaire.getChampionsEnJeu().size();

    // If the card currently activating effects is a champion belonging to the owner,
    // exclude it from the count (the card that triggered the effect should not count itself).
    Carte* activante = game.getCarteEnActivation();
    if (activante != nullptr) {
        // dynamic_cast to Champion to ensure it's a champion
        Champion* ch = dynamic_cast<Champion*>(activante);
        if (ch != nullptr) {
            // verify the champion belongs to the proprietor's championsEnJeu
            bool appartient = false;
            for (const auto& ptr : proprietaire.getChampionsEnJeu()){
                if (ptr.get() == activante) { appartient = true; break; }
            }
            if (appartient && nbChampions > 0) nbChampions -= 1;
        }
    }

    int combatGagne = nbChampions * montantParChampion;
    proprietaire.modiffCombat(combatGagne);
    std::cout<<combatGagne<<" de combat gagnés\n";
}

std::string EffetGainCombatParChampion::toString() const {
    return std::string("Gain de ") + std::to_string(montantParChampion) + " combat par champion";
}
