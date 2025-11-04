##### 🧙‍♂️ Hero Realms

**Projet de reproduction du jeu de cartes Hero Realms en C++, jouable dans le terminal.

🎯 Objectif

Ce projet vise à recréer les mécaniques principales du jeu original :

Construction de deck (achat de cartes au marché)

Gestion des ressources (or, combat, PV)

Activation de champions, objets et actions

Factions et effets combinés

Système de tours alternés (Joueur vs Joueur ou Joueur vs IA)**


# 🕹️ Fonctionnalités

Mode Joueur vs Joueur ⚔️

IA Heuristique 🤖

IA MCTS (Monte Carlo Tree Search) 🎲

Affichage texte enrichi avec bordures pour les cartes, couleurs pour les factions

Console avec couleurs et messages intuitifs

Gestion complète des effets de cartes (gain d’or, de combat, soins, sacrifices, défausse, etc...)

Pour la compilation, nous avons un Makefile:
Tapez: make run

src/ → Code source principal (Game, Player, Effets, UI…)

include/ → Headers

data/ → Fichier CSV des cartes

ui/ → Rendu texte et affichage terminal

ai/ → extension IA (Heuristic et MCTS)

