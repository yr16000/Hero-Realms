#ifndef ENUMS_H
#define ENUMS_H

#include <string>

enum class TypeCarte { Champion, Action, Objet };
enum class Faction { Imperiale, Guilde, Necros, Sauvage };

enum class SousTypeCarteFR {
    Aucun,
    Dragon, Humain, Mage, Paladin, Pretre, Guerrier, Moine, Ogre,
    Voleur, Assassin, Vampire, Necromancien, Demon, Elfe, Geant, Loup, Troll,
    Sort, Malediction,
    Monnaie, ArmeMeelee, Epee, Dague, Gemme
};

inline std::string to_string(Faction f){
    switch(f){
        case Faction::Imperiale: return "Imperiale";
        case Faction::Guilde: return "Guilde";
        case Faction::Necros: return "Necros";
        case Faction::Sauvage: return "Sauvage";
    }
    return "Unknown";
}

#endif
