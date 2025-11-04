#ifndef GAMEEVALUATOR_HPP
#define GAMEEVALUATOR_HPP

#include "../Player.hpp"
#include "../Game.hpp"
#include "../Carte.hpp"
#include "../Champion.hpp"
#include "../Enums.h"
#include <vector>
#include <memory>

/**
 * Classe responsable de l'évaluation de l'état du jeu pour l'IA.
 * Fournit des heuristiques pour évaluer la qualité d'une position.
 */
class GameEvaluator {
public:
    // Structure détaillée pour l'évaluation du jeu
    struct Evaluation {
        float totalScore = 0.0f;
        float hpScore = 0.0f;
        float boardScore = 0.0f;
        float handScore = 0.0f;
        float deckQualityScore = 0.0f;
        float resourceScore = 0.0f;
        float tempoScore = 0.0f;
        
        std::string toDebugString() const;
    };

    // Évalue l'état actuel du jeu pour un joueur donné et retourne une structure d'évaluation détaillée.
    static Evaluation evaluateGameState(Game& game, Player& player, Player& opponent);

    // Évalue la valeur intrinsèque d'une carte (sans contexte) et retourne un score.
    static float evaluateCardValue(const Carte* carte);

    // Évalue la valeur d'une carte dans le contexte actuel du jeu et retourne un score.
    static float evaluateCardInContext(const Carte* carte, Player& player, Game& game);

    // Évalue la qualité globale du deck d'un joueur avec divers critères et retourne un score.
    static float evaluateDeckQuality(Player& player);

    // Évalue le contrôle du board entre deux joueurs et retourne un score.
    static float evaluateBoardControl(Player& player, Player& opponent);

    // Évalue la synergie des cartes en jeu et en main.
    static float evaluateSynergy(Player& player);

    // Évalue le potentiel de la main actuelle du joueur.
    static float evaluateHandPotential(Player& player, Game& game);

    // Évalue la priorité de recruter des champions pour un joueur.
    static float evaluateChampionPriority(Player& player, Player& opponent);

    // Évalue la valeur d'un sacrifice de carte.
    static float evaluateSacrificeValue(const Carte* carte, Player& player);

    // Fonctions utilitaires pour l'analyse du jeu (utilisées par HeuristicAI et MCTS)
    
    // Compte le nombre de cartes d'un type spécifique.
    static int countCardsByType(const std::vector<std::unique_ptr<Carte>>& cards, TypeCarte type);
    
    // Compte le nombre de cartes d'une faction spécifique.
    static int countCardsByFaction(const std::vector<std::unique_ptr<Carte>>& cards, Faction faction);
    
    // Compte le nombre de champions dans une liste de cartes.
    static int countChampions(const std::vector<std::unique_ptr<Carte>>& cards);
    
    // Calcule la valeur totale des cartes dans une liste.
    static int getTotalCardValue(const std::vector<std::unique_ptr<Carte>>& cards);
    
    // Détermine si le joueur a une faction dominante (plus de 50% des cartes).
    static bool hasFactionDominante(Player& player, Faction faction);
    
    // Calcule le potentiel de dégâts restants du joueur (combat en main + champions non activés).
    static int calculateRemainingDamagePotential(Game& game, Player& player);

private:
    // Poids des différents facteurs pour l'évaluation globale
    static constexpr float HP_WEIGHT = 2.0f;
    static constexpr float BOARD_WEIGHT = 1.5f;
    static constexpr float HAND_WEIGHT = 1.0f;
    static constexpr float DECK_QUALITY_WEIGHT = 0.5f;
    static constexpr float RESOURCE_WEIGHT = 0.8f;
    static constexpr float TEMPO_WEIGHT = 1.2f;

    // Valeurs de base pour différents types de cartes
    static constexpr float CHAMPION_BASE_VALUE = 15.0f;
    static constexpr float ACTION_BASE_VALUE = 8.0f;
    static constexpr float OBJECT_BASE_VALUE = 5.0f;
    
    // Valeurs pour les cartes de départ (moins bonnes)
    static constexpr float STARTING_DAGGER_VALUE = 2.0f;
    static constexpr float STARTING_GOLD_VALUE = 1.0f;
    static constexpr float STARTING_RUBY_VALUE = 3.0f;
    static constexpr float STARTING_SHORT_SWORD_VALUE = 2.5f;

    // Méthodes utilitaires privées
    static bool isStartingCard(const Carte* carte);
};

#endif 
