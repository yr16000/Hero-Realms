#include "../../include/effets/EffetSacrifier.hpp"
#include "../../include/Player.hpp"
#include "../../include/ai/HeuristicAI.hpp"
#include "../../include/ai/GameEvaluator.hpp"
#include <iostream>
#include <vector>
#include <string>

EffetSacrifier::EffetSacrifier(int nombre, std::unique_ptr<Effet>&& effetSiSacrifie)
    : Effet("You may sacrifice up to N cards"), nombre(nombre), effetSiSacrifie(std::move(effetSiSacrifie)) {}

void EffetSacrifier::activerEffet(Player& proprietaire, Game& game){
    int n=nombre;
    
    bool isAI = game.isAIPlayer(proprietaire.getId());
    
    while(n>0){
        bool continueSacrifice = false;
        
        if (isAI) {
            // L'IA sacrifie seulement si elle a des cartes faibles (Gold/Dagger/Ruby)
            bool hasBadCards = false;
            for(const auto& card : proprietaire.getMain()) {
                float val = GameEvaluator::evaluateCardValue(card.get());
                if (val <= 2.0f) {
                    hasBadCards = true;
                    break;
                }
            }
            if (!hasBadCards) {
                for(const auto& card : proprietaire.getDefausse()) {
                    float val = GameEvaluator::evaluateCardValue(card.get());
                    if (val <= 2.0f) {
                        hasBadCards = true;
                        break;
                    }
                }
            }
            continueSacrifice = hasBadCards;
        } else {
            // Joueur humain
            std::cout << "Vous pouvez sacrifier jusqu'à " << n << " cartes de votre main ou defausse.\n";
            std::cout<<"Entrez 0 pour arrêter de sacrifier des cartes ou 1 pour continuer:\n";
            int choix;
            std::cin >> choix;
            continueSacrifice = (choix == 1);
        }
        
        if(!continueSacrifice){
            break;
        }
        
        bool fromMain=true;
        int index = -1;
        
        if (isAI) {
            // L'IA cherche la pire carte dans main puis defausse
            float lowestScore = 999999.0f;
            int bestIndex = -1;
            
            // Chercher dans la main
            for(size_t i = 0; i < proprietaire.getMain().size(); ++i) {
                float score = GameEvaluator::evaluateCardValue(proprietaire.getMain()[i].get());
                if (score < lowestScore) {
                    lowestScore = score;
                    bestIndex = i;
                    fromMain = true;
                }
            }
            
            // Chercher dans la défausse
            for(size_t i = 0; i < proprietaire.getDefausse().size(); ++i) {
                float score = GameEvaluator::evaluateCardValue(proprietaire.getDefausse()[i].get());
                if (score < lowestScore) {
                    lowestScore = score;
                    bestIndex = i;
                    fromMain = false;
                }
            }
            
            index = bestIndex + 1; // Convertir en 1-based
        } else {
            // Joueur humain
            std::cout << "Votre main:\n";
            proprietaire.afficherMainDetaillee();
            std::cout << "Votre defausse:\n";
            proprietaire.afficherDefausse();
            if(proprietaire.getMain().empty() && proprietaire.getDefausse().empty()){
                std::cout << "Vous n'avez pas de cartes à sacrifier.\n";
                break;
            }
            std::cout<<"Sacrifier de la main ou de la defausse? (1/0): ";
            int choixSource;
            std::cin >> choixSource;
            if(choixSource==0){
                fromMain=false;
            }
            (choixSource==1?proprietaire.afficherMainDetaillee():proprietaire.afficherDefausse());
            std::cout << "Entrez le numéro de la carte à sacrifier: ";
            std::cin >> index;
            
            if(fromMain){
                if(proprietaire.getMain().empty()){
                    std::cout << "Votre main est vide. Choisissez une carte dans la défausse.\n";
                    fromMain=false;
                }
                else {
                    while(index<1 || index>(int)proprietaire.getMain().size()){
                        std::cout << "Index invalide, reessayez: ";
                        std::cin >> index;
                    }
                }
            } else {
                if(proprietaire.getDefausse().empty()){
                    std::cout << "Votre défausse est vide. Choisissez une carte dans la main.\n";
                    fromMain=true;
                }
                else {
                while(index<1 || index>(int)proprietaire.getDefausse().size()){
                    std::cout << "Index invalide, reessayez: ";
                    std::cin >> index;
                }
                }
            }
        }
        
        if (index > 0) {
            if(fromMain && index <= (int)proprietaire.getMain().size()){
                Carte* carteASacrifier = proprietaire.getMain()[index-1].get();
                proprietaire.sacrifierCarte(carteASacrifier, game);
            } else if (!fromMain && index <= (int)proprietaire.getDefausse().size()) {
                Carte* carteASacrifier = proprietaire.getDefausse()[index-1].get();
                proprietaire.sacrifierCarte(carteASacrifier, game);
            }
            
            if(effetSiSacrifie){
                effetSiSacrifie->activerEffet(proprietaire, game);
            }
        }
        n--;
    }
}

std::string EffetSacrifier::toString() const {
    std::string s = "Vous pouvez sacrifier jusqu'à ";
    s += std::to_string(nombre);
    s += " carte(s) de votre main ou de votre défausse";
    if(effetSiSacrifie) {
        s += ". Si vous le faites, ";
        s += effetSiSacrifie->toString();
    }
    return s;
}
