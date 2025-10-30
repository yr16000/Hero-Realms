
#include "../include/CardLoader.hpp"
#include "../include/Carte.hpp"
#include "../include/Champion.hpp"
#include "../include/Action.hpp"
#include "../include/Objet.hpp"
#include "../include/Effet.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <iostream>
#include <unordered_map>

std::vector<std::unique_ptr<Carte>> CardLoader::loadBaseSet() {
    std::vector<std::unique_ptr<Carte>> cards;

    // IMPERIAL

    // 1 x Arkus, Imperial Dragon
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Arkus, Imperial Dragon",
            8,
            Faction::Imperiale,
            6,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Dragon,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Close Ranks
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Close Ranks",
            3,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Command
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Command",
            5,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Darian, War Mage  (Champion ◆ Human Mage)
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Darian, War Mage",
            4,
            Faction::Imperiale,
            5,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Domination
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Domination",
            7,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Cristov, the Just (Champion ◆ Human Paladin) 5 cost, 5 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Cristov, the Just",
            5,
            Faction::Imperiale,
            5,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainPaladin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Kraka, High Priest (Champion ◆ Human Priest) cost 6, def 6 (pas guard)
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Kraka, High Priest",
            6,
            Faction::Imperiale,
            6,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainPretre,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Man-at-Arms (Champion ◆ Human Warrior) cost 3, 4 Guard
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Man-at-Arms",
            3,
            Faction::Imperiale,
            4,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Master Weyan (Champion ◆ Human Monk) cost 4, 4 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Master Weyan",
            4,
            Faction::Imperiale,
            4,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainMoine,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Rally the Troops (Action) cost 4
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Rally the Troops",
            4,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Recruit (Action) cost 2
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Recruit",
            2,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Tithe Priest (Champion ◆ Human Priest) cost 2, def 3
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Tithe Priest",
            2,
            Faction::Imperiale,
            3,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainPretre,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Taxation (Action) cost 1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Taxation",
            1,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Word of Power (Action ◆ Spell) cost 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Word of Power",
            6,
            Faction::Imperiale,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,   // tu peux mettre TypeSecondaire::Sort si tu veux
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    //
    // ======================
    // GUILD
    // ======================
    //

    // 1 x Borg, Ogre Mercenary (Champion ◆ Ogre Warrior) cost 6, 6 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Borg, Ogre Mercenary",
            6,
            Faction::Guilde,
            6,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::OgreGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Bribe (Action) cost 3
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Bribe",
            3,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Death Threat (Action ◆ Assassin) cost 3
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Death Threat",
            3,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Assassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Deception (Action) cost 5
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Deception",
            5,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Fire Bomb (Action) cost 8
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Fire Bomb",
            8,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Hit Job (Action ◆ Assassin) cost 4
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Hit Job",
            4,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Assassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Intimidation (Action ◆ Rogue) cost 2
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Intimidation",
            2,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Voleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Myros, Guild Mage (Champion ◆ Human Mage) cost 5, def 3 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Myros, Guild Mage",
            5,
            Faction::Guilde,
            3,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Parov, the Enforcer (Champion ◆ Human Rogue) cost 5, def 5 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Parov, the Enforcer",
            5,
            Faction::Guilde,
            5,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainVoleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Profit (Action) cost 1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Profit",
            1,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Rake, Master Assassin (Champion ◆ Human Assassin) cost 7, def 7
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Rake, Master Assassin",
            7,
            Faction::Guilde,
            7,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainAssassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Rasmus, the Smuggler (Champion ◆ Human Rogue) cost 4, def 5
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Rasmus, the Smuggler",
            4,
            Faction::Guilde,
            5,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainVoleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Smash and Grab (Action ◆ Rogue) cost 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Smash and Grab",
            6,
            Faction::Guilde,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Voleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Street Thug (Champion ◆ Human Rogue) cost 3, def 4
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Street Thug",
            3,
            Faction::Guilde,
            4,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainVoleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    //
    // ======================
    // NECROS
    // ======================
    //

    // 2 x Cult Priest (Champion ◆ Human) cost 3, def 4
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Cult Priest",
            3,
            Faction::Necros,
            4,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Humain,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Dark Energy (Action ◆ Spell) cost 4
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Dark Energy",
            4,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Dark Reward (Action) cost 5
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Dark Reward",
            5,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Death Cultist (Champion ◆ Human) cost 2, def 3 Guard
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Death Cultist",
            2,
            Faction::Necros,
            3,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Humain,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Death Touch (Action ◆ Curse) cost 1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Death Touch",
            1,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Malediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Rayla, Endweaver (Champion ◆ Human Mage) cost 4, def 4
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Rayla, Endweaver",
            4,
            Faction::Necros,
            4,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Influence (Action) cost 2
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Influence",
            2,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Krythos, Master Vampire (Champion ◆ Vampire) cost 7, def 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Krythos, Master Vampire",
            7,
            Faction::Necros,
            6,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Vampire,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Life Drain (Action ◆ Curse) cost 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Life Drain",
            6,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Malediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Lys, the Unseen (Champion ◆ Vampire) cost 6, def 5 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Lys, the Unseen",
            6,
            Faction::Necros,
            5,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Vampire,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x The Rot (Action ◆ Curse) cost 3
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "The Rot",
            3,
            Faction::Necros,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Malediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Tyrannor, the Devourer (Champion ◆ Demon) cost 8, 6 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Tyrannor, the Devourer",
            8,
            Faction::Necros,
            6,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Demon,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Varrick, the Necromancer (Champion ◆ Human Necromancer) cost 5, def 3
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Varrick, the Necromancer",
            5,
            Faction::Necros,
            3,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainNecromancien,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    //
    // ======================
    // WILD
    // ======================
    //

    // 1 x Broelyn, Loreweaver (Champion ◆ Elf Mage) cost 4, def 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Broelyn, Loreweaver",
            4,
            Faction::Sauvage,
            6,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::ElfeMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Cron, the Berserker (Champion ◆ Human Warrior) cost 6, def 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Cron, the Berserker",
            6,
            Faction::Sauvage,
            6,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Dire Wolf (Champion ◆ Giant Wolf) cost 5, 5 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Dire Wolf",
            5,
            Faction::Sauvage,
            5,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::GeantLoup,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Elven Curse (Action ◆ Elf Curse) cost 3
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Elven Curse",
            3,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::ElfeMalediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Elven Gift (Action ◆ Elf) cost 2
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Elven Gift",
            2,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Elfe,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Grak, Storm Giant (Champion ◆ Giant) cost 8, 7 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Grak, Storm Giant",
            8,
            Faction::Sauvage,
            7,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Geant,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Nature's Bounty (Action) cost 4
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Nature's Bounty",
            4,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Orc Grunt (Champion ◆ Orc) cost 3, 3 Guard
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Orc Grunt",
            3,
            Faction::Sauvage,
            3,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::Orc,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Rampage (Action ◆ Orc) cost 6
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Rampage",
            6,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Orc,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Torgen Rocksplitter (Champion ◆ Troll Warrior) cost 7, 7 Guard
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Torgen Rocksplitter",
            7,
            Faction::Sauvage,
            7,
            true,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::TrollGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 3 x Spark (Action ◆ Spell) cost 1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Spark",
            1,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 1 x Wolf Form (Action ◆ Wolf) cost 5
    {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Action>(
            "Wolf Form",
            5,
            Faction::Sauvage,
            false,
            std::move(ec),
            std::move(ef),
            std::move(es),
            TypeSecondaire::Loup,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    // 2 x Wolf Shaman (Champion ◆ Human Priest) cost 2, def 4
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> ec;
        std::vector<std::unique_ptr<Effet>> ef;
        auto carte = std::make_unique<Champion>(
            "Wolf Shaman",
            2,
            Faction::Sauvage,
            4,
            false,
            std::move(ec),
            std::move(ef),
            TypeSecondaire::HumainPretre,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(carte));
    }

    //
    // ======================
    // ITEMS
    // ======================
    //

    // 16 x Fire Gem   Item ◆ Currency ◆ Gem   cost 2
    for (int i = 0; i < 16; ++i) {
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Objet>(
            "Fire Gem",
            2,
            2,      // gain 2 gold
            0,
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Gemme
        );
        cards.push_back(std::move(carte));
    }

    // 28 x Gold   Item ◆ Currency ◆ Coin   cost ?
    for (int i = 0; i < 28; ++i) {
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Objet>(
            "Gold",
            0,      // coût vide dans ton csv → 0
            1,      // {Gain 1 gold}
            0,
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Piece
        );
        cards.push_back(std::move(carte));
    }

    // 4 x Shortsword   Item ◆ Melee Weapon ◆ Sword
    for (int i = 0; i < 4; ++i) {
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Objet>(
            "Shortsword",
            0,
            0,
            2,      // {Gain 2 combat}
            std::move(es),
            TypeSecondaire::ArmeDeMelee,
            TypeTertiaire::Epee
        );
        cards.push_back(std::move(carte));
    }

    // 4 x Dagger   Item ◆ Melee Weapon ◆ Dagger
    for (int i = 0; i < 4; ++i) {
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Objet>(
            "Dagger",
            0,
            0,
            1,      // {Gain 1 combat}
            std::move(es),
            TypeSecondaire::ArmeDeMelee,
            TypeTertiaire::Dague
        );
        cards.push_back(std::move(carte));
    }

    // 4 x Ruby   Item ◆ Currency ◆ Gem
    for (int i = 0; i < 4; ++i) {
        std::vector<std::unique_ptr<Effet>> es;
        auto carte = std::make_unique<Objet>(
            "Ruby",
            0,
            2,      // {Gain 2 gold}
            0,
            std::move(es),
            TypeSecondaire::Aucun,
            TypeTertiaire::Gemme
        );
        cards.push_back(std::move(carte));
    }

    return cards;
}
