#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include "../include/Objet.hpp"
#include "../include/Action.hpp"
#include "../include/Champion.hpp"
#include <cctype>
#include "../include/ui/CardRenderer.hpp"

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
        std::cout << "--- Carte " << i << " ---\n";
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
    // For now, just return raw pointer; ownership remains with Game
    return marche[index].get();
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
