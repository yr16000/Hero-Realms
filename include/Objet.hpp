#ifndef OBJET_HPP
#define OBJET_HPP
#include "Carte.hpp"

class Objet : public Carte{
    private:
        int gainGold;
        int gainCombat;
    public:
        Objet(const std::string& nom, int cout,
            int gainGold, int gainCombat,
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);

};

#endif