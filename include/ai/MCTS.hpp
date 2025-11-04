#ifndef MCTS_HPP
#define MCTS_HPP

#include "../Game.hpp"
#include "../Player.hpp"
#include <memory>
#include <vector>
#include <random>

// Forward declarations


// Noeud de l'arbre MCTS
struct MCTSNode {
    int action;           // Index de l'action (-1 pour racine)
    int visits;
    float totalReward;
    MCTSNode* parent;
    std::vector<std::unique_ptr<MCTSNode>> children;
    std::vector<int> untriedActions;  // Actions pas encore explorées
    
    MCTSNode(int action = -1, MCTSNode* parent = nullptr, const std::vector<int>& validActions = {})
        : action(action), visits(0), totalReward(0.0f), parent(parent), untriedActions(validActions) {}
    
    float getUCB1(float explorationParam = 1.41f) const;
    bool isFullyExpanded() const;
    MCTSNode* selectChild() const;
    MCTSNode* addChild(int action);
};

class MCTS {
public:
    MCTS(int playerId, int iterations = 100, bool verbose = false);
    
    // Jouer un tour complet avec MCTS
    void playTour(Game& game, Player& player);
    
private:
    int playerId;
    int iterations;  // Nombre de simulations MCTS
    bool verbose;
    std::mt19937 rng;
    
    // Les 4 phases classiques de MCTS
    MCTSNode* selection(MCTSNode* root);
    MCTSNode* expansion(MCTSNode* node);
    float simulation(Game& game, Player& player);
    void backpropagation(MCTSNode* node, float reward);
    
    // Choisir la meilleure action via arbre MCTS
    int searchBestAction(Game& game, Player& player, const std::vector<int>& validActions);
    
    // Phases du tour
    void playCards(Game& game, Player& player);
    void activateChampions(Game& game, Player& player);
    void buyCards(Game& game, Player& player);
    void attack(Game& game, Player& player);
};

#endif
