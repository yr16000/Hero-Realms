#ifndef CARTE_HPP
#define CARTE_HPP
#include "Effet.hpp"
#include <string>
#include <vector>
#include <memory>


enum class SousTypeCarte {Aucun, Dragon, Humain, Mage, Paladin, Pretre, Guerrier, Moine, Ogre,
    Voleur, Assassin, Vampire, Necromancien, Demon, Elfe, Geant, Loup, Troll, Sort, Malédiction,
    Monnaie, ArmeMêlée, Épée, Dague, Gemme};
enum class Faction{Guilde, Necros, Imperiale, Sauvage};

class Carte{
    protected:
        std::string nom;
        int cout;
        Faction faction;
        SousTypeCarte typeSecondaire;
        SousTypeCarte typeTertiaire;
        std::vector<std::unique_ptr<Effet>> effetCarte;
        std::vector<std::unique_ptr<Effet>> effetFaction;
        bool utilisee;
    public:
        Carte(const std::string& nom, int cout, const Faction faction,
            std::vector<std::unique_ptr<Effet>>&& effetsCarte,
            std::vector<std::unique_ptr<Effet>>&& effetsFaction = {},
            const SousTypeCarte typeSecondaire=SousTypeCarte::Aucun,
            const SousTypeCarte typeTertiaire=SousTypeCarte::Aucun);
        virtual ~Carte();
        virtual void activer(Player proprietaire, Game& game) = 0;
        void resetUtilisation();
        bool estUtilisee() const;
        std::string getNom() const;
        SousTypeCarte getTypeSecondaire() const;
        SousTypeCarte getTypeTertiaire() const;
};

#endif