#ifndef GAME_HPP
#define GAME_HPP
#include "Player.hpp"
#include <vector>
#include <memory>

class HeuristicAI;

class Game{
    private:
        std::vector<Player> players;
        std::vector<std::unique_ptr<Carte>> marche;
        std::vector<std::unique_ptr<Carte>> pioche;
        std::vector<std::unique_ptr<Carte>> gemmesDeFeu;
        std::vector<std::unique_ptr<Carte>> deckBase1;
        std::vector<std::unique_ptr<Carte>> deckBase2;
        std::vector<std::unique_ptr<Carte>> ecartees;
        // pointant la carte actuellement en cours d'activation
        Carte* carteEnActivation = nullptr;
        bool godMode = false;

        // Support IA
        std::unique_ptr<HeuristicAI> aiPlayer;
        int aiPlayerIndex = -1; // -1 = pas d'IA, 0 ou 1 = index du joueur IA
    public:
        Game();
        std::vector<Player>& getPlayers();
        void demarrerPartie();
        void tourDeJeu(Player& joueur);
        void afficherMarche() const;
        Carte* acheterCarte(int index, Player& acheteur);
        Carte* acheterGemmeDeFeu(Player& acheteur);
        void ajouterCarteMarche(std::unique_ptr<Carte> carte);
        void melangerPioche();
        void initialiserMarche();
        void initialiserDeckBase();
        void initialiserGemmesDeFeu();
        void afficherEtatJoueurs() const;
        bool estTerminee() const;
        void afficherGagnant() const;
        // God mode methodes
        void setGodMode(bool val);
        void toggleGodMode();
        bool isGodMode() const;
    // nombre d'options d'achat visibles (marche + pioche si godMode)
        int getMarketSize() const;
        const std::vector<std::unique_ptr<Carte>>& getMarche() const;
        // Carte en activation
        void setCarteEnActivation(Carte* c);
        Carte* getCarteEnActivation() const;
        const Carte* getModeleGemmeDeFeu() const;

    // Méthodes IA
    void setAIPlayer(std::unique_ptr<HeuristicAI> ai, int playerIndex);
    HeuristicAI* getAIPlayer() const;
    int getAIPlayerIndex() const;
    bool isAIPlayer(int playerIndex) const;
    std::vector<std::unique_ptr<Carte>>& getFireGems();
 
};


#endif