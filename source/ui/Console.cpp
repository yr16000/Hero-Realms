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
    std::cout << CYAN << BOLD << "⚔️  HERO REALMS\n" << RESET;

    // Joueur actif (avec emojis)
    std::cout << "Vous (J" << (moi.getId()+1) << ")  | "
              << "❤️  " << moi.getHp() << "   "
              << "💰 " << moi.getGold() << "   "
              << "⚔️  " << moi.getAtk()  << "\n";

    // Adversaire (PV uniquement)
    std::cout << "Adversaire (J" << (adv.getId()+1) << ") | ❤️  " << adv.getHp() << "\n";
}

void Console::afficherPlateau(Game& game, Player& moi, Player& adv, bool pauseApres) {
    using namespace Ansi;
    std::cout << GRAY << "────────────────────────────────────────────────────────\n" << RESET;

    // Champions du joueur
    auto& mesChamps = moi.getChampionsEnJeu();
    std::cout << "🛡️  Vos champions :\n";
    if (mesChamps.empty()) {
        std::cout << "  (Aucun)\n";
    } else {
        for (size_t i = 0; i < mesChamps.size(); ++i) {
            auto* ch = dynamic_cast<Champion*>(mesChamps[i].get());
            std::cout << "  " << (i+1) << ") "
                      << colorFor(mesChamps[i]->getFaction())
                      << mesChamps[i]->getNom() << Ansi::RESET;
            if (ch) {
                std::cout << " (PV=" << ch->getPv() << ")";
                if (ch->getEstGarde()) std::cout << " [Garde]";
                if (ch->getEstActiver()) std::cout << " (activé)";
            }
            std::cout << "\n";
        }
    }

    // Champions adverses
    auto& advChamps = adv.getChampionsEnJeu();
    std::cout << "🛡️  Champions adverses :\n";
    if (advChamps.empty()) {
        std::cout << "  (Aucun)\n";
    } else {
        for (size_t i = 0; i < advChamps.size(); ++i) {
            auto* ch = dynamic_cast<Champion*>(advChamps[i].get());
            std::cout << "  " << (i+1) << ") "
                      << colorFor(advChamps[i]->getFaction())
                      << advChamps[i]->getNom() << Ansi::RESET;
            if (ch) {
                std::cout << " (PV=" << ch->getPv() << ")";
                if (ch->getEstGarde()) std::cout << " [Garde]";
                if (ch->getEstActiver()) std::cout << " (activé)";
            }
            std::cout << "\n";
        }
    }

    std::cout << GRAY << "────────────────────────────────────────────────────────\n" << RESET;
    std::cout << Ansi::BOLD << "🏪 Marché \n" << Ansi::RESET;
    game.afficherMarche(); // s’appuie sur CardRenderer côté Game

    if (pauseApres) attendreEntree();
}

void Console::afficherMenu(Game& game) {
    using namespace Ansi;
    std::cout << "\n" << CYAN << BOLD << "Que voulez-vous faire ?\n" << RESET;
    std::cout << " [1] 🃏 Jouer une carte\n";
    std::cout << " [2] ⚡ Activer un champion\n";
    std::cout << " [3] 📦 Voir les sacrifices\n";
    std::cout << " [4] 🏪 Acheter au marché\n";
    std::cout << " [5] ⚔️  Attaquer\n";
    std::cout << " [6] ➡️  Terminer le tour\n";
    std::cout << " [7] 📋 Voir le plateau\n";
    std::cout << " [8] 🔮 Toggle God Mode";
    if (game.isGodMode()) std::cout << " " << Ansi::YELLOW << "[ACTIF]" << Ansi::RESET;
    std::cout << "\n [0] ❌ Quitter\n";
}

void Console::jouerUneCarte(Player& p, Game& game) {
    auto& hand = p.getMain();
    if (hand.empty()) {
        std::cout << Ansi::RED << "Aucune carte en main.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }
    std::cout << Ansi::BOLD << "Votre main :\n" << Ansi::RESET;
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << "  [" << (i+1) << "] "
                  << colorFor(hand[i]->getFaction()) << hand[i]->getNom() << Ansi::RESET
                  << " (coût " << hand[i]->getCout() << ")\n";
    }
    int c = lireChoix("Carte à jouer", 1, (int)hand.size());
    if (c == 0) return;

    // Aperçu visuel via CardRenderer
    ui::CardRenderer::Options opts; opts.width = 60;
    std::cout << "\n" << ui::CardRenderer::render(*hand[c-1], opts) << "\n";

    p.jouerCarte(c, game); // ton flux prend un index 1-based dans ton main précédent
    attendreEntree();
}

void Console::activerUnChampion(Player& p, Game& game) {
    auto& champs = p.getChampionsEnJeu();
    if (champs.empty()) {
        std::cout << Ansi::RED << "Aucun champion en jeu.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }
    std::cout << Ansi::BOLD << "Vos champions :\n" << Ansi::RESET;
    for (size_t i = 0; i < champs.size(); ++i) {
        auto* ch = dynamic_cast<Champion*>(champs[i].get());
        std::cout << "  [" << (i+1) << "] "
                  << colorFor(champs[i]->getFaction()) << champs[i]->getNom() << Ansi::RESET;
        if (ch) {
            std::cout << " (PV=" << ch->getPv() << ")";
            if (ch->getEstGarde()) std::cout << " [Garde]";
            if (ch->getEstActiver()) std::cout << " (activé)";
        }
        std::cout << "\n";
    }
    int c = lireChoix("Champion à activer", 1, (int)champs.size());
    if (c == 0) return;

    auto* cible = dynamic_cast<Champion*>(champs[c-1].get());
    if (!cible) {
        std::cout << Ansi::RED << "La carte choisie n'est pas un champion.\n" << Ansi::RESET;
        attendreEntree();
        return;
    }
    cible->activer(p, game);
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
