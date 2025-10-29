#ifndef CARTE_HPP
#define CARTE_HPP
#include "Effet.hpp"
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>


enum class SousTypeCarte {Aucun, Dragon, Humain, Mage, Paladin, Pretre, Guerrier, Moine, Ogre,
    Voleur, Assassin, Vampire, Necromancien, Demon, Elfe, Geant, Loup, Troll, Sort, Malédiction,
    Monnaie, ArmeMêlée, Épée, Dague, Gemme};

class Carte{
    protected:
        std::string nom;
        int cout;
        Faction faction;
        TypeCarte typePrimaire;
        SousTypeCarte typeSecondaire;
        SousTypeCarte typeTertiaire;
        std::vector<std::unique_ptr<Effet>> effetCarte;
        std::vector<std::unique_ptr<Effet>> effetFaction;
        bool utilisee;
    public:
        Carte(const std::string& nom, int cout, const Faction faction, TypeCarte typePrimaire,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        virtual ~Carte();
        virtual void activer(Player& proprietaire, Game& game) = 0;
        virtual void onSacrifice(Player& proprietaire, Game& game);
        void resetUtilisation();
        bool estUtilisee() const;
        std::string getNom() const;
        Faction getFaction() const;
        SousTypeCarte getTypeSecondaire() const;
        SousTypeCarte getTypeTertiaire() const;
        TypeCarte getType() const;
    int getCout() const;
    const std::vector<std::unique_ptr<Effet>>& getEffetsCarte() const;
    const std::vector<std::unique_ptr<Effet>>& getEffetsFaction() const;
    // rendu texte uniforme de la carte
    std::string toString(unsigned width = 40) const;
        void afficherCarte() const;
};

#endif