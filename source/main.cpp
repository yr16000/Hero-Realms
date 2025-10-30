#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/Objet.hpp"
#include "../include/Action.hpp"
#include "../include/effets/EffetGainGold.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

static void initPlayerStarterDecks(Game& game){
    auto& players = game.getPlayers();
    for(auto &p : players){
        // 7 Gold
        for(int i=0;i<7;i++){
            p.getDeck().push_back(std::make_unique<Objet>("Or", 0, 1, 0));
        }
        // 1 Shortsword
        p.getDeck().push_back(std::make_unique<Objet>("Épée courte", 3, 0, 1));
        // 1 Dague
        p.getDeck().push_back(std::make_unique<Objet>("Dague", 2, 0, 1));
        // 1 Ruby
        p.getDeck().push_back(std::make_unique<Objet>("Rubis", 3, 0, 0));
        // shuffle and draw starting hand (5)
        p.melangerDeck();
        p.piocherCarte(5);
    }
}

int main(){
    Game game;

    // Remplir un marche minimal (exemples de cartes de base)
    for(int i=0;i<7;++i){
        game.ajouterCarteMarche(std::make_unique<Objet>("Or", 0, 1, 0));
    }
    game.ajouterCarteMarche(std::make_unique<Objet>("Épée courte", 3, 0, 1));
    game.ajouterCarteMarche(std::make_unique<Objet>("Dague", 2, 0, 1));

    // Afficher le marché minimal
    game.afficherMarche();

    // Initialiser decks des joueurs (7 Or, 1 Épée courte, 1 Dague, 1 Rubis) et piocher main de départ
    initPlayerStarterDecks(game);

    game.demarrerPartie();

    bool godMode = false;
    int current = 0;
    std::cout << "Commandes: m=marche, s=stats, h=main, d=deck, D=defausse, c=champions, p=jouer, b=acheter, n=tour suivant, g=god-mode, q=quitter\n";

    while(true){
        auto& players = game.getPlayers();
        if(players.empty()) break;

        Player& p = players[current];
        std::cout << "\nJoueur courant: " << p.getId() << " (PV: " << p.getHp() << ")\n";
        std::cout << "> ";

        std::string cmd;
        if(!(std::cin >> cmd)) break;

        if(cmd == "q") break;

        if(cmd == "g"){
            godMode = !godMode;
            if(godMode){
                for(auto& pl : game.getPlayers()){
                    int hp = pl.getHp();
                    if(hp > 1) pl.subirDegat(hp - 1);
                }
                std::cout << "God-mode activé : tous les joueurs mis à 1 PV.\n";
            } else {
                std::cout << "God-mode désactivé.\n";
            }
            continue;
        }

        if(cmd == "m"){
            game.afficherMarche();
            continue;
        }

        if(cmd == "s"){
            game.afficherEtatJoueurs();
            continue;
        }

        if(cmd == "h"){
            p.afficherMain();
            continue;
        }

        if(cmd == "d"){
            auto &deck = p.getDeck();
            std::cout << "Deck du joueur " << p.getId() << " (" << deck.size() << " cartes):\n";
            for(size_t i=0;i<deck.size();++i){
                if(deck[i]) std::cout << i+1 << ". " << deck[i]->getNom() << "\n";
            }
            continue;
        }

        if(cmd == "D"){
            p.afficherDefausse();
            continue;
        }

        if(cmd == "c"){
            p.afficherChampionsEnJeu();
            continue;
        }

        if(cmd == "p"){
            std::cout << "Index de la carte dans la main (1-based): ";
            int idx = 0; if(!(std::cin >> idx)) break;
            p.jouerCarte(idx-1, game);
            continue;
        }

        if(cmd == "b"){
            std::cout << "Index de la carte du marche (0-based): ";
            int idx = 0; if(!(std::cin >> idx)) break;
            p.acheterCarte(idx, game);
            continue;
        }

        if(cmd == "n"){
            game.tourDeJeu(p);
            current = (current + 1) % static_cast<int>(game.getPlayers().size());
            continue;
        }

        std::cout << "Commande inconnue.\n";
    }

    std::cout << "Fin de la partie.\n";
    return 0;
}
