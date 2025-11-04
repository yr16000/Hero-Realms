#ifndef ENUMS_H
#define ENUMS_H

#include <string>
// fichier pour les énumérations utilisées dans le jeu
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
        case Faction::Aucun: return "Aucun";
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


inline std::string to_string(TypeSecondaire t) {
    switch (t) {
        case TypeSecondaire::Aucun: return "Aucun";
        case TypeSecondaire::Assassin: return "Assassin";
        case TypeSecondaire::Monnaie: return "Monnaie";
        case TypeSecondaire::Malediction: return "Malediction";
        case TypeSecondaire::Demon: return "Demon";
        case TypeSecondaire::Dragon: return "Dragon";
        case TypeSecondaire::Elfe: return "Elfe";
        case TypeSecondaire::ElfeMalediction: return "Elfe Malediction";
        case TypeSecondaire::ElfeMage: return "Elfe Mage";
        case TypeSecondaire::Geant: return "Geant";
        case TypeSecondaire::GeantLoup: return "Geant Loup";
        case TypeSecondaire::Humain: return "Humain";
        case TypeSecondaire::HumainAssassin: return "Humain Assassin";
        case TypeSecondaire::HumainMage: return "Humain Mage";
        case TypeSecondaire::HumainMoine: return "Humain Moine";
        case TypeSecondaire::HumainNecromancien: return "Humain Necromancien";
        case TypeSecondaire::HumainPaladin: return "Humain Paladin";
        case TypeSecondaire::HumainPretre: return "Humain Pretre";
        case TypeSecondaire::HumainVoleur: return "Humain Voleur";
        case TypeSecondaire::HumainGuerrier: return "Humain Guerrier";
        case TypeSecondaire::ArmeDeMelee: return "Arme de melee";
        case TypeSecondaire::OgreGuerrier: return "Ogre Guerrier";
        case TypeSecondaire::Orc: return "Orc";
        case TypeSecondaire::Voleur: return "Voleur";
        case TypeSecondaire::Sort: return "Sort";
        case TypeSecondaire::TrollGuerrier: return "Troll Guerrier";
        case TypeSecondaire::Vampire: return "Vampire";
        case TypeSecondaire::Loup: return "Loup";
    }
    return "Unknown";
}

inline std::string to_string(TypeTertiaire t) {
    switch (t) {
        case TypeTertiaire::Aucun: return "Aucun";
        case TypeTertiaire::Gemme: return "Gemme";
        case TypeTertiaire::Dague: return "Dague";
        case TypeTertiaire::Epee:  return "Epee";
        case TypeTertiaire::Piece: return "Piece";
    }
    return "Unknown";
}


#endif

