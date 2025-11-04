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
    /**
     * Structure contenant l'évaluation détaillée d'un état de jeu.
     */
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

    /**
     * Évalue l'état du jeu du point de vue d'un joueur.
     * Plus le score est élevé, meilleure est la position.
     * @param game L'état du jeu actuel
     * @param player Le joueur à évaluer
     * @param opponent L'adversaire
     * @return Structure d'évaluation détaillée
     */
    static Evaluation evaluateGameState(Game& game, Player& player, Player& opponent);

    /**
     * Évalue la valeur intrinsèque d'une carte (indépendamment du contexte).
     * @param carte La carte à évaluer
     * @return Score de la carte (généralement entre 0 et 100)
     */
    static float evaluateCardValue(const Carte* carte);

    /**
     * Évalue la valeur d'une carte dans le contexte actuel du jeu.
     * @param carte La carte à évaluer
     * @param player Le joueur qui possède la carte
     * @param game L'état du jeu
     * @return Score contextuel de la carte
     */
    static float evaluateCardInContext(const Carte* carte, Player& player, Game& game);

    /**
     * Évalue la qualité du deck d'un joueur.
     * @param player Le joueur dont on évalue le deck
     * @return Score de qualité du deck
     */
    static float evaluateDeckQuality(Player& player);

    /**
     * Évalue le contrôle du board (champions en jeu).
     * @param player Le joueur à évaluer
     * @param opponent L'adversaire
     * @return Score de contrôle du board (positif = avantage, négatif = désavantage)
     */
    static float evaluateBoardControl(Player& player, Player& opponent);

    /**
     * Évalue la synergie entre les cartes du joueur (factions, combos).
     * @param player Le joueur à évaluer
     * @return Score de synergie
     */
    static float evaluateSynergy(Player& player);

    /**
     * Calcule le potentiel d'une main (combien de ressources peut générer la main).
     * @param player Le joueur
     * @return Score de potentiel
     */
    static float evaluateHandPotential(Player& player, Game& game);

    /**
     * Évalue l'urgence d'acheter des champions vs des actions.
     * @param player Le joueur
     * @param opponent L'adversaire
     * @return Score positif = priorité champions, négatif = priorité actions/objets
     */
    static float evaluateChampionPriority(Player& player, Player& opponent);

    /**
     * Évalue si une carte est bonne à sacrifier dans le contexte actuel.
     * @param carte La carte candidate au sacrifice
     * @param player Le joueur
     * @return Score (plus élevé = meilleur sacrifice)
     */
    static float evaluateSacrificeValue(const Carte* carte, Player& player);

    // Fonctions utilitaires pour l'analyse du jeu (utilisées par HeuristicAI et MCTS)
    
    /**
     * Compte le nombre de cartes d'un type spécifique.
     * @param cards Liste de cartes à analyser
     * @param type Type de carte recherché
     * @return Nombre de cartes du type spécifié
     */
    static int countCardsByType(const std::vector<std::unique_ptr<Carte>>& cards, TypeCarte type);
    
    /**
     * Compte le nombre de cartes d'une faction dans une liste.
     * @param cards Liste de cartes
     * @param faction Faction recherchée
     * @return Nombre de cartes de cette faction
     */
    static int countCardsByFaction(const std::vector<std::unique_ptr<Carte>>& cards, Faction faction);
    
    /**
     * Compte le nombre de champions dans une liste.
     * @param cards Liste de cartes
     * @return Nombre de champions
     */
    static int countChampions(const std::vector<std::unique_ptr<Carte>>& cards);
    
    /**
     * Calcule la valeur totale (coût) des cartes.
     * @param cards Liste de cartes
     * @return Somme des coûts
     */
    static int getTotalCardValue(const std::vector<std::unique_ptr<Carte>>& cards);
    
    /**
     * Vérifie si une faction domine (au moins 3 cartes).
     * @param player Le joueur
     * @param faction La faction à vérifier
     * @return true si domination de faction
     */
    static bool hasFactionDominante(Player& player, Faction faction);
    
    /**
     * Calcule le potentiel de dégâts restant du joueur.
     * @param game État du jeu
     * @param player Le joueur
     * @return Estimation des dégâts potentiels
     */
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

#endif // GAMEEVALUATOR_HPP
