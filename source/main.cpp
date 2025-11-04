#include <iostream>
#include <string>
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/ui/Console.hpp"
#include "../include/ai/HeuristicAI.hpp"

int main() {
    Game game;
    game.demarrerPartie();

    auto& joueurs = game.getPlayers();
    int joueurActif = 0;
    bool quitter = false;
    
    // Demander si on veut jouer contre l'IA
    std::cout << "Hero Realms\n";
    std::cout << "1. Joueur vs Joueur\n";
    std::cout << "2. Joueur vs IA\n";
    int modeJeu = ui::Console::lireChoix("> Mode de jeu", 1, 2);
    
    std::unique_ptr<HeuristicAI> ai = nullptr;
    bool aiVerbose = false;
    
    if (modeJeu == 2) {
        std::cout << "Activer le mode verbose (logs IA) ? (1=Oui, 0=Non): ";
        int verbose = ui::Console::lireChoix("", 0, 1);
        aiVerbose = (verbose == 1);
        
        // Joueur vs IA
        ai = std::make_unique<HeuristicAI>(1, aiVerbose);
        game.setAIPlayer(std::move(ai), 1);
        std::cout << "\n Joueur 1 (Vous) vs IA (Joueur 2) \n";
    }

    // Init tour du joueur 1
    joueurs[joueurActif].resetPourNouveauTour();

    while (!quitter && !game.estTerminee()) {
        // 🔹 Nettoyage clair du terminal avant chaque tour
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

        Player& p   = joueurs[joueurActif];
        Player& adv = joueurs[1 - joueurActif];
        
        // Si c'est le tour de l'IA, la laisser jouer
        if (game.isAIPlayer(joueurActif)) {
            std::cout << "\n Tour de l'IA (Joueur " << (joueurActif + 1) << ") \n";
            ui::Console::afficherPlateau(game, p, adv, false);
            
            HeuristicAI* ai = game.getAIPlayer();
            if (ai) {
                ai->playTour(game, p);
            }
            
            // Terminer le tour de l'IA
            joueurActif = 1 - joueurActif;
            joueurs[joueurActif].resetPourNouveauTour();
            continue;
        }

        // 🔹 En-tête + menu pour joueur humain
        ui::Console::afficherHeader(game, p, adv);
        ui::Console::afficherMenu(game);

        int choix = ui::Console::lireChoix("> Choix", 0, 9);  // 0=Quitter visible
        switch (choix) {
            case 0: // Quitter
                quitter = true; 
                break;

            case 1: // Jouer
                ui::Console::jouerUneCarte(p, game);
                break;

            case 2: // Activer champion
                ui::Console::activerUnChampion(p, game);
                break;

            case 3: // Voir sacrifices
                ui::Console::voirSacrifices(p, game);
                break;

            case 4: // Acheter
                ui::Console::acheterAuMarche(p, game);
                break;

            case 5: // Attaquer
                ui::Console::attaquer(p, adv, game);
                break;

            case 6: { // ➡️ Terminer le tour → bascule immédiate + redraw pour le suivant
                joueurActif = 1 - joueurActif;
                joueurs[joueurActif].resetPourNouveauTour();
                continue; // repart du début → clear + header du nouveau joueur
            }

            case 7: // 📋 Voir le plateau (affiche tout + pause)
                ui::Console::afficherPlateau(game, p, adv, /*pauseApres=*/true);
                break;

            case 8: // 🔮 God Mode
                game.toggleGodMode();
                break;

            case 9: // 🗃️  Voir la défausse
                ui::Console::voirDefausse(p, game);
                break;
        }
    }

    std::cout << "\nPartie terminée !\n";
    game.afficherGagnant();
    return 0;
}
