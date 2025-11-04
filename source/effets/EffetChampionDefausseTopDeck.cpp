#include "../../include/effets/EffetChampionDefausseTopDeck.hpp"
#include "../../include/Player.hpp"
#include "../../include/ui/CardRenderer.hpp"
#include <iostream>

EffetChampionDefausseTopDeck::EffetChampionDefausseTopDeck() : Effet("Expend: put a champion from discard on top of deck") {}

// Remettre un champion de la défausse sur le dessus du deck
void EffetChampionDefausseTopDeck::activerEffet(Player& proprietaire, Game& game){
    auto &def = proprietaire.getDefausse();
    proprietaire.afficherDefausse();
    std::vector<size_t> champsIdx;
    for(size_t i=0;i<def.size();++i){
        if(def[i] && def[i]->getType() == TypeCarte::Champion){
            champsIdx.push_back(i);
        }
    }
// Aucun champion dans la défausse
    if(champsIdx.empty()){
        std::cout << "Aucun champion dans la defausse.\n";
        return;
    }
// Un seul champion dans la défausse
    if(champsIdx.size() == 1){
        size_t idx = champsIdx[0];
        std::cout << "Remettre sur le dessus du deck: " << def[idx]->getNom() << "\n";
        proprietaire.getDeck().push_back(std::move(def[idx]));
        def.erase(def.begin()+idx);
        return;
    }
// Plusieurs champions dans la défausse
    for(size_t k=0;k<champsIdx.size();++k){
        size_t i = champsIdx[k];
        ui::CardRenderer::Options opts;
        opts.width = 60;
        std::cout << k << ": " << ui::CardRenderer::render(*def[i], opts) << "\n";
    }
    std::cout << "Choisissez un champion dans la defausse à remettre sur le dessus du deck:\n";
    int choice=-1;
    std::cin >> choice;
    if(!std::cin){
        std::cin.clear(); std::string dummy; std::getline(std::cin,dummy);
        std::cout << "Entrée invalide.\n";
        return;
    }
    if(choice < 0 || static_cast<size_t>(choice) >= champsIdx.size()){
        std::cout << "Choix invalide.\n";
        return;
    }
    size_t chosenIdx = champsIdx[choice];
    std::cout << "Remettre sur le dessus du deck: " << def[chosenIdx]->getNom() << "\n";
    proprietaire.getDeck().push_back(std::move(def[chosenIdx]));
    def.erase(def.begin()+chosenIdx);
}

std::string EffetChampionDefausseTopDeck::toString() const {
    return "Activer: Remettre un champion de la défausse sur le dessus du deck";
}
