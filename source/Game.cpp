#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include "../include/Objet.hpp"
#include "../include/Action.hpp"
#include "../include/Champion.hpp"
#include <cctype>
#include "../include/ui/CardRenderer.hpp"
#include "CardLoader.cpp"

Game::Game(){
    // create two players
    players.emplace_back(0);
    players.emplace_back(1);
}

std::vector<Player>& Game::getPlayers(){ return players; }

void Game::demarrerPartie(){
    std::cout << "Demarrage de la partie\n";
}

// (DataLoader removed) 

void Game::tourDeJeu(Player& joueur){
    std::cout << "Tour du joueur " << joueur.getId() << "\n";
}

void Game::afficherMarche() const{
    std::cout << "Marche: (" << marche.size() << " cartes)\n";
    ui::CardRenderer::Options opts;
    opts.width = 60;
    for(size_t i=0;i<marche.size();++i){
        std::cout << "--- Carte " << i+1 << " ---\n";
        try{
            std::cout << ui::CardRenderer::render(*marche[i], opts) << "\n";
        } catch(...){
            // fallback simple name if rendering fails
            std::cout << marche[i]->getNom() << " (cout=" << marche[i]->getCout() << ")\n";
        }
    }
}

Carte* Game::acheterCarte(int index, Player& acheteur){
    if(index<0 || index >= (int)marche.size()) return nullptr;

    // Simple purchase logic (no global modifiers): check funds, pay, transfer ownership
    Carte* cartePtr = marche[index].get();
    int prix = cartePtr->getCout();
    if(acheteur.getGold() < prix){
        std::cout << "Achat impossible : fonds insuffisants (cout=" << prix << ", or=" << acheteur.getGold() << ")\n";
        return nullptr;
    }

    // debit du joueur
    acheteur.modiffGold(-prix);

    // take ownership
    std::unique_ptr<Carte> carte = std::move(marche[index]);
    // remove from market
    marche.erase(marche.begin() + index);

    // Refill market from pioche if possible (keep market size stable)
    if(!pioche.empty()){
        // move top card from pioche into the vacated market slot (or end if erased last)
        std::unique_ptr<Carte> c = std::move(pioche.back());
        pioche.pop_back();
        // insert at the same index (if index == marche.size() this appends at end)
        marche.insert(marche.begin() + index, std::move(c));
    }

    // decide destination based on player's flags and card type
    if(acheteur.getNextAcquiredToHand()){
        std::cout << "Carte acquise -> main (flag nextAcquiredToHand).\n";
        acheteur.getMain().push_back(std::move(carte));
        acheteur.setNextAcquiredToHand(false);
        return acheteur.getMain().back().get();
    }

    // If card is an action and nextAcquiredActionToTopDeck is set, put on top of deck
    if(acheteur.getNextAcquiredActionToTopDeck() && cartePtr->getType() == TypeCarte::Action){
        std::cout << "Carte action acquise -> dessus du deck (flag nextAcquiredActionToTopDeck).\n";
        acheteur.getDeck().insert(acheteur.getDeck().begin(), std::move(carte));
        acheteur.setNextAcquiredActionToTopDeck(false);
        return acheteur.getDeck().front().get();
    }

    if(acheteur.getNextAcquiredToTopDeck()){
        std::cout << "Carte acquise -> dessus du deck (flag nextAcquiredToTopDeck).\n";
        acheteur.getDeck().insert(acheteur.getDeck().begin(), std::move(carte));
        acheteur.setNextAcquiredToTopDeck(false);
        return acheteur.getDeck().front().get();
    }

    // Default: go to defausse
    std::cout << "Carte achetee et mise en defausse par defaut.\n";
    acheteur.getDefausse().push_back(std::move(carte));
    return acheteur.getDefausse().back().get();
}

void Game::ajouterCarteMarche(std::unique_ptr<Carte> carte){
    marche.push_back(std::move(carte));
}

void Game::melangerPioche(){
    // naive shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pioche.begin(), pioche.end(), g);
}

void Game::afficherEtatJoueurs() const{
    for(const auto &p : players) p.afficherStats();
}

bool Game::estTerminee() const{ return false; }

void Game::afficherGagnant() const{
    std::cout << "Aucun gagnant determiner\n";
}

void Game::setCarteEnActivation(Carte* c) {
    carteEnActivation = c;
}

Carte* Game::getCarteEnActivation() const {
    return carteEnActivation;
}

void Game::initialiserMarche() {
    // Example initialization: add first 5 cards from pioche to marche
    for (int i = 0; i < 5 && !pioche.empty(); ++i) {
        marche.push_back(std::move(pioche.back()));
        pioche.pop_back();
    }
}

void Game::initialiserDeckBase() {
    Player& player1 = players[0];
    Player& player2 = players[1];
    deckBase1=CardLoader::loadPersonalDeck();
    deckBase2=CardLoader::loadPersonalDeck();
    for(auto& carte : deckBase1) {
        player1.getDefausse().push_back(std::move(carte));
    }
    for(auto& carte : deckBase2) {
        player2.getDefausse().push_back(std::move(carte));
    }
    deckBase1.clear();
    deckBase2.clear();
    player1.melangerDefausse();
    player2.melangerDefausse();
    player1.piocherCarte(5);
    player2.piocherCarte(5);
}