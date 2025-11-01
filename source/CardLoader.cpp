#include "../include/CardLoader.hpp"

#include "../include/Carte.hpp"
#include "../include/Champion.hpp"
#include "../include/Action.hpp"
#include "../include/Objet.hpp"
#include "../include/Enums.h"

// tous les effets que tu m'as passés
#include "../include/effets/EffetGainGold.hpp"
#include "../include/effets/EffetGainCombat.hpp"
#include "../include/effets/EffetPiocherCarte.hpp"
#include "../include/effets/EffetGainPV.hpp"
#include "../include/effets/EffetPrepareChampion.hpp"
#include "../include/effets/EffetSacrifier.hpp"
#include "../include/effets/EffetActionTopDeck.hpp"
#include "../include/effets/EffetCarteMainDirect.hpp"
#include "../include/effets/EffetChampionDefausseTopDeck.hpp"
#include "../include/effets/EffetTopDeckFromDiscard.hpp" 
#include "../include/effets/EffetTopDeckNextAcquired.hpp"
#include "../include/effets/EffetNextAcquiredToHand.hpp"
#include "../include/effets/EffetGainPvParChampion.hpp"
#include "../include/effets/EffetGainCombatParGarde.hpp"
#include "../include/effets/EffetGainCombatParChampion.hpp"
#include "../include/effets/EffetDrawThenDiscard.hpp"
#include "../include/effets/EffetDefausserCarteAdversaire.hpp"
#include "../include/effets/EffetAssomerChampion.hpp"
#include "../include/effets/EffetChoix.hpp"
#include "../include/effets/EffetOptionnel.hpp"
#include "../include/effets/EffetGainCombatParFaction.hpp"


#include <memory>
#include <vector>

std::vector<std::unique_ptr<Carte>> CardLoader::loadPioche() {

    std::vector<std::unique_ptr<Carte>> cards;
    cards.reserve(200); // large, pour éviter les realloc

    // Base Set	1	Arkus, Imperial Dragon	"{Expend}: {Gain 5 combat} Draw a card.
    // {Imperial Ally}: {Gain 6 health}"	Champion ◆ Dragon	Imperial	8	6 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(5));
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainPV>(6));

        auto c = std::make_unique<Champion>(
            "Arkus, Imperial Dragon",
            8,
            Faction::Imperiale,
            6,      // DEF
            true,   // Guard
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::Dragon,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Close Ranks
    // "{Gain 5 combat}
    // +{2 combat} for each champion you have in play.
    // {Imperial Ally}: {Gain 6 health}"	Action	Imperial	3
    // → on met l’effet principal + l’ally, on ignore le “+2/champion” pour l’instant
    {
    // effets principaux
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    // 1) gain 5 combat
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(5));
    // 2) +2 combat par champion en jeu
    effetsCarte.push_back(std::make_unique<EffetGainCombatParChampion>(2));

    // effets de faction
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainPV>(6));

    auto c = std::make_unique<Action>(
        "Close Ranks",
        3,
        Faction::Imperiale,
        false,
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{}, // pas de sacrifice
        TypeSecondaire::Aucun,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	1	Command
    // "{Gain 2 gold} {Gain 3 combat} {Gain 4 health}
    // Draw a card."	Action	Imperial	5
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));
        effetsCarte.push_back(std::make_unique<EffetGainPV>(4));
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Action>(
            "Command",
            5,
            Faction::Imperiale,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{}, // pas d'ally
            std::vector<std::unique_ptr<Effet>>{}, // pas de sacrifice
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Darian, War Mage
    // "{Expend}: {Gain 3 combat} or {Gain 4 health}"	Champion ◆ Human Mage	Imperial	4	5
    // → on met les 2 effets pour affichage
    {
    // on crée les deux options possibles
    std::vector<std::unique_ptr<Effet>> options;

    // option 1 : Gain 3 combat
    options.push_back(std::make_unique<EffetGainCombat>(3));

    // option 2 : Gain 4 PV
    options.push_back(std::make_unique<EffetGainPV>(4));

    // on encapsule ces deux effets dans un EffetChoix
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetChoix>(std::move(options)));

    auto c = std::make_unique<Champion>(
        "Darian, War Mage",
        4,                      // coût
        Faction::Imperiale,     // faction
        5,                      // PV
        false,                  // pas garde
        std::move(effetsCarte), // 1 seul effet : un choix
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::HumainMage,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}

    // Base Set	1	Domination
    // "{Gain 6 combat} {Gain 6 health}
    // Draw a card.
    // {Imperial Ally}: Prepare a champion."	Action	Imperial	7
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(6));
        effetsCarte.push_back(std::make_unique<EffetGainPV>(6));
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPrepareChampion>());

        auto c = std::make_unique<Action>(
            "Domination",
            7,
            Faction::Imperiale,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Cristov, the Just
    // "{Expend}: {Gain 2 combat} {Gain 3 health}
    // {Imperial Ally}: Draw a card."	Champion ◆ Human Paladin	Imperial	5	5 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));
        effetsCarte.push_back(std::make_unique<EffetGainPV>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Cristov, the Just",
            5,
            Faction::Imperiale,
            5,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainPaladin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Kraka, High Priest
    // "{Expend}: {Gain 2 health}
    // Draw a card.
    // {Imperial Ally}: Gain 2 health for each champion..."	Champion ◆ Human Priest	Imperial	6	6
    // → on simplifie l’ally en “Gain 2 health”
    {
    // effets normaux (quand tu l'expend)
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetGainPV>(2));      // +2 PV
    effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1)); // piocher 1

    // effets de faction (Imperial Ally)
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    // 2 PV *par* champion
    effetsFaction.push_back(std::make_unique<EffetGainPvParChampion>(2));

    auto c = std::make_unique<Champion>(
        "Kraka, High Priest",
        6,
        Faction::Imperiale,
        6,
        false, // pas garde
        std::move(effetsCarte),
        std::move(effetsFaction),
        TypeSecondaire::HumainPretre,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	2	Man-at-Arms
    // "{Expend}: {Gain 2 combat} +{1 combat} for each other guard..."	Champion ◆ Human Warrior	Imperial	3	4 Guard
    for (int i = 0; i < 2; ++i) {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // gain fixe
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

    // bonus par garde (autres gardes compris)
    effetsCarte.push_back(std::make_unique<EffetGainCombatParGarde>(1));

    auto c = std::make_unique<Champion>(
        "Man-at-Arms",
        3,                       // coût
        Faction::Imperiale,
        4,                       // PV
        true,                    // Garde
        std::move(effetsCarte),
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::HumainGuerrier,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	1	Master Weyan
    // "{Expend}: {Gain 3 combat} +{1 combat} for each other champion..."	Champion ◆ Human Monk	Imperial	4	4 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));
        auto c = std::make_unique<Champion>(
            "Master Weyan",
            4,
            Faction::Imperiale,
            4,
            true,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::HumainMoine,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Rally the Troops
    // "{Gain 5 combat} {Gain 5 health}
    // {Imperial Ally}: Prepare a champion."	Action	Imperial	4
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(5));
        effetsCarte.push_back(std::make_unique<EffetGainPV>(5));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPrepareChampion>());

        auto c = std::make_unique<Action>(
            "Rally the Troops",
            4,
            Faction::Imperiale,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Recruit
    // "{Gain 2 gold} {Gain 3 health}
    // +{1 health} for each champion...
    // {Imperial Ally}: {Gain 1 gold}"	Action	Imperial	2
    for (int i = 0; i < 3; ++i) {
    // effets normaux
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetGainGold>(2));       // {Gain 2 gold}
    effetsCarte.push_back(std::make_unique<EffetGainPV>(3));         // {Gain 3 health}
    effetsCarte.push_back(std::make_unique<EffetGainPvParChampion>(1)); // +1 PV par champion

    // effets de faction
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainGold>(1));     // {Imperial Ally}: {Gain 1 gold}

    auto c = std::make_unique<Action>(
        "Recruit",
        2,
        Faction::Imperiale,
        false,
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de sacrifice
        TypeSecondaire::Aucun,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	2	Tithe Priest
    // "{Expend}: {Gain 1 gold} or Gain {1 health} for each champion..."	Champion ◆ Human Priest	Imperial	2	3
    // Base Set    2   Tithe Priest
for (int i = 0; i < 2; ++i) {
    // on va construire les options du choix
    std::vector<std::unique_ptr<Effet>> options;

    // option 1 : Gain 1 gold
    options.push_back(std::make_unique<EffetGainGold>(1));

    // option 2 : Gain 1 health (version simplifiée de ta carte)
    options.push_back(std::make_unique<EffetGainPvParChampion>(1));

    // maintenant on met ce choix dans les effets de la carte
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetChoix>(std::move(options)));

    auto c = std::make_unique<Champion>(
        "Tithe Priest",
        2,                      // coût
        Faction::Imperiale,
        3,                      // PV
        false,                  // pas garde
        std::move(effetsCarte), // 1 seul effet : un choix
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction pour l'instant
        TypeSecondaire::HumainPretre,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}
    

    // Base Set	3	Taxation
    // "{Gain 2 gold}
    // {Imperial Ally}: {Gain 6 health}"	Action	Imperial	1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainPV>(6));

        auto c = std::make_unique<Action>(
            "Taxation",
            1,
            Faction::Imperiale,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Word of Power
    // "Draw two cards.
    // {Imperial Ally}: {Gain 5 health}
    // {Sacrifice}: {Gain 5 combat}"	Action ◆ Spell	Imperial	6
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainPV>(5));

        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetGainCombat>(5));

        auto c = std::make_unique<Action>(
            "Word of Power",
            6,
            Faction::Imperiale,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::move(effetsSacrifice),
            TypeSecondaire::Sort,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }



        //
    // ============================================================
    // 2. GUILD
    // ============================================================
    //
    // Base Set	1	Borg, Ogre Mercenary
    // "{Expend}: {Gain 4 combat}"	Champion ◆ Ogre Warrior	Guild	6	6 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(4));

        auto c = std::make_unique<Champion>(
            "Borg, Ogre Mercenary",
            6,
            Faction::Guilde,
            6,
            true,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::OgreGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Bribe
    // "{Gain 3 gold}
    // {Guild Ally}: Put the next action you acquire this turn on top of your deck."	Action	Guild	3
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetActionTopDeck>());

        auto c = std::make_unique<Action>(
            "Bribe",
            3,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Death Threat
    // "{Gain 1 combat} Draw a card.
    // {Guild Ally}: Stun target champion."	Action ◆ Assassin	Guild	3
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(1));
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetAssomerChampion>());

        auto c = std::make_unique<Action>(
            "Death Threat",
            3,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Assassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Deception
    // "{Gain 2 gold} Draw a card.
    // {Guild Ally}: Put the next card you acquire this turn into your hand."	Action	Guild	5
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetCarteMainDirect>());

        auto c = std::make_unique<Action>(
            "Deception",
            5,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Fire Bomb
    // "{Gain 8 combat} Stun target champion. Draw a card.
    // {Sacrifice}: {Gain 5 combat}"	Action	Guild	8
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(8));
        effetsCarte.push_back(std::make_unique<EffetAssomerChampion>());
        effetsCarte.push_back(std::make_unique<EffetPiocherCarte>(1));

        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetGainCombat>(5));

        auto c = std::make_unique<Action>(
            "Fire Bomb",
            8,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::move(effetsSacrifice),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Hit Job
    // "{Gain 7 combat}
    // {Guild Ally}: Stun target champion."	Action ◆ Assassin	Guild	4
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(7));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetAssomerChampion>());

        auto c = std::make_unique<Action>(
            "Hit Job",
            4,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Assassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	2	Intimidation
    // "{Gain 5 combat}
    // {Guild Ally}: {Gain 2 gold}"	Action ◆ Rogue	Guild	2
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(5));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainGold>(2));

        auto c = std::make_unique<Action>(
            "Intimidation",
            2,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Voleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Myros, Guild Mage
    // "{Expend}: {Gain 3 gold}
    // {Guild Ally}: {Gain 4 combat}"	Champion ◆ Human Mage	Guild	5	3 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(4));

        auto c = std::make_unique<Champion>(
            "Myros, Guild Mage",
            5,
            Faction::Guilde,
            3,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Parov, the Enforcer
    // "{Expend}: {Gain 3 combat}
    // {Guild Ally}: Draw a card."	Champion ◆ Human Rogue	Guild	5	5 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Parov, the Enforcer",
            5,
            Faction::Guilde,
            5,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainVoleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Profit
    // "{Gain 2 gold}
    // {Guild Ally}: {Gain 4 combat}"	Action	Guild	1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(4));

        auto c = std::make_unique<Action>(
            "Profit",
            1,
            Faction::Guilde,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Rake, Master Assassin
    // "{Expend}: {Gain 4 combat} You may stun target champion."	Champion ◆ Human Assassin	Guild	7	7
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(4));
        effetsCarte.push_back(std::make_unique<EffetAssomerChampion>());

        auto c = std::make_unique<Champion>(
            "Rake, Master Assassin",
            7,
            Faction::Guilde,
            7,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::HumainAssassin,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Rasmus, the Smuggler
    // "{Expend}: {Gain 2 gold}
    // {Guild Ally}: Put the next card you acquire this turn on top of your deck."	Champion ◆ Human Rogue	Guild	4	5
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetActionTopDeck>());

        auto c = std::make_unique<Champion>(
            "Rasmus, the Smuggler",
            4,
            Faction::Guilde,
            5,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainVoleur,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Smash and Grab
    // "{Gain 6 combat} You may put a card from your discard pile on top of your deck."	Action ◆ Rogue	Guild	6
    {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // effet principal
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(6));

    // effet optionnel : remettre une carte de la défausse sur le dessus du deck
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetTopDeckFromDiscard>()
        )
    );

    auto c = std::make_unique<Action>(
        "Smash and Grab",
        6,
        Faction::Guilde,
        false,
        std::move(effetsCarte),
        std::vector<std::unique_ptr<Effet>>{},   // pas d'effet de faction
        std::vector<std::unique_ptr<Effet>>{},   // pas d'effet de sacrifice
        TypeSecondaire::Voleur,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	2	Street Thug
    // "{Expend}: {Gain 1 gold} or {Gain 2 combat}"	Champion ◆ Human Rogue	Guild	3	4
    for (int i = 0; i < 2; ++i) {
    // Les deux options du choix
    std::vector<std::unique_ptr<Effet>> options;

    // Option 1 : Gain 1 gold
    options.push_back(std::make_unique<EffetGainGold>(1));

    // Option 2 : Gain 2 combat
    options.push_back(std::make_unique<EffetGainCombat>(2));

    // On encapsule le tout dans un effet de choix
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetChoix>(std::move(options)));

    auto c = std::make_unique<Champion>(
        "Street Thug",
        3,                       // coût
        Faction::Guilde,          // faction
        4,                       // PV
        false,                   // pas garde
        std::move(effetsCarte),  // un seul effet : un choix
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::HumainVoleur,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}

    
    // 3. NECROS
   

    // Base Set	2	Cult Priest
    // "{Expend}: {Gain 1 gold} or {Gain 1 combat}
    // {Necros Ally}: {Gain 4 combat}"	Champion ◆ Human	Necros	3	4
    for (int i = 0; i < 2; ++i) {
    // ---- Effets principaux ----
    std::vector<std::unique_ptr<Effet>> options;
    options.push_back(std::make_unique<EffetGainGold>(1));   // option 1 : or
    options.push_back(std::make_unique<EffetGainCombat>(1)); // option 2 : combat

    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetChoix>(std::move(options)));

    // ---- Effets de faction ----
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainCombat>(4));

    auto c = std::make_unique<Champion>(
        "Cult Priest",
        3,                       // coût
        Faction::Necros,
        4,                       // PV
        false,                   // pas garde
        std::move(effetsCarte),  // choix principal
        std::move(effetsFaction),
        TypeSecondaire::Humain,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}

    // Base Set	1	Dark Energy
    // "{Gain 7 combat}
    // {Necros Ally}: Draw a card."	Action ◆ Spell	Necros	4
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(7));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Action>(
            "Dark Energy",
            4,
            Faction::Necros,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Sort,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Dark Reward
    // "{Gain 3 gold}
    // You may sacrifice a card in your hand or discard pile.
    // {Necros Ally}: {Gain 6 combat}"	Action	Necros	5
    {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1) gain 3 gold
    effetsCarte.push_back(std::make_unique<EffetGainGold>(3));

    // 2) you MAY sacrifice
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetSacrifier>(1)
        )
    );

    // faction
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainCombat>(6));

    auto c = std::make_unique<Action>(
        "Dark Reward",
        5,
        Faction::Necros,
        false,
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{},
        TypeSecondaire::Aucun,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}


    // Base Set	2	Death Cultist
    // "{Expend}: {Gain 2 combat}"	Champion ◆ Human	Necros	2	3 Guard
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

        auto c = std::make_unique<Champion>(
            "Death Cultist",
            2,
            Faction::Necros,
            3,
            true,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Humain,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Death Touch
    // "{Gain 2 combat}
    // You may sacrifice a card in your hand or discard pile.
    // {Necros Ally}: {Gain 2 combat}"	Action ◆ Curse	Necros	1
    for (int i = 0; i < 3; ++i) {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1) gain 2 combat
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

    // 2) you MAY sacrifice
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetSacrifier>(1)
        )
    );

    // faction
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainCombat>(2));

    auto c = std::make_unique<Action>(
        "Death Touch",
        1,
        Faction::Necros,
        false,
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{},
        TypeSecondaire::Malediction,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}


    // Base Set	1	Rayla, Endweaver
    // "{Expend}: {Gain 3 combat}
    // {Necros Ally}: Draw a card."	Champion ◆ Human Mage	Necros	4	4
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Rayla, Endweaver",
            4,
            Faction::Necros,
            4,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Influence
    // "{Gain 3 gold}
    // {Sacrifice}: {Gain 3 combat}"	Action	Necros	2
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(3));

        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetGainCombat>(3));

        auto c = std::make_unique<Action>(
            "Influence",
            2,
            Faction::Necros,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::move(effetsSacrifice),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Krythos, Master Vampire
    // "{Expend}: {Gain 3 combat}
    // You may sacrifice a card, if you do gain an additionnal +3 combat"	Champion ◆ Vampire	Necros	7	6
    {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1️⃣ effet de base : gain 3 combat
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));

    // 2️⃣ effet conditionnel : sacrifice (optionnel) + bonus si fait
    effetsCarte.push_back(
        std::make_unique<EffetSacrifier>(
            1, // max 1 carte
            std::make_unique<EffetGainCombat>(3) // effet bonus si sacrifié
        )
    );

    auto c = std::make_unique<Champion>(
        "Krythos, Master Vampire",
        7,                        // coût
        Faction::Necros,
        6,                        // PV
        false,                    // pas garde
        std::move(effetsCarte),   // effets
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::Vampire,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}

    // Base Set	1	Life Drain
    // "{Gain 8 combat}
    // You may sacrifice a card in your hand or discard pile.
    // {Necros Ally}: Draw a card."	Action ◆ Curse	Necros	6
    {
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1) gain 8 combat
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(8));

    // 2) you MAY sacrifice
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetSacrifier>(1)
        )
    );

    // faction
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

    auto c = std::make_unique<Action>(
        "Life Drain",
        6,
        Faction::Necros,
        false,
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{},
        TypeSecondaire::Malediction,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}


    // Base Set	1	Lys, the Unseen
    // "{Expend}: {Gain 2 combat}
    // You may sacrifice a card in your hand or discard pile.If you do, gain an additional +2 combat"	Champion ◆ Vampire	Necros	6	5 Guard
    // Base Set  1  Lys, the Unseen

{
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // effet de base
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));
    // effet de sacrifice
    effetsCarte.push_back(
        std::make_unique<EffetSacrifier>(
            1, // tu peux en sacrifier 1 max
            std::make_unique<EffetGainCombat>(2) // effetSiSacrifie
        )
    );

    auto c = std::make_unique<Champion>(
        "Lys, the Unseen",
        6,
        Faction::Necros,
        5,
        true, // Guard
        std::move(effetsCarte),
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::Vampire,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}


    // Base Set	2	The Rot
    // "{Gain 4 combat}
    // You may sacrifice a card...
    // {Necros Ally}: {Gain 3 combat}"	Action ◆ Curse	Necros	3
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(4));
        effetsCarte.push_back(std::make_unique<EffetSacrifier>(1));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(3));

        auto c = std::make_unique<Action>(
            "The Rot",
            3,
            Faction::Necros,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Malediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Tyrannor, the Devourer
    // "{Expend}: {Gain 4 combat}
    // You may sacrifice up to two cards...
    // {Necros Ally}: Draw a card."	Champion ◆ Demon	Necros	8	6 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(4));
        effetsCarte.push_back(std::make_unique<EffetSacrifier>(2));
        //effetsCarte.push_back(std::make_unique<EffetSacrifier>());

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Tyrannor, the Devourer",
            8,
            Faction::Necros,
            6,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::Demon,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Varrick, the Necromancer
    // "{Expend}: Take a champion from your discard pile and put it on top of your deck.
    // {Necros Ally}: Draw a card."	Champion ◆ Human Necromancer	Necros	5	3
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetChampionDefausseTopDeck>());

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Varrick, the Necromancer",
            5,
            Faction::Necros,
            3,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainNecromancien,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

        //
    // ============================================================
    // 4. WILD
    // ============================================================
    //

    // Base Set	1	Broelyn, Loreweaver
    // "{Expend}: {Gain 2 gold}
    // {Wild Ally}: Target opponent discards a card."	Champion ◆ Elf Mage	Wild	4	6
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        auto c = std::make_unique<Champion>(
            "Broelyn, Loreweaver",
            4,
            Faction::Sauvage,
            6,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::ElfeMage,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Cron, the Berserker
    // "{Expend}: {Gain 5 combat}
    // {Wild Ally}: Draw a card."	Champion ◆ Human Warrior	Wild	6	6
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(5));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Cron, the Berserker",
            6,
            Faction::Sauvage,
            6,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::HumainGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Dire Wolf
    // "{Expend}: {Gain 3 combat}
    // {Wild Ally}: {Gain 4 combat}"	Champion ◆ Giant Wolf	Wild	5	5 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(4));

        auto c = std::make_unique<Champion>(
            "Dire Wolf",
            5,
            Faction::Sauvage,
            5,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::GeantLoup,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	2	Elven Curse
    // "{Gain 6 combat} Target opponent discards a card.
    // {Wild Ally}: {Gain 3 combat}"	Action ◆ Elf Curse	Wild	3
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(6));
        effetsCarte.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(3));

        auto c = std::make_unique<Action>(
            "Elven Curse",
            3,
            Faction::Sauvage,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::ElfeMalediction,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Elven Gift
    // "{Gain 2 gold} You may draw a card. If you do, discard a card.
    // {Wild Ally}: {Gain 4 combat}"	Action ◆ Elf	Wild	2

    for (int i = 0; i < 3; ++i) {
    //  Effets principaux 
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1) Gain 2 gold
    effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

    // 2) You MAY draw a card, then discard a card
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetDrawThenDiscard>()
        )
    );

    // Effets de faction (Wild Ally) 
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetGainCombat>(4));

    auto c = std::make_unique<Action>(
        "Elven Gift",
        2,                         // coût
        Faction::Sauvage,
        false,                     // pas garde
        std::move(effetsCarte),
        std::move(effetsFaction),
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de sacrifice
        TypeSecondaire::Elfe,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}

   


    // Base Set	1	Grak, Storm Giant
    // "{Expend}: {Gain 6 combat}
    // You may draw a card. If you do, discard a card.
    // {Wild Ally}: Draw a card, then discard a card."	Champion ◆ Giant	Wild	8	7 Guard
    // Base Set  1  Grak, Storm Giant

{
    //  Effets principaux 
    std::vector<std::unique_ptr<Effet>> effetsCarte;

    // 1) {Gain 6 combat}
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(6));

    // 2) You MAY draw then discard
    effetsCarte.push_back(
        std::make_unique<EffetOptionnel>(
            std::make_unique<EffetDrawThenDiscard>()
        )
    );

    //  Effets de faction (Wild Ally) 
    std::vector<std::unique_ptr<Effet>> effetsFaction;
    effetsFaction.push_back(std::make_unique<EffetDrawThenDiscard>());

    auto c = std::make_unique<Champion>(
        "Grak, Storm Giant",
        8,                      // coût
        Faction::Sauvage,
        7,                      // PV
        true,                   // Garde
        std::move(effetsCarte),
        std::move(effetsFaction),
        TypeSecondaire::Geant,
        TypeTertiaire::Aucun
    );
    cards.push_back(std::move(c));
}


    // Base Set	1	Nature's Bounty
    // "{Gain 4 gold}
    // {Wild Ally}: Target opponent discards a card.
    // {Sacrifice}: {Gain 4 combat}"	Action	Wild	4
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(4));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetGainCombat>(4));

        auto c = std::make_unique<Action>(
            "Nature's Bounty",
            4,
            Faction::Sauvage,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::move(effetsSacrifice),
            TypeSecondaire::Aucun,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	2	Orc Grunt
    // "{Expend}: {Gain 2 combat}
    // {Wild Ally}: Draw a card."	Champion ◆ Orc	Wild	3	3 Guard
    for (int i = 0; i < 2; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetPiocherCarte>(1));

        auto c = std::make_unique<Champion>(
            "Orc Grunt",
            3,
            Faction::Sauvage,
            3,
            true,
            std::move(effetsCarte),
            std::move(effetsFaction),
            TypeSecondaire::Orc,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Rampage
    // "{Gain 6 combat} You may draw up to two cards, then discard that many cards."	Action ◆ Orc	Wild	6
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(6));
        effetsCarte.push_back(std::make_unique<EffetDrawThenDiscard>());

        auto c = std::make_unique<Action>(
            "Rampage",
            6,
            Faction::Sauvage,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Orc,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Torgen Rocksplitter
    // "{Expend}: {Gain 4 combat}
    // Target opponent discards a card."	Champion ◆ Troll Warrior	Wild	7	7 Guard
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(4));
        effetsCarte.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        auto c = std::make_unique<Champion>(
            "Torgen Rocksplitter",
            7,
            Faction::Sauvage,
            7,
            true,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::TrollGuerrier,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	3	Spark
    // "{Gain 3 combat} Target opponent discards a card.
    // {Wild Ally}: {Gain 2 combat}"	Action ◆ Spell	Wild	1
    for (int i = 0; i < 3; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(3));
        effetsCarte.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        std::vector<std::unique_ptr<Effet>> effetsFaction;
        effetsFaction.push_back(std::make_unique<EffetGainCombat>(2));

        auto c = std::make_unique<Action>(
            "Spark",
            1,
            Faction::Sauvage,
            false,
            std::move(effetsCarte),
            std::move(effetsFaction),
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Sort,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	1	Wolf Form
    // "{Gain 8 combat} Target opponent discards a card.
    // {Sacrifice}: Target opponent discards a card."	Action ◆ Wolf	Wild	5
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(8));
        effetsCarte.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetDefausserCarteAdversaire>());

        auto c = std::make_unique<Action>(
            "Wolf Form",
            5,
            Faction::Sauvage,
            false,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::move(effetsSacrifice),
            TypeSecondaire::Loup,
            TypeTertiaire::Aucun
        );
        cards.push_back(std::move(c));
    }

    // Base Set	2	Wolf Shaman
    // "{Expend}: {Gain 2 combat}
    // +{1 combat} for each other Wild card you have in play."	Champion ◆ Human Priest	Wild	2	4
    for (int i = 0; i < 2; ++i) {
    std::vector<std::unique_ptr<Effet>> effetsCarte;
    effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

    //  bonus par autre carte Sauvage
    effetsCarte.push_back(std::make_unique<EffetGainCombatParFaction>(
        1,
        Faction::Sauvage,
        false   // on exclut ce Wolf Shaman lui-même, comme dit "other Wild card"
    ));

    auto c = std::make_unique<Champion>(
        "Wolf Shaman",
        2,
        Faction::Sauvage,
        4,
        false,  // pas garde
        std::move(effetsCarte),
        std::vector<std::unique_ptr<Effet>>{}, // pas d'effet de faction
        TypeSecondaire::HumainPretre,
        TypeTertiaire::Aucun
    );

    cards.push_back(std::move(c));
}

return cards;
}


// Chargement de la pile spéciale "Fire Gems"

std::vector<std::unique_ptr<Carte>> CardLoader::loadFireGems(int count) {
    std::vector<std::unique_ptr<Carte>> fireGems;
    fireGems.reserve(count);

    for (int i = 0; i < count; ++i) {
        // 1) effets quand on joue la Fire Gem
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));   // {Gain 2 gold}

        // 2) pas d’effets de faction
        std::vector<std::unique_ptr<Effet>> effetsFaction;

        // 3) effets de sacrifice
        std::vector<std::unique_ptr<Effet>> effetsSacrifice;
        effetsSacrifice.push_back(std::make_unique<EffetGainCombat>(3)); // {Sacrifice}: Gain 3 combat

        fireGems.push_back(std::make_unique<Objet>(
            "Fire Gem",
            2,                            // coût
            Faction::Aucun,               // pas de faction
            std::move(effetsCarte),       // effets normaux
            std::move(effetsFaction),     // effets de faction
            std::move(effetsSacrifice),   // effets de sacrifice
            TypeSecondaire::Monnaie,
            TypeTertiaire::Gemme
        ));
    }

    return fireGems;
}

std::vector<std::unique_ptr<Carte>> CardLoader::loadPersonalDeck() {
    std::vector<std::unique_ptr<Carte>> deck;

    // 7 Gold : {Gain 1 gold}
    for (int i = 0; i < 7; ++i) {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(1));

        deck.push_back(std::make_unique<Objet>(
            "Gold",
            0,
            Faction::Aucun,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},   // pas d’effet de faction
            std::vector<std::unique_ptr<Effet>>{},   // pas de sacrifice
            TypeSecondaire::Monnaie,
            TypeTertiaire::Piece
        ));
    }

    // 1 Shortsword : {Gain 2 combat}
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(2));

        deck.push_back(std::make_unique<Objet>(
            "Shortsword",
            0,
            Faction::Aucun,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::ArmeDeMelee,
            TypeTertiaire::Epee
        ));
    }

    // 1 Dagger : {Gain 1 combat}
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainCombat>(1));

        deck.push_back(std::make_unique<Objet>(
            "Dagger",
            0,
            Faction::Aucun,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::ArmeDeMelee,
            TypeTertiaire::Dague
        ));
    }

    // 1 Ruby : {Gain 2 gold}
    {
        std::vector<std::unique_ptr<Effet>> effetsCarte;
        effetsCarte.push_back(std::make_unique<EffetGainGold>(2));

        deck.push_back(std::make_unique<Objet>(
            "Ruby",
            0,
            Faction::Aucun,
            std::move(effetsCarte),
            std::vector<std::unique_ptr<Effet>>{},
            std::vector<std::unique_ptr<Effet>>{},
            TypeSecondaire::Monnaie,
            TypeTertiaire::Gemme
        ));
    }

    return deck;
}




