#include <iostream>
#include <string>
#include <cctype>

#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/ui/CardRenderer.hpp"

using namespace std;

// calcule gold/combat total à partir de la main + champions
// ⚠️ pas de const ici, parce que Player n'a pas de getters const
static pair<int,int> calculerGoldEtCombat(Player& joueur) {
    int goldPot = 0;
    int combatPot = 0;

    auto& main   = joueur.getMain();
    auto& champs = joueur.getChampionsEnJeu();

    for (auto& c : main) {
        const string nom = c->getNom();
        if (nom == "Gold") goldPot += 1;
        else if (nom == "Ruby") goldPot += 2;
        else if (nom == "Dagger") combatPot += 1;
        else if (nom == "Shortsword" || nom == "Short Sword") combatPot += 2;
        else if (nom == "Fire Gem") goldPot += 2;
    }

    for (auto& c : champs) {
        const string nom = c->getNom();
        if (nom.find("Wolf Shaman") != string::npos) {
            combatPot += 2;
        }
    }

    return {goldPot, combatPot};
}

// affiche le petit menu en bas
static void afficherMenu(const Game& game) {
    cout << "\nCommandes : "
         << "E(etat)  H(main)  M(marche)  "
         << "C(champions)  CA(champ adv)  "
         << "S(sacrifiées)  A(acheter)  P(jouer)  "
         << "N(passer)  G(god)  Q(quitter)";
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

    cout << "=== HERO REALMS - Console ===\n";
    afficherMenu(game);

    while (!quitter && !game.estTerminee()) {
        Player& p   = joueurs[joueurActif];
        Player& adv = joueurs[1 - joueurActif];

        // 1) on calcule les ressources à partir de la main
        auto [goldPot, combatPot] = calculerGoldEtCombat(p);

        // 2) on les met dans le joueur pour que Game/Player puissent les utiliser
        p.resetPourNouveauTour();      // remet or/combat internes à 0
        p.modiffGold(goldPot);         // on met l’or calculé
        p.modiffCombat(combatPot);     // on met le combat calculé

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
                cout << "\n(F) Gemme de Feu :\n";
                cout << ui::CardRenderer::render(*gemme, opts);
            }

            cout << "\nAcheter quelle carte ? (1–5 ou F) : ";
            string choix;
            getline(cin, choix);
            for (auto& c : choix) c = toupper((unsigned char)c);

            if (choix == "F") {
                p.acheterGemmeDeFeu(game);
            } else {
                try {
                    int index = stoi(choix) - 1;
                    game.acheterCarte(index, p);
                } catch (...) {
                    cout << "Entrée invalide.\n";
                }
            }
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
        else if (cmd == "N") {
            p.resetPourNouveauTour();
            joueurActif = 1 - joueurActif;
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