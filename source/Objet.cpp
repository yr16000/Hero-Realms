#include "../include/Objet.hpp"

Objet::Objet(const std::string& nom, int cout,
    int gainGold, int gainCombat,
    const SousTypeCarte typeSecondaire,
    const SousTypeCarte typeTertiaire)
    : Carte(nom, cout, Faction::Guilde, TypeCarte::Objet, {}, {}, typeSecondaire, typeTertiaire),
      gainGold(gainGold), gainCombat(gainCombat) {}
