#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include <memory>
#include "Carte.hpp"

class Player{
    private:
        int id;
        int pv=50;
        int combat=0;
        int gold=0;
        int heal=0;
        std::vector<std::unique_ptr<Carte>> main;
        std::vector<std::unique_ptr<Carte>> championsEnJeu;
        std::vector<std::unique_ptr<Carte>> deck;
        std::vector<std::unique_ptr<Carte>> defausse;
    public:
        Player(int id);
        void piocherCarte(int nb);
        void acheterCarte(int index, Game& game);
        void modiffGold(int nb);
        void modiffCombat(int nb);
        void modiffHeal(int nb);
        void soigner(int nb);
        void attaquer(Player& cible, int nb,Carte* carte=nullptr);
        void subirDegat(int nb);
        void jouerCarte(int index, Game& game);
        void sacrifierCarte(Carte* carte);
        bool estVivant() const;
        int getHp() const;
        int getGold() const;
        int getAtk() const;
        std::vector<std::unique_ptr<Carte>>& getDefausse();
        std::vector<std::unique_ptr<Carte>>& getChampionsEnJeu();
        std::vector<std::unique_ptr<Carte>>& getMain();
        std::vector<std::unique_ptr<Carte>>& getDeck();
        void afficherMain() const;
        void afficherStats() const;
};


#endif