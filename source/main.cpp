#include <iostream>
#include <string>
#include <cctype>

#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/ui/CardRenderer.hpp"
#include "../include/Champion.hpp"

using namespace std;

// NOTE: resource aggregation moved to manual play: players must play cards
// one-by-one to gain gold/combat (previous automatic aggregation removed).

// affiche le petit menu en bas
static void afficherMenu(const Game& game) {
    cout << "\nCommandes : "
         << "E(etat)  H(main)  M(marche)  "
         << "C(champions)  AC(activer champion)  CA(champ adv)  "
         << "S(sacrifiées)  A(acheter)  P(jouer)  "
        << "N(passer)  T(attaquer)  G(god)  Q(quitter)";
    if (game.isGodMode()) cout << "  [GodMode ACTIF]";
    cout << "\n";
}

static void afficherMainDetaillee(Player& joueur) {
    auto& main   = joueur.getMain();
    auto& champs = joueur.getChampionsEnJeu();

    ui::CardRenderer::Options opts;
    opts.width = 60;

    if (!main.empty()) {
        cout << "Votre main actuelle :\n";
        for (size_t i = 0; i < main.size(); ++i) {
            cout << "\n(" << i + 1 << ")\n";
            cout << ui::CardRenderer::render(*main[i], opts);
        }
    } else {
        cout << "Votre main est vide.\n";
    }

    if (!champs.empty()) {
        cout << "\nVos champions en jeu :\n";
        for (size_t i = 0; i < champs.size(); ++i) {
            cout << "\n[" << i + 1 << "]\n";
            cout << ui::CardRenderer::render(*champs[i], opts);
        }
    }
}

int main() {
    Game game;
    game.demarrerPartie();

    auto& joueurs = game.getPlayers();
    int joueurActif = 0;
    bool quitter = false;

    // initialize first player's turn state
    joueurs[joueurActif].resetPourNouveauTour();

    cout << "=== HERO REALMS - Console ===\n";
    afficherMenu(game);

    while (!quitter && !game.estTerminee()) {
        Player& p   = joueurs[joueurActif];
        Player& adv = joueurs[1 - joueurActif];

    // Resources are now managed by playing cards one-by-one.
    // The player's turn state (gold/combat/etc.) is reset only when the
    // turn starts; do not reset here to allow multiple actions in a turn.

        // 3) affichage
        cout << "\nJoueur " << (p.getId() + 1)
             << " | PV: " << p.getHp()
             << " | Or: " << p.getGold()
             << " | Combat: " << p.getAtk()
             << "\nAdversaire (J" << (adv.getId() + 1)
             << ") | PV: " << adv.getHp()
             << "\n> ";

        string cmd;
        getline(cin, cmd);
        if (cmd.empty()) continue;
        for (auto& c : cmd) c = toupper((unsigned char)c);

        if (cmd == "Q") {
            quitter = true;
        }
        else if (cmd == "E") {
            game.afficherEtatJoueurs();
        }
        else if (cmd == "H") {
            afficherMainDetaillee(p);
        }
        else if (cmd == "M") {
            game.afficherMarche();
        }
        else if (cmd == "C") {
            p.afficherChampionsEnJeu();
        }
        else if (cmd == "CA") {
            adv.afficherChampionsEnJeu();
        }
        else if (cmd == "S") {
            p.afficherSacrifices();
        }
        else if (cmd == "A") {
            game.afficherMarche();

            const Carte* gemme = game.getModeleGemmeDeFeu();
            if (gemme) {
                ui::CardRenderer::Options opts;
                opts.width = 60;
                std::cout << "\n(F) Gemme de Feu :\n";
                std::cout << ui::CardRenderer::render(*gemme, opts);
            }
            // Delegate full interactive purchase flow to Player::acheterCarte
            // which uses getline and respects GodMode-visible market size.
            p.acheterCarte(game);
        }
        else if (cmd == "P") {
            auto& main = p.getMain();
            if (main.empty()) {
                cout << "Vous n'avez aucune carte à jouer.\n";
            } else {
                afficherMainDetaillee(p);
                cout << "Quelle carte jouer ? : ";
                string s;
                getline(cin, s);
                if (!s.empty()) {
                    try {
                        int idx = stoi(s);
                        p.jouerCarte(idx, game);
                    } catch (...) {
                        cout << "Entrée invalide.\n";
                    }
                }
            }
        }
        else if (cmd == "G") {
            game.toggleGodMode();
            if (game.isGodMode())
                cout << "GodMode activé. Achats gratuits + pioche visible. Tapez G encore pour désactiver.\n";
            else
                cout << "GodMode désactivé.\n";
        }
        else if (cmd == "T") {
            // Attack: choose target (player or one of opponent's champions)
            auto& advChamps = adv.getChampionsEnJeu();
            if (advChamps.empty()) {
                cout << "Aucun champion adverse. Attaque directe sur le joueur.\n";
                p.attaquer(adv, nullptr);
            } else {
                cout << "Choisissez cible : (P) Joueur ou numéro du champion adverse :\n";
                for (size_t i = 0; i < advChamps.size(); ++i) {
                    // show index, name, and (if available) hp via dynamic cast
                    std::cout << i + 1 << ") " << advChamps[i]->getNom();
                    Champion* ch = dynamic_cast<Champion*>(advChamps[i].get());
                    if (ch) std::cout << " (PV=" << ch->getPv() << ")";
                    std::cout << "\n";
                }
                cout << "> ";
                string choice;
                getline(cin, choice);
                if (choice.empty()) {
                    cout << "Attaque annulée.\n";
                } else {
                    for (auto &c : choice) c = toupper((unsigned char)c);
                    if (choice == "P") {
                        p.attaquer(adv, nullptr);
                    } else {
                        try {
                            int idx = stoi(choice) - 1;
                            if (idx < 0 || static_cast<size_t>(idx) >= advChamps.size()) {
                                cout << "Index de champion invalide.\n";
                            } else {
                                Champion* cible = dynamic_cast<Champion*>(advChamps[idx].get());
                                if (!cible) {
                                    cout << "La carte choisie n'est pas un champion.\n";
                                } else {
                                    p.attaquer(adv, cible);
                                }
                            }
                        } catch (...) {
                            cout << "Entrée invalide.\n";
                        }
                    }
                }
            }
        }
        else if (cmd == "AC") {
            // Activate one of your champions in play
            auto& champs = p.getChampionsEnJeu();
            if (champs.empty()) {
                cout << "Vous n'avez aucun champion en jeu.\n";
            } else {
                cout << "Choisissez le champion à activer :\n";
                for (size_t i = 0; i < champs.size(); ++i) {
                    Champion* ch = dynamic_cast<Champion*>(champs[i].get());
                    cout << i + 1 << ") " << champs[i]->getNom();
                    if (ch && ch->getEstActiver()) cout << " (déjà activé)";
                    cout << "\n";
                }
                cout << "> ";
                string s;
                getline(cin, s);
                if (!s.empty()) {
                    try {
                        int idx = stoi(s) - 1;
                        if (idx < 0 || static_cast<size_t>(idx) >= champs.size()) {
                            cout << "Index invalide.\n";
                        } else {
                            Champion* cible = dynamic_cast<Champion*>(champs[idx].get());
                            if (!cible) {
                                cout << "Carte sélectionnée n'est pas un champion.\n";
                            } else if (cible->getEstActiver()) {
                                cout << "Ce champion est déjà activé ce tour.\n";
                            } else {
                                cible->activer(p, game);
                            }
                        }
                    } catch (...) {
                        cout << "Entrée invalide.\n";
                    }
                }
            }
        }
        else if (cmd == "N") {
            // end current player's turn and start the other player's turn
            joueurActif = 1 - joueurActif;
            joueurs[joueurActif].resetPourNouveauTour();
            cout << "→ Tour du joueur " << (joueurActif + 1) << "\n";
        }
        else {
            cout << "Commande inconnue.\n";
        }

        afficherMenu(game);
    }

    cout << "\nPartie terminée !\n";
    game.afficherGagnant();
    return 0;
}