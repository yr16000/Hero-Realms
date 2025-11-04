#include "../../include/ai/HeuristicAI.hpp"
#include "../../include/Champion.hpp"
#include "../../include/Action.hpp"
#include "../../include/Objet.hpp"
#include "../../include/effets/Effets.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

HeuristicAI::HeuristicAI(int playerId, bool verbose)
    : playerId(playerId), verbose(verbose) {
}

void HeuristicAI::playTour(Game& game, Player& player) {
    logDecision("IA commence son tour");
    
    Player& opponent = game.getPlayers()[1 - playerId];
    
    // STRATÉGIE: Vérifier si on peut finir l'adversaire ce tour
    int maxCombat = calculateMaxPotentialCombat(game, player);
    bool canFinishOpponent = (maxCombat >= opponent.getHp());
    
    logDecision("Combat max potentiel: " + std::to_string(maxCombat) + 
               ", HP adversaire: " + std::to_string(opponent.getHp()));
    
    if (canFinishOpponent) {
        logDecision("!!! VICTOIRE POSSIBLE CE TOUR (Combat max: " + 
                   std::to_string(maxCombat) + " vs HP adversaire: " + 
                   std::to_string(opponent.getHp()) + ") !!!");
    }
    
    // Phase 1: Jouer les cartes dans l'ordre optimal
    logDecision("Phase 1: Jouer les cartes");
    bool continuePlayingCards = true;
    int safetyCounter = 0;
    const int MAX_ITERATIONS = 20;
    
    while (continuePlayingCards && safetyCounter++ < MAX_ITERATIONS) {
        int cardToPlay = decideCardPlay(game, player);
        
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
            continueActivating = false;
        }
    }
    
    // Phase 3: Achats (seulement si on ne peut pas finir l'adversaire)
    if (canFinishOpponent) {
        logDecision("Phase 3: SKIP ACHATS - Focus sur la victoire !");
    } else {
        logDecision("Phase 3: Achats (Or disponible: " + std::to_string(player.getGold()) + ")");
        safetyCounter = 0;
        bool continueBuying = true;
        
        while (continueBuying && safetyCounter++ < MAX_ITERATIONS) {
            int cardToBuy = decideCardBuy(game, player);
            
            if (cardToBuy == -2) {
                continueBuying = false;
                logDecision("Fin des achats");
            } else if (cardToBuy == -1) {
                logDecision("Achète Gemme de Feu");
                game.acheterGemmeDeFeu(player);
            } else {
                logDecision("Achète carte index: " + std::to_string(cardToBuy));
                game.acheterCarte(cardToBuy, player);
            }
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
    
    logDecision("IA termine son tour");
}

int HeuristicAI::decideCardPlay(Game& game, Player& player) {
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
    
    Player& opponent = game.getPlayers()[1 - playerId];
    
    // Utiliser evaluateCardValue de GameEvaluator comme base
    float score = GameEvaluator::evaluateCardValue(carte);
    
    // Ajuster selon le contexte actuel
    TypeCarte type = carte->getType();
    
    // 1. PRIORITÉ: Vérifier si on peut finir l'adversaire avec combat max potentiel
    int maxCombat = calculateMaxPotentialCombat(game, player);
    bool canFinish = (maxCombat >= opponent.getHp());
    
    if (canFinish) {
        // Favoriser FORTEMENT les cartes qui donnent du combat
        bool hasCombatEffect = false;
        for (const auto& effet : carte->getEffetsCarte()) {
            if (dynamic_cast<EffetGainCombat*>(effet.get()) ||
                dynamic_cast<EffetGainCombatParChampion*>(effet.get()) ||
                dynamic_cast<EffetGainCombatParFaction*>(effet.get())) {
                hasCombatEffect = true;
                score += 100.0f; // ÉNORME bonus pour victoire
                break;
            }
        }
        
        // Pénaliser les cartes sans combat si on peut finir
        if (!hasCombatEffect && type != TypeCarte::Champion) {
            score -= 50.0f;
        }
    }
    
    // 2. Champions: jouer tôt pour avoir leurs effets + présence board
    if (type == TypeCarte::Champion) {
        score += 25.0f;
        const Champion* champ = dynamic_cast<const Champion*>(carte);
        if (champ && champ->getEstGarde()) {
            score += 10.0f;
        }
    }
    
    // 3. Objets: générer de l'or tôt pour achats
    if (type == TypeCarte::Objet) {
        score += 20.0f;
    }
    
    // 4. Actions: polyvalence
    if (type == TypeCarte::Action) {
        score += 15.0f;
    }
    
    // 5. Synergies de faction (IMPORTANT)
    Faction faction = carte->getFaction();
    if (faction != Faction::Aucun) {
        int factionInPlay = GameEvaluator::countCardsByFaction(player.getChampionsEnJeu(), faction);
        int factionInHand = GameEvaluator::countCardsByFaction(player.getMain(), faction);
        
        if (factionInPlay + factionInHand > 1) {
            score += (factionInPlay + factionInHand) * 12.0f;
            
            if (!carte->getEffetsFaction().empty()) {
                score += 20.0f;
            }
        }
    }
    
    // 6. Effets spéciaux prioritaires
    for (const auto& effet : carte->getEffetsCarte()) {
        if (dynamic_cast<EffetPiocherCarte*>(effet.get())) {
            score += 15.0f;
        }
        if (dynamic_cast<EffetAssomerChampion*>(effet.get())) {
            score += 20.0f;
        }
        if (dynamic_cast<EffetDefausserCarteAdversaire*>(effet.get())) {
            score += 12.0f;
        }
        if (dynamic_cast<EffetCarteDefausseTopDeck*>(effet.get())) {
            score += 10.0f;
        }
        if (dynamic_cast<EffetCarteMainDirect*>(effet.get())) {
            score += 10.0f;
        }
        if (dynamic_cast<EffetChampionDefausseTopDeck*>(effet.get())) {
            score += 13.0f;
        }
    }
    
    return score;
}

int HeuristicAI::decideCardBuy(Game& game, Player& player) {
    int gold = player.getGold();
    
    logDecision("decideCardBuy: Or disponible = " + std::to_string(gold));
    
    if (gold == 0) {
        logDecision("decideCardBuy: Pas d'or");
        return -2; // Pas d'or, ne rien acheter
    }
    
    auto purchaseOptions = evaluatePurchaseOptions(game, player);
    
    logDecision("decideCardBuy: " + std::to_string(purchaseOptions.size()) + " options trouvées");
    
    if (purchaseOptions.empty()) {
        logDecision("decideCardBuy: Aucune option d'achat");
        return -2; // Pas d'options d'achat
    }
    
    // Meilleure option
    ActionEvaluation best = purchaseOptions[0];
    
    logDecision("decideCardBuy: Meilleure option = " + best.description);
    
    if (!shouldPurchaseNow(game, player, best)) {
        logDecision("decideCardBuy: shouldPurchaseNow a retourné false");
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
    
    // Évaluer chaque carte du marché avec GameEvaluator
    const auto& marche = game.getMarche();
    
    logDecision("Marché contient " + std::to_string(marche.size()) + " cartes:");
    
    for (size_t i = 0; i < marche.size(); ++i) {
        try {
            const Carte* carte = marche[i].get();
            
            logDecision("  Index " + std::to_string(i) + ": " + 
                       (carte ? carte->getNom() : "nullptr") + 
                       " (cout: " + (carte ? std::to_string(carte->getCout()) : "?") + ")");
            
            if (!carte) {
                logDecision("    -> Carte null, skip");
                continue;
            }
            
            if (carte->getCout() > gold) {
                logDecision("    -> Trop cher (or=" + std::to_string(gold) + ")");
                continue; // Trop cher
            }
            
            logDecision("    -> Evaluation en cours...");
            
            // Utiliser evaluateCardInContext de GameEvaluator
            float score = GameEvaluator::evaluateCardInContext(carte, player, game);
            
            logDecision("    -> Score de base: " + std::to_string(score));
            
            // Bonus si on manque de champions
            if (carte->getType() == TypeCarte::Champion) {
                float championPriority = GameEvaluator::evaluateChampionPriority(player, opponent);
                score += championPriority;
                logDecision("    -> Bonus champion: " + std::to_string(championPriority));
            }
            
            std::string desc = carte->getNom() + " (cout: " + std::to_string(carte->getCout()) + 
                              ", score: " + std::to_string(score) + ", index: " + std::to_string(i) + ")";
            logDecision("    -> Ajouté aux options: " + desc);
            options.emplace_back(static_cast<int>(i), score, desc);
        } catch (const std::exception& e) {
            logDecision("    -> EXCEPTION: " + std::string(e.what()));
        } catch (...) {
            logDecision("    -> EXCEPTION INCONNUE");
        }
    }
    
    logDecision("Évaluation des options de marché terminée");
    
    // Gemme de feu
    if (gold >= 2) {
        logDecision("Évaluation de la Gemme de Feu...");
        
        // Évaluer la gemme selon la qualité actuelle du deck
        float deckQuality = GameEvaluator::evaluateDeckQuality(player);
        
        logDecision("  Qualité du deck: " + std::to_string(deckQuality));
        
        float score = 15.0f;
        
        // Si le deck est faible, gemme est moins importante
        if (deckQuality < 20.0f) {
            score -= 10.0f;
        }
        
        logDecision("  Score gemme: " + std::to_string(score));
        
        options.emplace_back(-1, score, "Gemme de Feu (score: " + std::to_string(score) + ")");
    }
    
    logDecision("Tri des options...");
    
    // Trier par score décroissant
    std::sort(options.begin(), options.end(),
              [](const ActionEvaluation& a, const ActionEvaluation& b) {
                  return a.score > b.score;
              });
    
    logDecision("Options triées, retour de " + std::to_string(options.size()) + " options");
    
    return options;
}

bool HeuristicAI::shouldPurchaseNow(Game& game, Player& player, const ActionEvaluation& bestPurchase) {
    // Toujours acheter si on a trouvé une bonne option
    return bestPurchase.score > 5.0f;
}

int HeuristicAI::decideAttackTarget(Game& game, Player& player, Player& opponent) {
    auto& advChamps = opponent.getChampionsEnJeu();
    
    if (player.getAtk() == 0) {
        return -2; // Pas de combat, ne pas attaquer
    }
    
    // PRIORITÉ 1: Vérifier si on peut FINIR l'adversaire
    if (player.getAtk() >= opponent.getHp()) {
        // Vérifier s'il y a une garde qui bloque
        int guardeIndex = -1;
        for (size_t i = 0; i < advChamps.size(); ++i) {
            Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
            if (champ && champ->getEstGarde()) {
                guardeIndex = static_cast<int>(i);
                break;
            }
        }
        
        if (guardeIndex == -1) {
            // Pas de garde, tuer directement !
            logDecision("VICTOIRE POSSIBLE - Attaque directe pour tuer l'adversaire");
            return -1;
        } else {
            // Il y a une garde, il faut la tuer d'abord
            Champion* garde = dynamic_cast<Champion*>(advChamps[guardeIndex].get());
            if (garde && player.getAtk() >= garde->getPv() + opponent.getHp()) {
                // On peut tuer la garde ET finir l'adversaire
                logDecision("Tue la garde puis finira l'adversaire");
                return guardeIndex;
            } else if (garde && player.getAtk() >= garde->getPv()) {
                // On peut seulement tuer la garde
                logDecision("Tue la garde (garde bloque la victoire)");
                return guardeIndex;
            } else {
                // Pas assez pour tuer la garde, on attaque quand même
                logDecision("Attaque la garde (pas assez pour tuer)");
                return guardeIndex;
            }
        }
    }
    
    // PRIORITÉ 2: Vérifier s'il y a des gardes (obligatoire)
    int guardeIndex = -1;
    for (size_t i = 0; i < advChamps.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
        if (champ && champ->getEstGarde()) {
            guardeIndex = static_cast<int>(i);
            break;
        }
    }
    
    if (guardeIndex != -1) {
        Champion* garde = dynamic_cast<Champion*>(advChamps[guardeIndex].get());
        if (garde) {
            if (player.getAtk() >= garde->getPv()) {
                logDecision("Tue la garde obligatoire");
            } else {
                logDecision("Attaque la garde (pas assez pour tuer)");
            }
            return guardeIndex;
        }
    }
    
    // PRIORITÉ 3: Évaluer le contrôle du board
    float boardControl = GameEvaluator::evaluateBoardControl(player, opponent);
    
    // Si on domine largement le board, attaquer directement
    if (boardControl > 30.0f) {
        logDecision("Domination du board - Attaque directe sur le joueur");
        return -1;
    }
    
    // PRIORITÉ 4: Chercher un champion qu'on peut tuer
    int bestTargetIndex = -1;
    float bestTargetValue = 0.0f;
    
    for (size_t i = 0; i < advChamps.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
        if (champ && player.getAtk() >= champ->getPv()) {
            // On peut tuer ce champion - évaluer sa valeur
            float value = GameEvaluator::evaluateCardValue(champ);
            
            // Bonus si c'est une garde (protège l'adversaire)
            if (champ->getEstGarde()) {
                value += 20.0f;
            }
            
            if (value > bestTargetValue) {
                bestTargetValue = value;
                bestTargetIndex = static_cast<int>(i);
            }
        }
    }
    
    if (bestTargetIndex != -1) {
        logDecision("Tue le champion le plus précieux");
        return bestTargetIndex;
    }
    
    // PRIORITÉ 5: Pas de champion tuable - attaquer directement
    logDecision("Aucun champion tuable - Attaque directe sur le joueur");
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
    Player& opponent = game.getPlayers()[1 - playerId];
    
    for (size_t i = 0; i < champs.size(); ++i) {
        Champion* champ = dynamic_cast<Champion*>(champs[i].get());
        if (champ && !champ->getEstActiver()) {
            // Utiliser evaluateCardValue comme base
            float score = GameEvaluator::evaluateCardValue(champ);
            
            // Bonus pour les effets de faction si activables
            Faction faction = champ->getFaction();
            if (faction != Faction::Aucun && !champ->getEffetsFaction().empty()) {
                int factionCount = GameEvaluator::countCardsByFaction(player.getMain(), faction);
                factionCount += GameEvaluator::countCardsByFaction(player.getChampionsEnJeu(), faction);
                
                if (factionCount >= 2) {
                    score += factionCount * 15.0f; // Fort bonus si synergie
                }
            }
            
            // Analyser les effets pour priorisation
            for (const auto& effet : champ->getEffetsCarte()) {
                // Priorité haute: effets qui peuvent finir l'adversaire
                if (dynamic_cast<EffetGainCombat*>(effet.get())) {
                    if (player.getAtk() + 5 >= opponent.getHp()) { // Estimation
                        score += 30.0f; // Peut permettre de finir
                    }
                }
                // Pioche = très bon
                if (dynamic_cast<EffetPiocherCarte*>(effet.get())) {
                    score += 20.0f;
                }
                // Contrôle adversaire
                if (dynamic_cast<EffetAssomerChampion*>(effet.get())) {
                    if (!opponent.getChampionsEnJeu().empty()) {
                        score += 25.0f; // Très utile si adversaire a des champions
                    }
                }
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

int HeuristicAI::countCardsInHand(Player& player, TypeCarte type) {
    // Utiliser la fonction de GameEvaluator
    return GameEvaluator::countCardsByType(player.getMain(), type);
}

bool HeuristicAI::hasFactionDominance(Player& player, Faction faction) {
    // Utiliser la fonction de GameEvaluator
    return GameEvaluator::hasFactionDominante(player, faction);
}

int HeuristicAI::calculateRemainingDamagePotential(Game& game, Player& player) {
    // Utiliser la fonction de GameEvaluator
    return GameEvaluator::calculateRemainingDamagePotential(game, player);
}

int HeuristicAI::calculateMaxPotentialCombat(Game& game, Player& player) {
    int totalCombat = player.getAtk(); // Combat actuel
    
    // Ajouter le combat potentiel de chaque carte en main
    for (const auto& carte : player.getMain()) {
        // Analyser les effets de la carte
        for (const auto& effet : carte->getEffetsCarte()) {
            if (dynamic_cast<EffetGainCombat*>(effet.get())) {
                totalCombat += 3; // Estimation conservative
            }
            if (dynamic_cast<EffetGainCombatParChampion*>(effet.get())) {
                int champCount = GameEvaluator::countChampions(player.getChampionsEnJeu());
                totalCombat += champCount * 2;
            }
            if (dynamic_cast<EffetGainCombatParFaction*>(effet.get())) {
                Faction faction = carte->getFaction();
                if (faction != Faction::Aucun) {
                    int factionCount = GameEvaluator::countCardsByFaction(player.getMain(), faction);
                    factionCount += GameEvaluator::countCardsByFaction(player.getChampionsEnJeu(), faction);
                    totalCombat += factionCount;
                }
            }
        }
        
        // Ajouter les effets de faction
        for (const auto& effet : carte->getEffetsFaction()) {
            if (dynamic_cast<EffetGainCombat*>(effet.get())) {
                totalCombat += 2;
            }
        }
    }
    
    // Ajouter le combat potentiel des champions non activés
    for (const auto& carte : player.getChampionsEnJeu()) {
        Champion* champ = dynamic_cast<Champion*>(carte.get());
        if (champ && !champ->getEstActiver()) {
            for (const auto& effet : champ->getEffetsCarte()) {
                if (dynamic_cast<EffetGainCombat*>(effet.get())) {
                    totalCombat += 3;
                }
                if (dynamic_cast<EffetGainCombatParChampion*>(effet.get())) {
                    int champCount = GameEvaluator::countChampions(player.getChampionsEnJeu());
                    totalCombat += champCount * 2;
                }
            }
        }
    }
    
    return totalCombat;
}

bool HeuristicAI::shouldActivateOptionalEffect(const Effet* effet, Player& player, Game& game) {
    if (!effet) return false;
    
    Player& opponent = game.getPlayers()[1 - playerId];
    
    // Toujours activer les effets positifs
    if (dynamic_cast<const EffetGainCombat*>(effet)) return true;
    if (dynamic_cast<const EffetGainGold*>(effet)) return true;
    if (dynamic_cast<const EffetGainPV*>(effet)) return true;
    if (dynamic_cast<const EffetPiocherCarte*>(effet)) return true;
    
    // Assommer champion: seulement si adversaire a des champions
    if (dynamic_cast<const EffetAssomerChampion*>(effet)) {
        return !opponent.getChampionsEnJeu().empty();
    }
    
    // Défausser carte adversaire: seulement si adversaire a des cartes en main
    if (dynamic_cast<const EffetDefausserCarteAdversaire*>(effet)) {
        return !opponent.getMain().empty();
    }
    
    // Sacrifice: évaluer si on a de mauvaises cartes
    if (dynamic_cast<const EffetSacrifier*>(effet)) {
        for (const auto& carte : player.getMain()) {
            if (GameEvaluator::evaluateCardValue(carte.get()) < 5.0f) {
                return true;
            }
        }
        return false;
    }
    
    return true;
}

int HeuristicAI::chooseFromEffects(const std::vector<std::unique_ptr<Effet>>& effets, 
                                   Player& player, Game& game) {
    if (effets.empty()) return 0;
    
    Player& opponent = game.getPlayers()[1 - playerId];
    int bestIndex = 0;
    float bestScore = -1000.0f;
    
    for (size_t i = 0; i < effets.size(); ++i) {
        const Effet* effet = effets[i].get();
        float score = 0.0f;
        
        if (dynamic_cast<const EffetGainCombat*>(effet)) {
            score = 15.0f;
            if (player.getAtk() + 5 >= opponent.getHp()) {
                score += 30.0f;
            }
        }
        else if (dynamic_cast<const EffetGainGold*>(effet)) {
            score = 12.0f;
            if (player.getGold() < 8) {
                score += 10.0f;
            }
        }
        else if (dynamic_cast<const EffetGainPV*>(effet)) {
            score = 10.0f;
            if (player.getHp() < 25) {
                score += 15.0f;
            }
        }
        else if (dynamic_cast<const EffetPiocherCarte*>(effet)) {
            score = 20.0f;
        }
        else if (dynamic_cast<const EffetAssomerChampion*>(effet)) {
            score = opponent.getChampionsEnJeu().empty() ? 5.0f : 25.0f;
        }
        else if (dynamic_cast<const EffetDefausserCarteAdversaire*>(effet)) {
            score = opponent.getMain().empty() ? 5.0f : 18.0f;
        }
        else {
            score = 10.0f;
        }
        
        if (score > bestScore) {
            bestScore = score;
            bestIndex = static_cast<int>(i);
        }
    }
    
    logDecision("Choix d'effet: index " + std::to_string(bestIndex) + 
               " (score: " + std::to_string(bestScore) + ")");
    
    return bestIndex;
}

float HeuristicAI::estimateCardPlayOutcome(const Carte* carte, Game& game, Player& player) {
    // Simplification : retourner la valeur de la carte
    return GameEvaluator::evaluateCardValue(carte);
}

bool HeuristicAI::shouldPrioritizeAttack(Game& game, Player& player, Player& opponent) {
    // Prioriser l'attaque si on peut tuer l'adversaire
    return player.getAtk() >= opponent.getHp();
}

float HeuristicAI::evaluateOpponentThreat(Player& opponent) {
    float threat = 0.0f;
    
    // Utiliser evaluateBoardControl pour évaluer la menace du board
    // Plus l'adversaire a de champions, plus il est menaçant
    threat += static_cast<float>(opponent.getChampionsEnJeu().size()) * 10.0f;
    
    // Plus l'adversaire a de HP, plus il est menaçant à long terme
    threat += static_cast<float>(opponent.getHp()) * 0.5f;
    
    return threat;
}
