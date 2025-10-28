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
        std::vector<std::unique_ptr<Carte>> ecartees;

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

};


#endif