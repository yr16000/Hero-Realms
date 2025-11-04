#ifndef HEURISTICAI_HPP
#define HEURISTICAI_HPP

#include "GameEvaluator.hpp"
#include "../Player.hpp"
#include "../Game.hpp"
#include "../Carte.hpp"
#include <string>
#include <vector>
#include <memory>

class Game;
class Player;
class Carte;

/**
 * Implémentation d'une IA basée sur des heuristiques avancées.
 * Cette IA utilise plusieurs stratégies pour prendre des décisions :
 * - Évaluation de l'état du jeu
 * - Priorisation des achats (champions vs actions)
 * - Optimisation de l'ordre de jeu des cartes
 * - Gestion des synergies de faction
 * - Calcul du meilleur moment pour attaquer
 */
class HeuristicAI {
private:
    int playerId;
    bool verbose;
    GameEvaluator evaluator;

    /**
     * Structure pour stocker l'évaluation d'une action possible.
     */
    struct ActionEvaluation {
        int actionIndex;      // Index de l'action (-1 pour actions spéciales)
        float score;          // Score de l'action
        std::string description; // Description pour le debug
        
        ActionEvaluation(int idx, float s, const std::string& desc = "")
            : actionIndex(idx), score(s), description(desc) {}
        
        bool operator<(const ActionEvaluation& other) const {
            return score < other.score;
        }
    };

public:
    HeuristicAI(int playerId, bool verbose = false);
    ~HeuristicAI() = default;
    
    void playTour(Game& game, Player& player);
    int decideCardPlay(Game& game, Player& player);
    int decideCardBuy(Game& game, Player& player);
    int decideAttackTarget(Game& game, Player& player, Player& opponent);
    int decideChampionToActivate(Game& game, Player& player);
    Carte* decideCardToSacrifice(Game& game, Player& player);
    
    std::string getName() const { return "HeuristicAI"; }
    void setVerbose(bool v) { verbose = v; }
    int getPlayerId() const { return playerId; }

private:
    /**
     * Évalue toutes les cartes de la main et retourne l'ordre optimal de jeu.
     * @return Vecteur d'indices triés par priorité de jeu
     */
    std::vector<int> evaluatePlayOrder(Game& game, Player& player);

    /**
     * Évalue toutes les options d'achat disponibles.
     * @return Vecteur d'évaluations d'achats triées
     */
    std::vector<ActionEvaluation> evaluatePurchaseOptions(Game& game, Player& player);

    /**
     * Évalue s'il faut acheter une carte maintenant ou attendre.
     * @param bestPurchase La meilleure option d'achat disponible
     * @return true si on devrait acheter, false si attendre
     */
    bool shouldPurchaseNow(Game& game, Player& player, const ActionEvaluation& bestPurchase);

    /**
     * Évalue chaque champion en jeu pour décider lequel activer.
     * @return Vecteur d'évaluations triées
     */
    std::vector<ActionEvaluation> evaluateChampionActivations(Game& game, Player& player);

    /**
     * Calcule le score d'une carte à jouer dans le contexte actuel.
     * Prend en compte : effets faction, synergies, ressources actuelles, etc.
     */
    float scoreCardPlay(const Carte* carte, Game& game, Player& player);

    /**
     * Calcule le score d'achat d'une carte.
     * Prend en compte : valeur, synergie, stratégie long-terme, etc.
     */
    float scorePurchase(const Carte* carte, Game& game, Player& player);

    /**
     * Évalue si on devrait garder des ressources pour le prochain tour.
     */
    bool shouldSaveResources(Game& game, Player& player);

    /**
     * Compte le nombre de cartes d'un certain type dans la main.
     */
    int countCardsInHand(Player& player, TypeCarte type);

    /**
     * Vérifie si une faction est dominante dans le deck du joueur.
     */
    bool hasFactionDominance(Player& player, Faction faction);

    /**
     * Calcule le potentiel de dégâts restant dans la main.
     */
    int calculateRemainingDamagePotential(Game& game, Player& player);

    /**
     * Calcule le combat maximum possible ce tour (main + champions non activés).
     * @return Combat potentiel maximum
     */
    int calculateMaxPotentialCombat(Game& game, Player& player);

    /**
     * Décide si on doit activer un effet optionnel.
     * @param effet L'effet optionnel
     * @param player Le joueur
     * @param game Le jeu
     * @return true si on active
     */
    bool shouldActivateOptionalEffect(const Effet* effet, Player& player, Game& game);

    /**
     * Choisit le meilleur effet parmi un choix.
     * @param effets Liste des effets possibles
     * @param player Le joueur
     * @param game Le jeu
     * @return Index de l'effet choisi
     */
    int chooseFromEffects(const std::vector<std::unique_ptr<Effet>>& effets, Player& player, Game& game);

    /**
     * Log une décision si le mode verbose est activé.
     */
    void logDecision(const std::string& decision);

    /**
     * Simule mentalement le jeu d'une carte et évalue le résultat.
     * (Simplifié - pas de vraie simulation pour l'instant)
     */
    float estimateCardPlayOutcome(const Carte* carte, Game& game, Player& player);

    /**
     * Détermine s'il faut prioriser l'attaque ou l'achat ce tour.
     */
    bool shouldPrioritizeAttack(Game& game, Player& player, Player& opponent);

    /**
     * Évalue la menace que représente l'adversaire.
     */
    float evaluateOpponentThreat(Player& opponent);
};

#endif // HEURISTICAI_HPP
