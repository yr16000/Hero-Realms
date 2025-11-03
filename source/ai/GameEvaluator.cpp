#include "../../include/ai/GameEvaluator.hpp"
#include "../../include/Champion.hpp"
#include "../../include/Action.hpp"
#include "../../include/Objet.hpp"
#include "../../include/effets/Effets.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <map>
#include <typeinfo>

std::string GameEvaluator::Evaluation::toDebugString() const {
    std::ostringstream oss;
    oss << "Total: " << totalScore 
        << " [HP: " << hpScore 
        << ", Board: " << boardScore
        << ", Hand: " << handScore
        << ", Deck: " << deckQualityScore
        << ", Resources: " << resourceScore
        << ", Tempo: " << tempoScore << "]";
    return oss.str();
}

// Fonction helper pour évaluer la valeur d'un effet spécifique
static float evaluateEffectValue(const Effet* effet) {
    if (!effet) return 0.0f;
    
    // Utiliser typeid pour identifier le type d'effet
    const std::type_info& type = typeid(*effet);
    
    // Effets de generation de ressources
    if (type == typeid(EffetGainCombat)) return 5.0f;
    if (type == typeid(EffetGainGold)) return 6.0f;
    if (type == typeid(EffetGainPV)) return 4.0f;
    
    // Effets de pioche
    if (type == typeid(EffetPiocherCarte)) return 8.0f;
    
    // Effets conditionnels de ressources (bons avec synergie)
    if (type == typeid(EffetGainCombatParChampion)) return 6.0f;
    if (type == typeid(EffetGainPvParChampion)) return 5.0f;
    if (type == typeid(EffetGainCombatParFaction)) return 4.0f;
    if (type == typeid(EffetGainCombatParGarde)) return 3.0f;
    
    // Effets d'acquisition et manipulation de deck
    if (type == typeid(EffetNextAcquiredToHand)) return 7.0f;
    if (type == typeid(EffetTopDeckNextAcquired)) return 6.0f;
    if (type == typeid(EffetCarteMainDirect)) return 8.0f;
    if (type == typeid(EffetActionTopDeck)) return 5.0f;
    if (type == typeid(EffetCarteDefausseTopDeck)) return 7.0f;
    if (type == typeid(EffetTopDeckFromDiscard)) return 7.0f;
    if (type == typeid(EffetChampionDefausseTopDeck)) return 5.0f;
    
    // Effets de controle
    if (type == typeid(EffetAssomerChampion)) return 10.0f;
    if (type == typeid(EffetDefausserCarteAdversaire)) return 6.0f;
    if (type == typeid(EffetPrepareChampion)) return 8.0f;
    
    // Effets de choix et optionnels
    if (type == typeid(EffetChoix)) return 8.0f;
    if (type == typeid(EffetOptionnel)) return 5.0f;
    if (type == typeid(EffetDrawThenDiscard)) return 4.0f;
    
    // Effets de sacrifice
    if (type == typeid(EffetSacrifier)) return 3.0f;
    
    //valeur par défaut
    return 2.0f;
}

GameEvaluator::Evaluation GameEvaluator::evaluateGameState(
    Game& game, Player& player, Player& opponent) {
    
    Evaluation eval;

    // 1 Evaluation des HP (différence de HP)
    int hpDiff = player.getHp() - opponent.getHp();
    eval.hpScore = static_cast<float>(hpDiff) * HP_WEIGHT;
    
    // Bonus/malus si on est proche de la mort ou de la victoire
    if (player.getHp() <= 15) {
        eval.hpScore -= 20.0f; // Penalité forte si proche de la mort
    }
    if (opponent.getHp() <= 15) {
        eval.hpScore += 20.0f; // Bonus si adversaire proche de la mort
    }

    // 2. Evaluation du controle du board
    eval.boardScore = evaluateBoardControl(player, opponent) * BOARD_WEIGHT;

    // 3. Évaluation de la main actuelle
    eval.handScore = evaluateHandPotential(player) * HAND_WEIGHT;

    // 4. Évaluation de la qualité du deck
    eval.deckQualityScore = evaluateDeckQuality(player) * DECK_QUALITY_WEIGHT;

    // 5. Évaluation des ressources actuelles (gold et combat)
    eval.resourceScore = (player.getGold() * 2.0f + player.getAtk() * 1.5f) * RESOURCE_WEIGHT;

    // 6. Évaluation du tempo (synergie et momentum)
    eval.tempoScore = evaluateSynergy(player) * TEMPO_WEIGHT;

    // Score total
    eval.totalScore = eval.hpScore + eval.boardScore + eval.handScore 
                    + eval.deckQualityScore + eval.resourceScore + eval.tempoScore;

    return eval;
}

float GameEvaluator::evaluateCardValue(const Carte* carte) {
    if (!carte) return 0.0f;

    // Cartes de départ (faible valeur)
    if (isStartingCard(carte)) {
        std::string nom = carte->getNom();
        if (nom.find("Dagger") != std::string::npos) {
            return STARTING_DAGGER_VALUE;
        }
        if (nom.find("Gold") != std::string::npos) {
            return STARTING_GOLD_VALUE;
        }
        if (nom.find("Ruby") != std::string::npos) {
            return STARTING_RUBY_VALUE;
        }
        if (nom.find("Shortsword") != std::string::npos) {
            return STARTING_SHORT_SWORD_VALUE;
        }
    }

    float value = 0.0f;
    
    // Valeur de base selon le type
    TypeCarte type = carte->getType();
    if (type == TypeCarte::Champion) {
        const Champion* champ = dynamic_cast<const Champion*>(carte);
        if (champ) {
            value = CHAMPION_BASE_VALUE + champ->getPv() * 3.0f;
            if (champ->getEstGarde()) {
                value += 10.0f; // Les gardes sont précieux
            }
        } else {
            value = CHAMPION_BASE_VALUE;
        }
    } else if (type == TypeCarte::Action) {
        value = ACTION_BASE_VALUE;
        if(static_cast<const Action*>(carte)->get) {
            value += 5.0f; // Bonus pour les actions multi-usage
        }
    }

    // Bonus basé sur le coût (cartes plus chères sont généralement meilleures)
    int cost = carte->getCout();
    value += cost * 3.0f;

    // Bonus pour les gemmes de feu
    if (carte->getNom().find("Fire Gem") != std::string::npos) {
        value += 5.0f;
    }

    // Évaluer chaque effet de la carte
    for (const auto& effet : carte->getEffetsCarte()) {
        value += evaluateEffectValue(effet.get());
    }

    // Les effets de faction valent un peu plus
    for (const auto& effet : carte->getEffetsFaction()) {
        value += evaluateEffectValue(effet.get()) * 1.2f;
    }


    return value;
}

float GameEvaluator::evaluateCardInContext(const Carte* carte, Player& player, Game& game) {
    
    float baseValue = evaluateCardValue(carte);
    float contextBonus = 0.0f;

    if (!carte) return 0.0f;
    if(carte->getFaction() != Faction::Aucun){
        Faction faction = carte->getFaction();
    
        // Bonus si on a deja des cartes de cette faction
        int factionCount = player.getFactionCount(faction);
        if (factionCount > 0) {
            contextBonus += factionCount * 5.0f;
        }
    }

    // Bonus si c'est un champion et qu'on en manque
    if (carte->getType() == TypeCarte::Champion) {
        int championsInPlay = static_cast<int>(player.getChampionsEnJeu().size());
        if (championsInPlay < 2) {
            contextBonus += 10.0f;
        }
    }

    // Bonus si c'est une action et qu'on a beaucoup de champions
    if (carte->getType() == TypeCarte::Action && player.getChampionsEnJeu().size() >= 5) {
        contextBonus += 10.0f;
    }

    return baseValue + contextBonus;
}

float GameEvaluator::evaluateDeckQuality(Player& player) {
    float quality = 0.0f;
    
    // Compter toutes les cartes (deck + defausse + main + champions)
    const auto& deckNb = const_cast<Player&>(const_cast<Player&>(player)).getDeck();
    const auto& defausseNb = const_cast<Player&>(player).getDefausse();
    const auto& mainNb = const_cast<Player&>(player).getMain();
    const auto& champsNb = const_cast<Player&>(player).getChampionsEnJeu();

    int totalCards = static_cast<int>(deckNb.size() + 
                                      defausseNb.size() + 
                                      mainNb.size() + 
                                      champsNb.size());
    
    if (totalCards == 0) return 0.0f;

    // Valeur totale du deck
    float totalValue = 0.0f;
    
    for (const auto& c : deckNb) {
        totalValue += evaluateCardValue(c.get());
    }
    for (const auto& c : defausseNb) {
        totalValue += evaluateCardValue(c.get());
    }
    for (const auto& c : mainNb) {
        totalValue += evaluateCardValue(c.get());
    }
    for (const auto& c : champsNb) {
        totalValue += evaluateCardValue(c.get());
    }

    // Qualite moyenne par carte
    quality = totalValue / static_cast<float>(totalCards);

    // Penalite si trop de cartes de depart
    int startingCards = 0;
    for (const auto& c : deckNb) {
        if (isStartingCard(c.get())) startingCards++;
    }
    for (const auto& c : defausseNb) {
        if (isStartingCard(c.get())) startingCards++;
    }
    
    float startingRatio = static_cast<float>(startingCards) / static_cast<float>(totalCards);
    if (startingRatio > 0.5f) {
        quality -= (startingRatio - 0.5f) * 20.0f;
    }

    return quality;
}

float GameEvaluator::evaluateBoardControl(Player& player, Player& opponent) {
    float control = 0.0f;

    // Evaluer nos champions
    for (const auto& c : player.getChampionsEnJeu()) {
        Champion* champ = dynamic_cast<Champion*>(c.get());
        if (champ) {
            control += champ->getPv() * 2.0f;
            if (champ->getEstGarde()) {
                control += 15.0f; // Grande valeur pour les gardes
            }
            // Bonus pour les champions non-activés (peuvent encore agir)
            if (!champ->getEstActiver()) {
                control += 5.0f;
            }
        }
    }

    // Évaluer les champions adverses (négativement)
    for (const auto& c : opponent.getChampionsEnJeu()) {
        const Champion* champ = dynamic_cast<const Champion*>(c.get());
        if (champ) {
            control -= champ->getPv() * 2.0f;
            if (champ->getEstGarde()) {
                control -= 15.0f;
            }
        }
    }

    return control;
}

float GameEvaluator::evaluateSynergy(Player& player) {
    float synergy = 0.0f;

    // Compter les cartes par faction dans la main et en jeu
    std::map<Faction, int> factionCounts;
    
    for (const auto& c : player.getMain()) {
        Faction f = c->getFaction();
        if (f != Faction::Aucun) {
            factionCounts[f]++;
        }
    }
    
    for (const auto& c : player.getChampionsEnJeu()) {
        Faction f = c->getFaction();
        if (f != Faction::Aucun) {
            factionCounts[f]++;
        }
    }

    // Bonus pour chaque faction concentrée (plus de 2 cartes)
    for (const auto& pair : factionCounts) {
        if (pair.second >= 2) {
            synergy += (pair.second - 1) * 8.0f; // Bonus croissant
        }
    }

    return synergy;
}

float GameEvaluator::evaluateHandPotential(Player& player) {
    float potential = 0.0f;

    for (const auto& c : player.getMain()) {
        potential += evaluateCardValue(c.get()) * 0.3f;
    }

    // Bonus si main équilibrée (champions + actions)
    int champions = 0, actions = 0, objects = 0;
    for (const auto& c : player.getMain()) {
        switch (c->getType()) {
            case TypeCarte::Champion: champions++; break;
            case TypeCarte::Action: actions++; break;
            case TypeCarte::Objet: objects++; break;
        }
    }

    if (champions > 0 && (actions > 0 || objects > 0)) {
        potential += 10.0f; // Bonus pour diversité
    }

    return potential;
}

float GameEvaluator::evaluateChampionPriority(Player& player, Player& opponent) {
    float priority = 0.0f;

    // Si on a peu de champions en jeu, priorité haute
    int ourChampions = static_cast<int>(player.getChampionsEnJeu().size());
    int theirChampions = static_cast<int>(opponent.getChampionsEnJeu().size());

    if (ourChampions < 2) {
        priority += 15.0f;
    }
    
    if (theirChampions > ourChampions) {
        priority += (theirChampions - ourChampions) * 10.0f;
    }

    // Si adversaire a beaucoup de HP, on a besoin de champions pour damage récurrent
    if (opponent.getHp() > 30) {
        priority += 10.0f;
    }

    return priority;
}

float GameEvaluator::evaluateSacrificeValue(const Carte* carte, Player& player) {
    if (!carte) return 0.0f;

    float sacrificeValue = 0.0f;

    // Cartes de départ sont de bons candidats au sacrifice
    if (isStartingCard(carte)) {
        sacrificeValue += 15.0f;
    }

    // Cartes à faible coût sont de meilleurs sacrifices
    int cost = carte->getCout();
    if (cost <= 2) {
        sacrificeValue += 10.0f - (cost * 3.0f);
    }

    // Champions déjà activés sont de bons candidats
    Champion* champ = dynamic_cast<Champion*>(const_cast<Carte*>(carte));
    if (champ && champ->getEstActiver()) {
        sacrificeValue += 5.0f;
    }

    return sacrificeValue;
}

bool GameEvaluator::isStartingCard(const Carte* carte) {
    if (!carte) return false;
    
    std::string nom = carte->getNom();
    
    // Vérifier si c'est une des cartes de départ typiques
    return (nom.find("Dague") != std::string::npos || 
            nom.find("Dagger") != std::string::npos ||
            nom.find("Or") != std::string::npos || 
            nom.find("Gold") != std::string::npos ||
            nom.find("Rubis personnel") != std::string::npos ||
            nom.find("Personal Ruby") != std::string::npos ||
            carte->getCout() == 0); // Les cartes gratuites sont souvent des starters
}

int GameEvaluator::countCardsByFaction(
    const std::vector<std::unique_ptr<Carte>>& cards, Faction faction) {
    
    int count = 0;
    for (const auto& c : cards) {
        if (c && c->getFaction() == faction) {
            count++;
        }
    }
    return count;
}

int GameEvaluator::countChampions(const std::vector<std::unique_ptr<Carte>>& cards) {
    int count = 0;
    for (const auto& c : cards) {
        if (c && c->getType() == TypeCarte::Champion) {
            count++;
        }
    }
    return count;
}

int GameEvaluator::getTotalCardValue(const std::vector<std::unique_ptr<Carte>>& cards) {
    int total = 0;
    for (const auto& c : cards) {
        if (c) {
            total += c->getCout();
        }
    }
    return total;
}
