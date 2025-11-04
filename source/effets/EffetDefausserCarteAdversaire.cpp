#include "../../include/effets/EffetDefausserCarteAdversaire.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/ai/GameEvaluator.hpp"
#include <iostream>

EffetDefausserCarteAdversaire::EffetDefausserCarteAdversaire(int nb): Effet("Defausser adversaire"), nb(nb){}

void EffetDefausserCarteAdversaire::activerEffet(Player& proprietaire, Game& game){
    if (!game.isAIPlayer(proprietaire.getId())) {
        std::cout << "Forcer l'adversaire a defausser "<< nb << " carte(s)\n";
    }
    
    Player* adversaire = nullptr;
    for(auto& player: game.getPlayers()){
        if(player.getId() != proprietaire.getId()){
            adversaire = &player;
            break;
        }
    }
    if(adversaire == nullptr){
        if (!game.isAIPlayer(proprietaire.getId())) {
            std::cout << "Aucun adversaire trouve.\n";
        }
        return;
    }
    
    for(int i=0;i<nb;++i){
        if(adversaire->getMain().empty()){
            if (!game.isAIPlayer(proprietaire.getId())) {
                std::cout << "L'adversaire n'a plus de cartes en main.\n";
            }
            break;
        }
        
        int choix = 1;
        
        if (game.isAIPlayer(adversaire->getId())) {
            // L'IA choisit sa pire carte à défausser
            float lowestScore = 999999.0f;
            int worstIndex = 0;
            for(size_t j = 0; j < adversaire->getMain().size(); ++j) {
                float score = GameEvaluator::evaluateCardValue(adversaire->getMain()[j].get());
                if (score < lowestScore) {
                    lowestScore = score;
                    worstIndex = j;
                }
            }
            choix = worstIndex + 1;
        } else {
            // Joueur humain
            std::cout << "Adversaire, choisissez une carte a defausser:\n";
            adversaire->afficherMainDetaillee();
            std::cin >> choix;
            while(choix<1 || choix>(int)adversaire->getMain().size()){
                std::cout << "Choix invalide, reessayez: ";
                std::cin >> choix;
            }
        }
        
        if (choix >= 1 && choix <= (int)adversaire->getMain().size()) {
            adversaire->getDefausse().push_back(std::move(adversaire->getMain()[choix-1]));
            adversaire->getMain().erase(adversaire->getMain().begin()+(choix-1));
            if (!game.isAIPlayer(proprietaire.getId())) {
                std::cout<< "Carte defaussee.\n";
            }
        }
    }
}

std::string EffetDefausserCarteAdversaire::toString() const {
    return std::string("Forcer l'adversaire à défausser ") + std::to_string(nb) + (nb > 1 ? " cartes" : " carte");
}
