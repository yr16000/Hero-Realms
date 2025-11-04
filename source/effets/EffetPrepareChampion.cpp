#include "../../include/effets/EffetPrepareChampion.hpp"
#include "../../include/ai/HeuristicAI.hpp"
#include "../../include/ai/GameEvaluator.hpp"
#include <iostream>
#include <algorithm>
#include "../../include/Champion.hpp"
#include <vector>

EffetPrepareChampion::EffetPrepareChampion() : Effet("Préparer un champion") {}

void EffetPrepareChampion::activerEffet(Player& proprietaire, Game& game) {
    if(proprietaire.getChampionsEnJeu().empty()) {
        if (!game.isAIPlayer(proprietaire.getId())) {
            std::cout << "Aucun champion en jeu à préparer.\n";
        }
        return;
    }
    
    // Trouver les champions activés
    std::vector<int> activerIndices;
    for(size_t i = 0; i < proprietaire.getChampionsEnJeu().size(); ++i) {
        auto champ = dynamic_cast<Champion*>(proprietaire.getChampionsEnJeu()[i].get());
        if(champ && champ->getEstActiver()) {
            activerIndices.push_back(i);
        }
    }
    
    if(activerIndices.empty()) {
        if (!game.isAIPlayer(proprietaire.getId())) {
            std::cout << "Tous les champions sont déjà préparés.\n";
        }
        return;
    }
    
    int selectedIndex = -1;
    
    // Vérifier si c'est l'IA qui joue
    if (game.isAIPlayer(proprietaire.getId())) {
        // L'IA choisit le champion avec le plus faible score
        // (on veut garder les meilleurs champions activés)
        float lowestScore = 999999.0f;
        for(const auto& idx : activerIndices) {
            const Carte* carte = proprietaire.getChampionsEnJeu()[idx].get();
            float score = GameEvaluator::evaluateCardValue(carte);
            if (score < lowestScore) {
                lowestScore = score;
                selectedIndex = idx;
            }
        }
    } else {
        // Joueur humain : demander via console
        std::cout << "Champions qui peuvent etre preparés :\n";
        for(const auto& idx : activerIndices) {
            std::cout << idx + 1 << ". " << proprietaire.getChampionsEnJeu()[idx]->getNom() << "\n";
        }
        std::cout << "Sélectionnez le numéro du champion à préparer : ";
        int index;
        std::cin >> index;
        while(std::find(activerIndices.begin(), activerIndices.end(), index - 1) == activerIndices.end()) {
            std::cout << "Index invalide, choisissez un champion activé.\n";
            std::cout << "Choisissez :\n";
            std::cin >> index;
        }
        selectedIndex = index - 1;
    }
    
    if (selectedIndex >= 0 && selectedIndex < (int)proprietaire.getChampionsEnJeu().size()) {
        auto champ = dynamic_cast<Champion*>(proprietaire.getChampionsEnJeu()[selectedIndex].get());
        if(champ) {
            champ->setEstActiver(false);
            if (!game.isAIPlayer(proprietaire.getId())) {
                std::cout << "Champion préparé.\n";
            }
        }
    }
}

std::string EffetPrepareChampion::toString() const {
    return std::string("Préparer un champion en jeu (retirer l'état activé)");
}