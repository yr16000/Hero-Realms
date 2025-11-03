#include "../../include/ai/HeuristicAI.hpp"
#include "../../include/Champion.hpp"
#include "../../include/Action.hpp"
#include "../../include/Objet.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

HeuristicAI::HeuristicAI(int playerId, bool verbose)
    : playerId(playerId), verbose(verbose) {
}

void HeuristicAI::playTurn(Game& game, Player& player) {
    logDecision("=== IA commence son tour ===");
    
    Player& opponent = game.getPlayers()[1 - playerId];
    
    // Phase 1: Jouer les cartes dans l'ordre optimal
    logDecision("Phase 1: Jouer les cartes");
    bool continuePlayingCards = true;
    int safetyCounter = 0;
    const int MAX_ITERATIONS = 20; // Éviter boucles infinies
    
    while (continuePlayingCards && safetyCounter++ < MAX_ITERATIONS) {
        int cardToPlay = decideCardToPlay(game, player);
        
        if (cardToPlay == -1) {
            continuePlayingCards = false;
            logDecision("Plus de cartes à jouer");
        } else {
            auto& main = player.getMain();
            if (cardToPlay >= 1 && static_cast<size_t>(cardToPlay) <= main.size()) {
                std::string cardName = main[cardToPlay - 1]->getNom();
                logDecision("Joue carte: " + cardName);
                player.jouerCarte(cardToPlay, game);
            } else {
                continuePlayingCards = false;
            }
        }
    }
    
    // Phase 2: Activer les champions en jeu
    logDecision("Phase 2: Activer champions");
    safetyCounter = 0;
    bool continueActivating = true;
    
    while (continueActivating && safetyCounter++ < MAX_ITERATIONS) {
        int champToActivate = decideChampionToActivate(game, player);
        
        if (champToActivate == -1) {
            continueActivating = false;
            logDecision("Plus de champions à activer");
        } else {
            auto& champs = player.getChampionsEnJeu();
            if (champToActivate >= 0 && static_cast<size_t>(champToActivate) < champs.size()) {
                Champion* champ = dynamic_cast<Champion*>(champs[champToActivate].get());
                if (champ && !champ->getEstActiver()) {
                    logDecision("Active champion: " + champ->getNom());
                    champ->activer(player, game);
                }
            }
            continueActivating = false; // Pour l'instant, activer un seul champion par itération
        }
    }
    
    // Phase 3: Achats
    logDecision("Phase 3: Achats (Or disponible: " + std::to_string(player.getGold()) + ")");
    safetyCounter = 0;
    bool continueBuying = true;
    
    while (continueBuying && safetyCounter++ < MAX_ITERATIONS) {
        int cardToBuy = decideCardToBuy(game, player);
        
        if (cardToBuy == -2) {
            continueBuying = false;
            logDecision("Fin des achats");
        } else if (cardToBuy == -1) {
            // Acheter gemme de feu
            logDecision("Achète Gemme de Feu");
            game.acheterGemmeDeFeu(player);
        } else {
            // Acheter carte du marché
            logDecision("Achète carte index: " + std::to_string(cardToBuy + 1));
            game.acheterCarte(cardToBuy, player);
        }
    }
    
    // Phase 4: Attaquer
    logDecision("Phase 4: Attaque (Combat disponible: " + std::to_string(player.getAtk()) + ")");
    if (player.getAtk() > 0) {
        int target = decideAttackTarget(game, player, opponent);
        
        if (target == -1) {
            logDecision("Attaque directe sur le joueur");
            player.attaquer(opponent, nullptr);
        } else if (target >= 0) {
            auto& advChamps = opponent.getChampionsEnJeu();
            if (static_cast<size_t>(target) < advChamps.size()) {
                Champion* targetChamp = dynamic_cast<Champion*>(advChamps[target].get());
                if (targetChamp) {
                    logDecision("Attaque champion: " + targetChamp->getNom());
                    player.attaquer(opponent, targetChamp);
                }
            }
        }
    }
    
    logDecision("=== IA termine son tour ===");
}

int HeuristicAI::decideCardToPlay(Game& game, Player& player) {
    auto& main = player.getMain();
    
    if (main.empty()) {
        return -1;
    }
    
    std::vector<int> playOrder = evaluatePlayOrder(game, player);
    
    if (playOrder.empty()) {
        return -1;
    }
    
    // Retourner le premier (meilleur) choix
    return playOrder[0];
}

std::vector<int> HeuristicAI::evaluatePlayOrder(Game& game, Player& player) {
    auto& main = player.getMain();
    std::vector<ActionEvaluation> evaluations;
    
    for (size_t i = 0; i < main.size(); ++i) {
        const Carte* carte = main[i].get();
        float score = scoreCardPlay(carte, game, player);
        
        std::string desc = carte->getNom() + " (score: " + std::to_string(score) + ")";
        evaluations.emplace_back(static_cast<int>(i + 1), score, desc);
    }
    
    // Trier par score décroissant
    std::sort(evaluations.begin(), evaluations.end(), 
              [](const ActionEvaluation& a, const ActionEvaluation& b) {
                  return a.score > b.score;
              });
    
    std::vector<int> playOrder;
    for (const auto& eval : evaluations) {
        if (eval.score > 0.0f) {
            playOrder.push_back(eval.actionIndex);
            if (verbose) {
                logDecision("  Ordre de jeu: " + eval.description);
            }
        }
    }
    
    return playOrder;
}

float HeuristicAI::scoreCardPlay(const Carte* carte, Game& game, Player& player) {
    if (!carte) return 0.0f;
    
    float score = 10.0f; // Score de base pour jouer une carte
    
    // Bonus pour les cartes qui génèrent des ressources
    TypeCarte type = carte->getType();
    
    // Les objets génèrent généralement de l'or
    if (type == TypeCarte::Objet) {
        score += 15.0f; // Priorité haute pour générer de l'or tôt
    }
    
    // Les actions peuvent générer or, combat, pioche
    if (type == TypeCarte::Action) {
        score += 12.0f;
    }
    
    // Les champions restent en jeu - jouer tôt
    if (type == TypeCarte::Champion) {
        score += 20.0f; // Haute priorité pour mettre des champions en jeu
        
        const Champion* champ = dynamic_cast<const Champion*>(carte);
        if (champ && champ->getEstGarde()) {
            score += 5.0f; // Bonus pour les gardes
        }
    }
    
    // Bonus pour les cartes avec effets de faction si on a déjà cette faction
    Faction faction = carte->getFaction();
    if (faction != Faction::Aucun) {
        int factionCount = player.getFactionCount(faction);
        if (factionCount > 0) {
            score += factionCount * 8.0f; // Fort bonus pour synergie
            
            if (!carte->getEffetsFaction().empty()) {
                score += 10.0f; // Bonus si la carte a des effets de faction
            }
        }
    }
    
    // Pénalité si on a déjà beaucoup de combat et pas besoin de plus
    Player& opponent = game.getPlayers()[1 - playerId];
    if (player.getAtk() > opponent.getHp()) {
        // On a déjà assez de combat pour tuer l'adversaire
        if (type == TypeCarte::Champion) {
            score -= 5.0f; // Moins urgent de jouer des champions
        }
    }
    
    return score;
}

int HeuristicAI::decideCardToBuy(Game& game, Player& player) {
    int gold = player.getGold();
    
    if (gold == 0) {
        return -2; // Pas d'or, ne rien acheter
    }
    
    auto purchaseOptions = evaluatePurchaseOptions(game, player);
    
    if (purchaseOptions.empty()) {
        return -2; // Pas d'options d'achat
    }
    
    // Meilleure option
    ActionEvaluation best = purchaseOptions[0];
    
    if (!shouldPurchaseNow(game, player, best)) {
        return -2; // Attendre un meilleur achat
    }
    
    if (verbose) {
        logDecision("Meilleur achat: " + best.description);
    }
    
    return best.actionIndex;
}

std::vector<HeuristicAI::ActionEvaluation> HeuristicAI::evaluatePurchaseOptions(
    Game& game, Player& player) {
    
    std::vector<ActionEvaluation> options;
    int gold = player.getGold();
    Player& opponent = game.getPlayers()[1 - playerId];
    
    PurchaseStrategy strategy = determinePurchaseStrategy(game, player, opponent);
    
    // Évaluer les cartes du marché
    // Note: On ne peut pas accéder directement au marché depuis ici
    // On va utiliser getVisibleMarketSize et supposer que les indices sont valides
    int marketSize = game.getVisibleMarketSize();
    
    // Pour l'instant, on va simplement favoriser les achats de cartes chères
    // Dans une version plus avancée, on accéderait aux cartes réelles du marché
    
    // Gemme de feu (coût 2)
    if (gold >= 2) {
        float score = 15.0f; // Score de base pour gemme de feu
        
        if (strategy == PurchaseStrategy::ECONOMIC) {
            score += 10.0f; // Les gemmes sont bonnes pour l'économie
        }
        
        options.emplace_back(-1, score, "Gemme de Feu");
    }
    
    // Pour les cartes du marché, on va utiliser une heuristique basée sur le coût disponible
    // Idéalement, on voudrait pouvoir accéder aux cartes réelles
    // Pour l'instant, simulons quelques options basées sur l'or disponible
    
    if (gold >= 8) {
        float score = 50.0f;
        if (strategy == PurchaseStrategy::AGGRESSIVE) score += 15.0f;
        if (strategy == PurchaseStrategy::CONTROL) score += 10.0f;
        options.emplace_back(0, score, "Carte chère (estimée 8+)");
    }
    
    if (gold >= 5) {
        float score = 35.0f;
        if (strategy == PurchaseStrategy::BALANCED) score += 10.0f;
        options.emplace_back(0, score, "Carte moyenne (estimée 5-7)");
    }
    
    if (gold >= 3) {
        float score = 20.0f;
        if (strategy == PurchaseStrategy::ECONOMIC) score += 5.0f;
        options.emplace_back(0, score, "Carte bon marché (estimée 3-4)");
    }
    
    // Trier par score décroissant
    std::sort(options.begin(), options.end(),
              [](const ActionEvaluation& a, const ActionEvaluation& b) {
                  return a.score > b.score;
              });
    
    return options;
}

bool HeuristicAI::shouldPurchaseNow(Game& game, Player& player, const ActionEvaluation& bestPurchase) {
    // Toujours acheter si on a trouvé une bonne option
    // Dans une version plus avancée, on pourrait attendre de meilleures cartes
    return bestPurchase.score > 10.0f;
}

HeuristicAI::PurchaseStrategy HeuristicAI::determinePurchaseStrategy(
    Game& game, Player& player, Player& opponent) {
    
    int hpDiff = player.getHp() - opponent.getHp();
    int ourChampions = static_cast<int>(player.getChampionsEnJeu().size());
    int theirChampions = static_cast<int>(opponent.getChampionsEnJeu().size());
    
    // Si on est en danger (faible HP), jouer défensif
    if (player.getHp() < 20 || hpDiff < -10) {
        return PurchaseStrategy::CONTROL;
    }
    
    // Si on a un gros avantage, être agressif
    if (hpDiff > 15 && ourChampions > theirChampions) {
        return PurchaseStrategy::AGGRESSIVE;
    }
    
    // Si on manque de champions, prioriser le contrôle
    if (ourChampions < 2) {
        return PurchaseStrategy::CONTROL;
    }
    
    // Si début de partie, focus économie
    int totalCards = static_cast<int>(player.getDeck().size() + 
                                      player.getDefausse().size() + 
                                      player.getMain().size());
    if (totalCards < 15) {
        return PurchaseStrategy::ECONOMIC;
    }
    
    return PurchaseStrategy::BALANCED;
}

int HeuristicAI::decideAttackTarget(Game& game, Player& player, Player& opponent) {
    auto& advChamps = opponent.getChampionsEnJeu();
    
    if (player.getAtk() == 0) {
        return -2; // Pas de combat, ne pas attaquer
    }
    
    // Vérifier s'il y a des gardes
    int guardeIndex = -1;
    for (size_t i = 0; i < advChamps.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
        if (champ && champ->getEstGarde()) {
            guardeIndex = static_cast<int>(i);
            break;
        }
    }
    
    // Si garde présent, on DOIT l'attaquer
    if (guardeIndex != -1) {
        Champion* garde = dynamic_cast<Champion*>(advChamps[guardeIndex].get());
        if (garde && player.getAtk() >= garde->getPv()) {
            logDecision("Attaque le garde obligatoire");
            return guardeIndex;
        } else {
            logDecision("Pas assez de combat pour tuer le garde, attaque quand même");
            return guardeIndex;
        }
    }
    
    // Pas de garde : choisir entre champions et joueur
    
    // Si on peut tuer l'adversaire, le faire !
    if (player.getAtk() >= opponent.getHp()) {
        logDecision("Peut tuer l'adversaire - attaque directe");
        return -1; // Attaque directe
    }
    
    // Sinon, évaluer s'il vaut mieux tuer un champion ou frapper le joueur
    for (size_t i = 0; i < advChamps.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
        if (champ && player.getAtk() >= champ->getPv()) {
            // On peut tuer ce champion
            logDecision("Peut tuer un champion adverse");
            return static_cast<int>(i);
        }
    }
    
    // Sinon, attaque directe sur le joueur
    logDecision("Attaque directe sur le joueur");
    return -1;
}

int HeuristicAI::decideChampionToActivate(Game& game, Player& player) {
    auto evaluations = evaluateChampionActivations(game, player);
    
    if (evaluations.empty()) {
        return -1;
    }
    
    // Retourner le meilleur champion à activer
    return evaluations[0].actionIndex;
}

std::vector<HeuristicAI::ActionEvaluation> HeuristicAI::evaluateChampionActivations(
    Game& game, Player& player) {
    
    std::vector<ActionEvaluation> evaluations;
    auto& champs = player.getChampionsEnJeu();
    
    for (size_t i = 0; i < champs.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(champs[i].get());
        if (champ && !champ->getEstActiver()) {
            float score = 30.0f; // Score de base
            
            // Bonus pour les cartes avec effets de faction
            Faction faction = champ->getFaction();
            if (faction != Faction::Aucun) {
                int factionCount = player.getFactionCount(faction);
                if (factionCount > 1) {
                    score += factionCount * 10.0f;
                }
            }
            
            // Bonus si le champion a des effets de faction
            if (!champ->getEffetsFaction().empty()) {
                score += 15.0f;
            }
            
            std::string desc = champ->getNom() + " (score: " + std::to_string(score) + ")";
            evaluations.emplace_back(static_cast<int>(i), score, desc);
        }
    }
    
    // Trier par score décroissant
    std::sort(evaluations.begin(), evaluations.end(),
              [](const ActionEvaluation& a, const ActionEvaluation& b) {
                  return a.score > b.score;
              });
    
    return evaluations;
}

Carte* HeuristicAI::decideCardToSacrifice(Game& game, Player& player) {
    // Pour l'instant, ne pas sacrifier automatiquement
    // Cette logique sera ajoutée quand les effets de sacrifice seront mieux gérés
    return nullptr;
}

void HeuristicAI::logDecision(const std::string& decision) {
    if (verbose) {
        std::cout << "[IA] " << decision << std::endl;
    }
}

bool HeuristicAI::shouldSaveResources(Game& game, Player& player) {
    // Pour l'instant, toujours utiliser toutes les ressources
    return false;
}

int HeuristicAI::countCardsInHand(const Player& player, TypeCarte type) {
    int count = 0;
    for (const auto& carte : player.getMain()) {
        if (carte->getType() == type) {
            count++;
        }
    }
    return count;
}

bool HeuristicAI::hasFactionDominance(const Player& player, Faction faction) {
    int factionCount = player.getFactionCount(faction);
    return factionCount >= 3;
}

int HeuristicAI::calculateRemainingDamagePotential(Game& game, Player& player) {
    // Estimer le potentiel de dégâts restant
    int potential = player.getAtk();
    
    // Ajouter les champions non activés
    for (const auto& c : player.getChampionsEnJeu()) {
        Champion* champ = dynamic_cast<Champion*>(c.get());
        if (champ && !champ->getEstActiver()) {
            potential += 3; // Estimation conservative
        }
    }
    
    return potential;
}

float HeuristicAI::estimateCardPlayOutcome(const Carte* carte, Game& game, Player& player) {
    // Simplification : retourner la valeur de la carte
    return GameEvaluator::evaluateCardValue(carte);
}

bool HeuristicAI::shouldPrioritizeAttack(Game& game, Player& player, Player& opponent) {
    // Prioriser l'attaque si on peut tuer l'adversaire
    return player.getAtk() >= opponent.getHp();
}

float HeuristicAI::evaluateOpponentThreat(const Player& opponent) {
    float threat = 0.0f;
    
    // Plus l'adversaire a de champions, plus il est menaçant
    threat += static_cast<float>(opponent.getChampionsEnJeu().size()) * 10.0f;
    
    // Plus l'adversaire a de HP, plus il est menaçant à long terme
    threat += static_cast<float>(opponent.getHp()) * 0.5f;
    
    return threat;
}
