#include "../../include/ai/MCTS.hpp"
#include "../../include/ai/GameEvaluator.hpp"
#include "../../include/Champion.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>


float MCTSNode::getUCB1(float explorationParam) const {
    if (visits == 0) return std::numeric_limits<float>::infinity();
    if (!parent) return totalReward / static_cast<float>(visits);
    
    float exploitation = totalReward / static_cast<float>(visits);
    float exploration = explorationParam * std::sqrt(std::log(static_cast<float>(parent->visits)) / static_cast<float>(visits));
    
    return exploitation + exploration;
}

bool MCTSNode::isFullyExpanded() const {
    return untriedActions.empty();
}

MCTSNode* MCTSNode::selectChild() const {
    MCTSNode* best = nullptr;
    float bestValue = -std::numeric_limits<float>::infinity();
    
    for (const auto& child : children) {
        float value = child->getUCB1();
        if (value > bestValue) {
            bestValue = value;
            best = child.get();
        }
    }
    
    return best;
}

MCTSNode* MCTSNode::addChild(int action) {
    // Retirer l'action des actions non essayées
    auto it = std::find(untriedActions.begin(), untriedActions.end(), action);
    if (it != untriedActions.end()) {
        untriedActions.erase(it);
    }
    
    // Créer et ajouter le nouveau noeud enfant
    auto child = std::make_unique<MCTSNode>(action, this);
    MCTSNode* childPtr = child.get();
    children.push_back(std::move(child));
    
    return childPtr;
}


MCTS::MCTS(int playerId, int iterations, bool verbose)
    : playerId(playerId), iterations(iterations), verbose(verbose) {
    std::random_device rd;
    rng.seed(rd());
}

void MCTS::playTour(Game& game, Player& player) {
    Player& opponent = game.getPlayers()[1 - playerId];
    
    if (verbose) {
        std::cout << "  MCTS - Debut du tour" << std::endl;
        std::cout << "Iterations MCTS: " << iterations << std::endl;
        std::cout << std::endl;
    }
    
    if (verbose) {
        std::cout << "PHASE 1: Jouer les cartes de la main" << std::endl;
    }
    playCards(game, player);
    
    if (verbose) {
        std::cout << "PHASE 2: Activer les champions" << std::endl;
    }
    activateChampions(game, player);
    
    // Skip achats si on peut finir l'adversaire
    int combat = player.getAtk();
    bool canFinish = (combat >= opponent.getHp());
    
    if (canFinish) {
        if (verbose) {
            std::cout << "PHASE 3: Achats (SKIP)" << std::endl;
            std::cout << "Objectif victoire detecte (" << combat << " ATK >= " 
                      << opponent.getHp() << " HP adverse)" << std::endl;
        }
    } else {
        if (verbose) {
            std::cout << "PHASE 3: Achats" << std::endl;
            std::cout << "Or disponible: " << player.getGold() << std::endl;
        }
        buyCards(game, player);
    }
    
    if (verbose) {
        std::cout << "PHASE 4: Attaque" << std::endl;
        std::cout << "Degats disponibles: " << player.getAtk() << std::endl;
    }
    attack(game, player);
    
    if (verbose) std::cout << "[MCTS] Fin du tour" << std::endl;
}


// Phase 1: Descendre dans l'arbre via UCB1
MCTSNode* MCTS::selection(MCTSNode* root) {
    MCTSNode* node = root;
    
    // Descendre tant que le noeud est completement exploré et a des enfants
    while (node->isFullyExpanded() && !node->children.empty()) {
        node = node->selectChild();
    }
    
    return node;
}

// Phase 2: Ajouter un nouveau noeud enfant
MCTSNode* MCTS::expansion(MCTSNode* node) {
    if (node->untriedActions.empty()) {
        return node;
    }
    
    // Choisir une action aléatoire parmi celles non essayées
    std::uniform_int_distribution<int> dist(0, node->untriedActions.size() - 1);
    int idx = dist(rng);
    int action = node->untriedActions[idx];
    
    // Créer un nouveau noeud pour cette action
    return node->addChild(action);
}

// Phase 3: SIMULATION
float MCTS::simulation(Game& game, Player& player) {
    Player& opponent = game.getPlayers()[player.getId() == 1 ? 1 : 0];
    
    // Évaluer l'état actuel avec GameEvaluator
    auto evaluation = GameEvaluator::evaluateGameState(game, player, opponent);
    float baseScore = evaluation.totalScore;
    
    // Bonus important si victoire proche
    if (opponent.getHp() <= player.getAtk()) {
        baseScore += 200.0f;
    }
    
    // Pénalité pour les cartes non jouées en main
    // (encourage à jouer toutes les cartes)
    int cardsInHand = player.getMain().size();
    if (cardsInHand > 0) {
        baseScore -= cardsInHand * 7.0f;  // -7 points par carte non jouée
    }
    
    // Bonus pour les ressources disponibles (or, combat)
    baseScore += player.getGold() * 2.0f;   // L'or est précieux
    baseScore += player.getAtk() * 3.0f;    // Le combat aussi
    
    return baseScore;
}

// Phase 4: Remonter les résultats
void MCTS::backpropagation(MCTSNode* node, float reward) {
    while (node != nullptr) {
        node->visits++;
        node->totalReward += reward;
        node = node->parent;
    }
}


int MCTS::searchBestAction(Game& game, Player& player, const std::vector<int>& validActions) {
    if (validActions.empty()) return -1;
    if (validActions.size() == 1) return validActions[0];
    
    // Créer la racine de l'arbre avec toutes les actions valides
    MCTSNode root(-1, nullptr, validActions);
    
    // Effectuer les itérations MCTS
    for (int i = 0; i < iterations; ++i) {
        // Descendre dans l'arbre via UCB1
        MCTSNode* node = selection(&root);

        // Ajouter un nouveau noeud si possible
        if (!node->untriedActions.empty()) {
            node = expansion(node);
        }
        
        // 3. SIMULATION: Évaluer cet état
        float reward = simulation(game, player);
        
        // 4. BACKPROPAGATION: Remonter le résultat
        backpropagation(node, reward);
    }
    
    // Choisir l'action la plus visitée (plus robuste que meilleur score)
    MCTSNode* bestChild = nullptr;
    int mostVisits = -1;
    
    for (const auto& child : root.children) {
        if (child->visits > mostVisits) {
            mostVisits = child->visits;
            bestChild = child.get();
        }
    }
    
    return bestChild ? bestChild->action : validActions[0];
}

void MCTS::playCards(Game& game, Player& player) {
    // Utiliser MCTS pour déterminer l'ordre optimal de jeu des cartes
    int cardNumber = 1;
    
    while (!player.getMain().empty()) {
        auto& main = player.getMain();
        
        // Si une seule carte, la jouer directement
        if (main.size() == 1) {
            if (verbose) {
                std::cout << "\nDerniere carte en main:" << std::endl;
                std::cout << "Carte #" << cardNumber << ": "<<std::endl;
                main[0]->afficherCarte();
            }
            player.jouerCarteIA(1, game, false);
            cardNumber++;
            break;
        }
        
        // Créer la liste des actions valides (indices des cartes, SANS option "stop")
        std::vector<int> validActions;
        for (size_t i = 0; i < main.size(); ++i) {
            validActions.push_back(static_cast<int>(i + 1)); // 1-indexed
        }
        
        if (verbose && !main.empty()) {
            std::cout << "\nCartes en main (" << main.size() << "):" << std::endl;
            for (size_t i = 0; i < main.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << main[i]->getNom() << std::endl;
            }
            std::cout << "\nMCTS recherche la meilleure carte..." << std::flush;
        }
        
        // Utiliser MCTS pour choisir la meilleure carte
        int cardToPlay = searchBestAction(game, player, validActions);
        
        if (cardToPlay >= 1 && cardToPlay <= static_cast<int>(main.size())) {
            if (verbose) {
                std::cout << " OK" << std::endl;
                std::cout << "Carte #" << cardNumber << ": "<<std::endl;
                main[cardToPlay - 1]->afficherCarte();
            }
            
            // Jouer la carte sans activer les champions (Phase 1)
            player.jouerCarteIA(cardToPlay, game, false);
            cardNumber++;
        } else {
            break;
        }
    }
    
    if (verbose && cardNumber == 1) {
        std::cout << "Aucune carte jouee" << std::endl;
    }
}

void MCTS::activateChampions(Game& game, Player& player) {
    auto& champs = player.getChampionsEnJeu();
    int activatedCount = 0;
    
    for (auto& champPtr : champs) {
        Champion* champ = dynamic_cast<Champion*>(champPtr.get());
        if (champ && !champ->getEstActiver()) {
            if (verbose) {
                std::cout << "Activation: " << champ->getNom() << std::endl;
            }
            champ->activer(player, game);
            activatedCount++;
        }
    }
    
    if (verbose && activatedCount == 0) {
        std::cout << "Aucun champion a activer" << std::endl;
    }
}

void MCTS::buyCards(Game& game, Player& player) {
    // Utiliser MCTS pour décider quoi acheter
    int safetyCounter = 0;
    const int MAX_PURCHASES = 10;
    int purchaseNumber = 1;
    
    while (player.getGold() > 0 && safetyCounter++ < MAX_PURCHASES) {
        const auto& marche = game.getMarche();
        std::vector<int> validActions;
        
        // Ajouter les cartes du marché accessibles (1-indexed)
        for (size_t i = 0; i < marche.size(); ++i) {
            if (marche[i] && marche[i]->getCout() <= player.getGold()) {
                validActions.push_back(static_cast<int>(i + 1));
            }
        }
        
        // Ajouter la gemme de feu si accessible
        if (player.getGold() >= 2) {
            validActions.push_back(-1);
        }
        
        // Ajouter l'option "ne rien acheter"
        validActions.push_back(0);
        
        if (validActions.empty()) break;
        
        if (verbose) {
            std::cout << "\nOr restant: " << player.getGold() << std::endl;
            std::cout << "Options d'achat disponibles:" << std::endl;
            for (size_t i = 0; i < marche.size(); ++i) {
                if (marche[i] && marche[i]->getCout() <= player.getGold()) {
                    std::cout << "  - " << marche[i]->getNom() 
                              << " (" << marche[i]->getCout() << " gold)" << std::endl;
                }
            }
            if (player.getGold() >= 2) {
                std::cout << "  - Gemme de Feu (2 gold)" << std::endl;
            }
            std::cout << "\nMCTS recherche le meilleur achat..." << std::flush;
        }
        
        // Utiliser MCTS pour choisir le meilleur achat
        int cardToBuy = searchBestAction(game, player, validActions);
        
        if (cardToBuy == 0) {
            if (verbose) std::cout << " STOP" << std::endl;
            break; // Ne rien acheter
        }
        
        if (cardToBuy == -1) {
            // Acheter Gemme de Feu
            if (verbose) {
                std::cout << " OK" << std::endl;
                std::cout << "Achat #" << purchaseNumber << ": Gemme de Feu" << std::endl;
            }
            game.acheterGemmeDeFeu(player);
            purchaseNumber++;
        } else if (cardToBuy >= 1) {
            // Acheter carte du marché (cardToBuy est 1-indexed)
            int index = cardToBuy - 1;
            if (index >= 0 && index < static_cast<int>(marche.size())) {
                if (marche[index] && player.getGold() >= marche[index]->getCout()) {
                    if (verbose) {
                        std::cout << " OK" << std::endl;
                        std::cout << "Achat " << purchaseNumber << ": " << marche[index]->getNom() << std::endl;
                    }
                    game.acheterCarte(index, player);
                    purchaseNumber++;
                } else {
                    break;
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    if (verbose && purchaseNumber == 1) {
        std::cout << "Aucun achat effectue" << std::endl;
    }
}

void MCTS::attack(Game& game, Player& player) {
    if (player.getAtk() == 0) {
        if (verbose) std::cout << "Aucun degat disponible" << std::endl;
        return;
    }
    
    Player& opponent = game.getPlayers()[playerId == 1 ? 1 : 0];
    auto& advChamps = opponent.getChampionsEnJeu();
    
    std::vector<int> validTargets;
    
    // -1 = attaque directe, 1+ = index du champion (1-indexed)
    validTargets.push_back(-1);
    for (size_t i = 0; i < advChamps.size(); ++i) {
        validTargets.push_back(static_cast<int>(i + 1));
    }
    
    if (verbose) {
        std::cout << "\nCibles disponibles:" << std::endl;
        std::cout << "  - Adversaire directement (" << opponent.getHp() << " HP)" << std::endl;
        for (size_t i = 0; i < advChamps.size(); ++i) {
            Champion* champ = dynamic_cast<Champion*>(advChamps[i].get());
            if (champ) {
                std::cout << "  - " << champ->getNom() 
                          << " (" << champ->getPv() << " PV";
                if (champ->getEstGarde()) std::cout << ", GARDE";
                std::cout << ")" << std::endl;
            }
        }
        std::cout << "\nMCTS choisit la cible..." << std::flush;
    }
    
    // Utiliser MCTS pour choisir la meilleure cible
    int target = searchBestAction(game, player, validTargets);
    
    if (target == -1) {
        // Attaque directe
        if (verbose) {
            std::cout << " OK" << std::endl;
            std::cout << "Cible: Adversaire directement (" << player.getAtk() << " degats)" << std::endl;
        }
        player.attaquer(opponent, nullptr);
    } else if (target >= 1) {
        // Attaquer un champion (target est 1-indexed)
        int index = target - 1;
        if (index >= 0 && index < static_cast<int>(advChamps.size())) {
            Champion* targetChamp = dynamic_cast<Champion*>(advChamps[index].get());
            if (targetChamp) {
                if (verbose) {
                    std::cout << " OK" << std::endl;
                    std::cout << "Cible: " << targetChamp->getNom() 
                              << " (" << player.getAtk() << " degats)" << std::endl;
                }
                player.attaquer(opponent, targetChamp);
            }
        }
    }
}

