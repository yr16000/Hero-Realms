#ifndef CARTE_HPP
#define CARTE_HPP
#include "Effet.hpp"
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>


class Player;
class Game;

class Carte{
    protected:
        std::string nom;
        int cout;
        Faction faction;
        TypeCarte typePrimaire;
        TypeSecondaire typeSecondaire;
        TypeTertiaire typeTertiaire;
        std::vector<std::unique_ptr<Effet>> effetCarte;
        std::vector<std::unique_ptr<Effet>> effetFaction;
        bool utilisee;
    public:
        Carte(const std::string& nom, int cout, const Faction faction, TypeCarte typePrimaire,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const TypeSecondaire typeSecondaire=TypeSecondaire::Aucun,
            const TypeTertiaire typeTertiaire=TypeTertiaire::Aucun);
        virtual ~Carte();
        virtual void activer(Player& proprietaire, Game& game) = 0;
        virtual void onSacrifice(Player& proprietaire, Game& game);
        void resetUtilisation();
        bool estUtilisee() const;
        std::string getNom() const;
        Faction getFaction() const;
        TypeSecondaire getTypeSecondaire() const;
        TypeTertiaire getTypeTertiaire() const;
        TypeCarte getType() const;
        int getCout() const;
        const std::vector<std::unique_ptr<Effet>>& getEffetsCarte() const;
        const std::vector<std::unique_ptr<Effet>>& getEffetsFaction() const;
        // rendu texte uniforme de la carte
        std::string toString(unsigned width = 40) const;
        void afficherCarte() const;
};

#endif