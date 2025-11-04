#include <iostream>
#include <string>
#include <limits>
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/ui/Console.hpp"
#include "../include/ai/HeuristicAI.hpp"
#include "../include/ai/MCTS.hpp"

int main() {
    Game game;
    game.demarrerPartie();

    auto& joueurs = game.getPlayers();
    int joueurActif = 0;
    bool quitter = false;
    
    // Demander si on veut jouer contre l'IA
    std::cout << "Hero Realms\n";
    std::cout << "1. Joueur vs Joueur\n";
    std::cout << "2. Joueur vs IA Heuristique\n";
    std::cout << "3. Joueur vs IA MCTS\n";
    int modeJeu = ui::Console::lireChoix("> Mode de jeu", 1, 3);
    
    std::unique_ptr<HeuristicAI> heuristicAI = nullptr;
    std::unique_ptr<MCTS> mctsAI = nullptr;
    bool aiVerbose = false;
    int aiType = 0; // 0=none, 1=heuristic, 2=mcts
    
    if (modeJeu == 2) {
        std::cout << "Activer le mode verbose (logs IA) ? (1=Oui, 0=Non): ";
        int verbose = ui::Console::lireChoix("", 0, 1);
        aiVerbose = (verbose == 1);
        
        // Joueur vs IA Heuristique
        heuristicAI = std::make_unique<HeuristicAI>(1, aiVerbose);
        game.setAIPlayer(std::move(heuristicAI), 1);
        aiType = 1;
        std::cout << "\n Joueur 1 (Vous) vs IA Heuristique (Joueur 2) \n";
    } else if (modeJeu == 3) {
        std::cout << "Activer le mode verbose (logs IA) ? (1=Oui, 0=Non): ";
        int verbose = ui::Console::lireChoix("", 0, 1);
        aiVerbose = (verbose == 1);
        
        std::cout << "Nombre d'iterations MCTS (recommande: 50-200): ";
        int iterations = ui::Console::lireChoix("", 10, 1000);
        
        // Joueur vs IA MCTS
        mctsAI = std::make_unique<MCTS>(1, iterations, aiVerbose);
        aiType = 2;
        std::cout << "\n Joueur 1 (Vous) vs IA MCTS (Joueur 2) \n";
    }

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
        if ((aiType > 0 && joueurActif == 1) || game.isAIPlayer(joueurActif)) {
            std::cout << "\n Tour de l'IA (Joueur " << (joueurActif + 1) << ") \n";
            ui::Console::afficherPlateau(game, p, adv, false);
            
            if (aiType == 1) {
                // IA Heuristique
                HeuristicAI* ai = game.getAIPlayer();
                if (ai) {
                    ai->playTour(game, p);
                }
            } else if (aiType == 2) {
                // IA MCTS
                if (mctsAI) {
                    mctsAI->playTour(game, p);
                }
            }
            
            // Afficher l'état après le tour de l'IA
            std::cout << "\n Fin du tour de l'IA \n";
            std::cout << "Appuyez sur Entree pour continuer...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
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
