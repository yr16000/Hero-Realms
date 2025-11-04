#include "../../include/ui/Console.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include <cctype>

using namespace ui;

static bool toInt(const std::string& s, int& out) {
    if (s.empty()) return false;
    for (char c: s) if (!std::isdigit((unsigned char)c)) return false;
    try { out = std::stoi(s); } catch(...) { return false; }
    return true;
}

void Console::attendreEntree(const std::string& msg) {
    std::cout << Ansi::GRAY << msg << Ansi::RESET;
    std::string _; std::getline(std::cin, _);
}

std::string Console::lireLigne(const std::string& prompt) {
    std::cout << prompt;
    std::string s; std::getline(std::cin, s);
    return s;
}

int Console::lireChoix(const std::string& prompt, int min, int max) {
    while (true) {
        std::ostringstream oss;
        oss << prompt << " (" << min << "-" << max << ", 0=Annuler) : ";
        std::string s = lireLigne(oss.str());
        if (s == "0") return 0;
        int n;
        if (toInt(s, n) && n >= min && n <= max) return n;
        std::cout << Ansi::RED << "Entrée invalide.\n" << Ansi::RESET;
    }
}

const char* Console::colorFor(Faction f) {
    using namespace Ansi;
    switch (f) {
        case Faction::Imperiale: return YELLOW; // 👑 Impériale
        case Faction::Guilde:    return BLUE;   // 💰 Guilde
        case Faction::Necros:    return RED;    // 💀 Nécros
        case Faction::Sauvage:   return GREEN;  // 🌿 Sauvage
        default:                 return RESET;
    }
}

void Console::afficherHeader(Game& /*game*/, Player& moi, Player& adv) {
    using namespace Ansi;
    std::cout << GREEN << BOLD << "⚔️  HERO REALMS\n" << RESET;

    // Joueur actif 
    std::cout << "Vous (Joueur" << (moi.getId()+1) << ")  | "
              << "PV: " << moi.getHp() << "❤️   "
              << "OR : " << moi.getGold() << "💰  "
              << "COMBAT: " << moi.getAtk()  << "⚔️\n";

    // Adversaire (PV uniquement)
    std::cout << "Adversaire (Joueur" << (adv.getId()+1) << ") | PV: " << adv.getHp() << "❤️\n";
}

void Console::afficherPlateau(Game& game, Player& moi, Player& adv, bool pauseApres) {
    using namespace Ansi;
    std::cout << GRAY << "────────────────────────────────────────────────────────\n" << RESET;

    ui::CardRenderer::Options ro;
    ro.width = 60;
    ro.perRow = 2;          // 2 cartes côte à côte
    ro.showIndices = false; // pas d’indices sur le plateau

    // ----- Champions du joueur -----
    auto& mesChamps = moi.getChampionsEnJeu();
    std::cout << "🛡️  Vos champions :\n";
    if (mesChamps.empty()) {
        std::cout << "  (Aucun)\n";
    } else {
        std::vector<const Carte*> mine;
        mine.reserve(mesChamps.size());
        for (const auto& c : mesChamps) mine.push_back(c.get());

        try {
            std::cout << ui::CardRenderer::renderMultiple(mine, ro) << "\n";
        } catch (...) {
            // Fallback minimal
            for (size_t i = 0; i < mine.size(); ++i) {
                std::cout << "  - " << mine[i]->getNom() << "\n";
            }
        }
    }

    // ----- Champions adverses -----
    auto& advChamps = adv.getChampionsEnJeu();
    std::cout << "🛡️  Champions adverses :\n";
    if (advChamps.empty()) {
        std::cout << "  (Aucun)\n";
    } else {
        std::vector<const Carte*> theirs;
        theirs.reserve(advChamps.size());
        for (const auto& c : advChamps) theirs.push_back(c.get());

        try {
            std::cout << ui::CardRenderer::renderMultiple(theirs, ro) << "\n";
        } catch (...) {
            // Fallback minimal
            for (size_t i = 0; i < theirs.size(); ++i) {
                std::cout << "  - " << theirs[i]->getNom() << "\n";
            }
        }
    }

    std::cout << GRAY << "────────────────────────────────────────────────────────\n" << RESET;
    std::cout << Ansi::BOLD << "🏪 Marché \n" << Ansi::RESET;
    game.afficherMarche(); // s’appuie sur CardRenderer côté Game

    if (pauseApres) attendreEntree();
}


void Console::afficherMenu(Game& game) {
    using namespace Ansi;
    std::cout << "\n" << GREEN << BOLD << "Que voulez-vous faire ?\n" << RESET;
    std::cout << " [1] 🃏 Jouer une carte\n";
    std::cout << " [2] ⚡ Activer un champion\n";
    std::cout << " [3] 📦 Voir les sacrifices\n";
    std::cout << " [4] 🏪 Acheter au marché\n";
    std::cout << " [5] ⚔️  Attaquer\n";
    std::cout << " [6] ➡️  Terminer le tour\n";
    std::cout << " [7] 📋 Voir le plateau\n";
    std::cout << " [8] 🔮 God Mode";
    if (game.isGodMode()) std::cout << " " << Ansi::YELLOW << "[ACTIF]" << Ansi::RESET;
    std::cout << " \n [9] 🗃️  Voir la défausse";
    std::cout << "\n [0] ❌ Quitter\n";
}

void Console::jouerUneCarte(Player& p, Game& game) {
    auto& hand = p.getMain();
    if (hand.empty()) {
        std::cout << Ansi::RED << "Aucune carte en main.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }

    // Affichage visuel de la main en grille 2 par ligne
    {
        ui::CardRenderer::Options opts;
        opts.width = 60;          // même largeur que le marché
        opts.perRow = 2;          // 2 cartes par ligne
        opts.showIndices = true;  // affiche [1] [2] ...

        std::vector<const Carte*> cartes;
        cartes.reserve(hand.size());
        for (const auto& c : hand) cartes.push_back(c.get());

        std::cout << Ansi::BOLD << "Votre main (" << hand.size() << " cartes)\n" << Ansi::RESET;
        try {
            std::cout << ui::CardRenderer::renderMultiple(cartes, opts) << "\n";
        } catch (...) {
            // fallback minimal si jamais le renderer plante
            for (size_t i = 0; i < hand.size(); ++i) {
                std::cout << "[" << (i+1) << "] "
                          << colorFor(hand[i]->getFaction()) << hand[i]->getNom() << Ansi::RESET
                          << " (coût " << hand[i]->getCout() << ")\n";
            }
        }
    }

    // --- Choix utilisateur (inchangé) ---
    int c = lireChoix("Carte à jouer", 1, (int)hand.size());
    if (c == 0) return;

    // Aperçu de la carte choisie
    ui::CardRenderer::Options opts; opts.width = 60;
    std::cout << "\n" << ui::CardRenderer::render(*hand[c-1], opts) << "\n";

    p.jouerCarte(c, game); // ton flux prend un index 1-based
    attendreEntree();
}


void Console::activerUnChampion(Player& p, Game& game) {
    auto& champs = p.getChampionsEnJeu();
    if (champs.empty()) {
        std::cout << Ansi::RED << "Aucun champion en jeu.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }

    std::cout << Ansi::BOLD << "Vos champions en jeu :\n" << Ansi::RESET;

    ui::CardRenderer::Options opts;
    opts.width = 60;          // même gabarit que le marché
    opts.showIndices = false; // on gère nous-mêmes l'entête [i]

    // Affiche chaque champion individuellement
    for (size_t i = 0; i < champs.size(); ++i) {
        auto* c = champs[i].get();
        auto* ch = dynamic_cast<Champion*>(c);

        // Entête [i] + tag (activé) en rouge si actif
        std::cout << "[" << (i + 1) << "] ";
        if (ch && ch->getEstActiver()) {
            std::cout << Ansi::RED << "(activé)" << Ansi::RESET << " ";
        }
        std::cout << "\n";

        // Rendu complet de la carte (une par une)
        try {
            std::cout << ui::CardRenderer::render(*c, opts) << "\n";
        } catch (...) {
            // fallback minimal en cas de souci
            std::cout << c->getNom() << " (coût=" << c->getCout() << ")\n";
        }

        // Petite séparation visuelle entre les cartes
        std::cout << "\n";
    }

    // Choix du champion à activer
    int cidx = lireChoix("Champion à activer", 1, (int)champs.size());
    if (cidx == 0) return;

    auto* cible = dynamic_cast<Champion*>(champs[(size_t)cidx - 1].get());
    if (!cible) {
        std::cout << Ansi::RED << "La carte choisie n'est pas un champion.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }

    // Aperçu avant activation
    std::cout << "Avant activation :\n"
              << ui::CardRenderer::render(*cible, opts) << "\n";

    // Activation
    cible->activer(p, game);

    // Réaffichage du tag (activé) en dehors, en rouge
    std::cout << "Après activation : "
              << Ansi::RED << "(activé)" << Ansi::RESET << "\n"
              << ui::CardRenderer::render(*cible, opts) << "\n";

    attendreEntree();
}


void Console::voirSacrifices(Player& p, Game& /*game*/) {
    // Affichage interne existant (peut s’appuyer sur CardRenderer si tu l’y utilises)
    p.afficherSacrifices();
    attendreEntree();
}

void Console::acheterAuMarche(Player& p, Game& game) {
    game.afficherMarche();

    if (const Carte* gemme = game.getModeleGemmeDeFeu()) {
        ui::CardRenderer::Options opts; opts.width = 60;
        std::cout << "\n(F) Gemme de Feu :\n"
                  << ui::CardRenderer::render(*gemme, opts) << "\n";
    }
    p.acheterCarte(game);
    attendreEntree();
}

void Console::attaquer(Player& p, Player& adv, Game& /*game*/) {
    auto& advChamps = adv.getChampionsEnJeu();

    std::cout << Ansi::BOLD << "Cible :\n" << Ansi::RESET;
    std::cout << "  [1] Joueur adverse\n";
    std::cout << "  [2] Champion adverse\n";
    int c = lireChoix("Choix", 1, 2);
    if (c == 0) return;

    if (c == 1) {
        p.attaquer(adv, nullptr);
        attendreEntree();
        return;
    }

    if (advChamps.empty()) {
        std::cout << Ansi::RED << "Aucun champion adverse.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }
    for (size_t i = 0; i < advChamps.size(); ++i) {
        auto* ch = dynamic_cast<Champion*>(advChamps[i].get());
        std::cout << "  [" << (i+1) << "] "
                  << colorFor(advChamps[i]->getFaction()) << advChamps[i]->getNom() << Ansi::RESET;
        if (ch) {
            std::cout << " (PV=" << ch->getPv() << ")";
            if (ch->getEstGarde()) std::cout << " [Garde]";
            if (ch->getEstActiver()) std::cout << " (activé)";
        }
        std::cout << "\n";
    }
    int t = lireChoix("Champion à cibler", 1, (int)advChamps.size());
    if (t == 0) return;

    auto* cible = dynamic_cast<Champion*>(advChamps[t-1].get());
    p.attaquer(adv, cible);
    attendreEntree();
}


void Console::voirDefausse(Player& p, Game& /*game*/) {
    std::cout << "\n";
    p.afficherDefausse();  // ⬅️ réutilise la méthode de Player
    attendreEntree();
}

