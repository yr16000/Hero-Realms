#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include <memory>
#include "Carte.hpp"
#include <iostream>
#include <unordered_map>
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
    // counts of cards per faction measured at the start of the turn
    // counts of cards (by faction) that have been played during the current turn
    // this includes Actions and Objects that were played (and moved to defausse)
        std::unordered_map<Faction,int> factionCount;
    public:
        Player(int id);
        void piocherCarte(int nb);
        void acheterCarte(Game& game);
        void acheterGemmeDeFeu(Game& game);
        void modiffGold(int nb);
        void modiffCombat(int nb);
        void modiffHeal(int nb);
        //void soigner();
        void soigner(int nb);
        void attaquer(Player& cible, Champion* carte=nullptr);
        void subirDegat(int nb);
        void jouerCarte(int index, Game& game);
        void jouerCarteIA(int index, Game& game, bool activerChampion = true);
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
        void afficherMainDetaillee() const;
        void afficherChampionsEnJeu() const;
        void afficherDefausse() const;
        void afficherSacrifices() const;
        void afficherStats() const;
        void resetPourNouveauTour();
    // faction counts computed at start of turn (hand + champions in play)
    int getFactionCount(Faction f) const;
    void incrFactionCount(Faction f);
};


#endif