#pragma once
#ifndef UI_CONSOLE_HPP
#define UI_CONSOLE_HPP

#include "../Game.hpp"
#include "../Player.hpp"
#include "../Carte.hpp"
#include "../Champion.hpp"
#include "../Enums.h"
#include "CardRenderer.hpp"
#include <string>
#include "../Action.hpp"

namespace ui {

// Couleurs ANSI simples
namespace Ansi {
    static constexpr const char* RESET  = "\033[0m";
    static constexpr const char* BOLD   = "\033[1m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE   = "\033[34m";
    static constexpr const char* RED    = "\033[31m";
    static constexpr const char* GREEN  = "\033[32m";
    static constexpr const char* CYAN   = "\033[36m";
    static constexpr const char* GRAY   = "\033[90m";
    static constexpr const char* BROWN       = "\033[38;5;94m";  
    static constexpr const char* DARK_BROWN  = "\033[38;5;130m"; 
    static constexpr const char* ORANGE      = "\033[38;5;208m";
}

/**
 * Console texte du jeu.
 * - Menu NUMÉROTÉ (1..n, 0 = retour/annuler/quit)
 * - Factions colorées : Impériale=jaune, Guilde=bleu, Necros=rouge, Sauvage=vert
 * - Utilise CardRenderer pour afficher les cartes (main, marché, sacrifices)
 * - “Voir les sacrifices” au lieu de “Sacrifier une carte”
 * - Un seul choix à la fois (pas de "1,2,3")
 * - Pas d'affichage du plateau au lancement : uniquement en option.
 */
class Console {
public:
    // En-tête (Joueur actif + Adversaire) avec emojis
    static void afficherHeader(Game& game, Player& moi, Player& adv);

    // Plateau complet (champions des 2 côtés + marché via CardRenderer)
    static void afficherPlateau(Game& game, Player& moi, Player& adv, bool pauseApres=false);

    // Menu principal (numéroté)
    static void afficherMenu(Game& game);

    // Lit un entier via getline, retourne 0 pour Annuler/Quitter (visible dans le prompt)
    static int lireChoix(const std::string& prompt, int min, int max);

    // Couleur par faction
    static const char* colorFor(Faction f);

    // Actions
    static void jouerUneCarte(Player& p, Game& game);
    static void activerUnChampion(Player& p, Game& game);
    static void sacrifierUneCarte(Player& p, Game& game);
    static void voirSacrifices(Player& p, Game& game);
    static void acheterAuMarche(Player& p, Game& game);
    static void attaquer(Player& p, Player& adv, Game& game);
    static void voirDefausse(Player& p, Game& game);


    // Utilitaires
    static void attendreEntree(const std::string& msg="(Entrée pour continuer)");
    static std::string lireLigne(const std::string& prompt);
};

} // namespace ui

#endif