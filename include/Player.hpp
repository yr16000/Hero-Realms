#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include <memory>
#include "Carte.hpp"
#include <iostream>
// forward declaration to avoid circular include
class Game;
class Champion;

class Player{
    private:
        int id;
        int pv=50;
        int combat=0;
        int gold=0;
        int heal=0;
        bool nextAcquiredToTopDeck=false;
        bool nextAcquiredToHand=false;
        bool nextAcquiredActionToTopDeck=false;
        std::vector<std::unique_ptr<Carte>> main;
        std::vector<std::unique_ptr<Carte>> championsEnJeu;
        std::vector<std::unique_ptr<Carte>> deck;
        std::vector<std::unique_ptr<Carte>> defausse;
        std::vector<std::unique_ptr<Carte>> sacrifices;
    public:
        Player(int id);
        void piocherCarte(int nb);
        void acheterCarte(int index, Game& game);
        void modiffGold(int nb);
        void modiffCombat(int nb);
        void modiffHeal(int nb);
        //void soigner();
        void soigner(int nb);
        void attaquer(Player& cible, Champion* carte=nullptr);
        void subirDegat(int nb);
        void jouerCarte(int index, Game& game);
        void sacrifierCarte(Carte* carte, Game& game);
        bool estVivant() const;
        int getId() const;
        int getHp() const;
        int getGold() const;
        int getAtk() const;
        bool getNextAcquiredToTopDeck() const;
        void setNextAcquiredToTopDeck(bool val);
        bool getNextAcquiredToHand() const;
        void setNextAcquiredToHand(bool val);
        bool getNextAcquiredActionToTopDeck() const;
        void setNextAcquiredActionToTopDeck(bool val);
        std::vector<std::unique_ptr<Carte>>& getDefausse();
        std::vector<std::unique_ptr<Carte>>& getChampionsEnJeu();
        std::vector<std::unique_ptr<Carte>>& getMain();
        std::vector<std::unique_ptr<Carte>>& getDeck();
        void melangerDeck();
        void melangerDefausse();
        void afficherMain() const;
        void afficherChampionsEnJeu() const;
        void afficherDefausse() const;
        void afficherStats() const;
        void resetPourNouveauTour();
};


#endif