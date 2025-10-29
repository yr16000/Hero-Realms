#include "../../include/effets/EffetPrepareChampion.hpp"
#include <iostream>
#include <algorithm>

EffetPrepareChampion::EffetPrepareChampion() : Effet("Préparer un champion") {}

void EffetPrepareChampion::activerEffet(Player& proprietaire, Game& game) {
    if(proprietaire.getChampionsEnJeu().empty()) {
        std::cout << "Aucun champion en jeu à préparer.\n";
        return;
    }
    bool aActiver = false;
    for(const auto& champPtr : proprietaire.getChampionsEnJeu()) {
        auto champ = dynamic_cast<Champion*>(champPtr.get());
        if(champ && champ->getEstActiver()) {
            aActiver = true;
            break;
        }
    }
    if(!aActiver) {
        std::cout << "Tous les champions sont déjà préparés.\n";
        return;
    }
    std::vector<int> activerIndices;
    for(size_t i = 0; i < proprietaire.getChampionsEnJeu().size(); ++i) {
        auto champ = dynamic_cast<Champion*>(proprietaire.getChampionsEnJeu()[i].get());
        if(champ && champ->getEstActiver()) {
            activerIndices.push_back(i);
        }
    }
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
    auto champ = dynamic_cast<Champion*>(proprietaire.getChampionsEnJeu()[index - 1].get());
    if(champ) {
        champ->setEstActiver(false);
        std::cout << "Champion préparé.\n";
    }
}

std::string EffetPrepareChampion::toString() const {
    return std::string("Préparer un champion en jeu (retirer l'état activé)");
}