#ifndef GAME_HPP
#define GAME_HPP
#include "Player.hpp"
#include <vector>
#include <memory>

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
    public:
        Game();
        std::vector<Player>& getPlayers();
        void demarrerPartie();
        void tourDeJeu(Player& joueur);
        void afficherMarche() const;
        Carte* acheterCarte(int index, Player& acheteur);
        void ajouterCarteMarche(std::unique_ptr<Carte> carte);
        void melangerPioche();
        void afficherEtatJoueurs() const;
        bool estTerminee() const;
        void afficherGagnant() const;
        // Activation context helpers
        void setCarteEnActivation(Carte* c);
        Carte* getCarteEnActivation() const;

};


#endif