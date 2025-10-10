#ifndef GAME_HPP
#define GAME_HPP
#include "Player.hpp"

class Game{
    private:
        Player players[2];
        std::vector<std::unique_ptr<Carte>> marche;
        std::vector<std::unique_ptr<Carte>> pioche;
        std::vector<std::unique_ptr<Carte>> ecartées;

    public:
        Game();
        void demarrerPartie();
        void tourDeJeu(Player& joueur);
        void finDePartie();
        void afficherMarche() const;
        Carte* acheterCarte(int index, Player& acheteur);
        void ajouterCarteMarche(std::unique_ptr<Carte> carte);

};


#endif