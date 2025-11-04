#ifndef GAME_HPP
#define GAME_HPP
#include "Player.hpp"
#include <vector>
#include <memory>

// Forward declaration
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
        // Pointer to the card currently activating its effects (or nullptr)
        Carte* carteEnActivation = nullptr;
        bool godMode = false;
        
        // AI Support
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
        // God mode controls
        void setGodMode(bool val);
        void toggleGodMode();
        bool isGodMode() const;
    // number of visible purchase options (marche + pioche if godMode)
        int getMarketSize() const;
        const std::vector<std::unique_ptr<Carte>>& getMarche() const;
        // Activation context helpers
        void setCarteEnActivation(Carte* c);
        Carte* getCarteEnActivation() const;
        const Carte* getModeleGemmeDeFeu() const;
    
    // AI Methods
    void setAIPlayer(std::unique_ptr<HeuristicAI> ai, int playerIndex);
    HeuristicAI* getAIPlayer() const;
    int getAIPlayerIndex() const;
    bool isAIPlayer(int playerIndex) const;
 

};


#endif