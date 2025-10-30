#ifndef ENUMS_H
#define ENUMS_H

#include <string>

enum class TypeCarte { 
    Champion,
    Action, 
    Objet //Objet
    };

enum class Faction {
    Aucun = 0,
    Imperiale,
    Guilde, 
    Necros,
    Sauvage 
};

enum class TypeSecondaire {
    Aucun = 0,

    Assassin,              // Assassin
    Monnaie,               // Currency
    Malediction,           // Curse
    Demon,                 // Demon
    Dragon,                // Dragon

    Elfe,                  // Elf
    ElfeMalediction,       // Elf Curse
    ElfeMage,              // Elf Mage

    Geant,                 // Giant
    GeantLoup,             // Giant Wolf

    Humain,                // Human
    HumainAssassin,        // Human Assassin
    HumainMage,            // Human Mage
    HumainMoine,           // Human Monk
    HumainNecromancien,    // Human Necromancer
    HumainPaladin,         // Human Paladin
    HumainPretre,          // Human Priest
    HumainVoleur,          // Human Rogue
    HumainGuerrier,        // Human Warrior

    ArmeDeMelee,           // Melee Weapon

    OgreGuerrier,          // Ogre Warrior

    Orc,                   // Orc
    Voleur,                // Rogue
    Sort,                  // Spell

    TrollGuerrier,         // Troll Warrior

    Vampire,               // Vampire
    Loup                   // Wolf
};


enum class TypeTertiaire {
    Aucun = 0,

    Gemme,  // Gem
    Dague,  // Dagger
    Epee,   // Sword
    Piece   // Coin
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

inline std::string to_string(TypeCarte t){
    switch(t){
        case TypeCarte::Champion: return "Champion";
        case TypeCarte::Action: return "Action";
        case TypeCarte::Objet: return "Objet";
    }
    return "Unknown";
}

#endif
