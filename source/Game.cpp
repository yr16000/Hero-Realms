#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include "../include/Objet.hpp"
#include "../include/Action.hpp"
#include "../include/Champion.hpp"
#include <cctype>
#include "../include/ui/CardRenderer.hpp"
#include "../include/CardLoader.hpp"

Game::Game(){
    // create two players
    players.emplace_back(0);
    players.emplace_back(1);
}

std::vector<Player>& Game::getPlayers(){ return players; }

void Game::demarrerPartie(){
    pioche=CardLoader::loadPioche();
    melangerPioche();
    initialiserDeckBase();
    initialiserMarche();
    initialiserGemmesDeFeu();
    std::cout << "Initialisation terminée. Demarrage de la partie\n";
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
    if (godMode) {
        std::cout << "-- Pioche (accessible en GodMode): " << pioche.size() << " cartes --\n";
        for (size_t i = 0; i < pioche.size(); ++i) {
            std::cout << "--- Pioche " << (marche.size() + i + 1) << " ---\n";
            try {
                std::cout << ui::CardRenderer::render(*pioche[i], opts) << "\n";
            } catch(...) {
                std::cout << pioche[i]->getNom() << " (cout=" << pioche[i]->getCout() << ")\n";
            }
        }
    }
}

Carte* Game::acheterCarte(int index, Player& acheteur){
    int marcheSize = (int)marche.size();
    int piocheSize = (int)pioche.size();
    int totalAvailable = marcheSize + (godMode ? piocheSize : 0);
    if(totalAvailable == 0){
        std::cout << "Achat impossible : marche et pioche vides\n";
        return nullptr;
    }
    if (index < 0 || index >= totalAvailable) return nullptr;

    bool fromPioche = false;
    std::unique_ptr<Carte> carteUptr;
    Carte* cartePtrRaw = nullptr;

    if (index < marcheSize) {
        cartePtrRaw = marche[index].get();
    } else {
        fromPioche = true;
        int piocheIndex = index - marcheSize;
        cartePtrRaw = pioche[piocheIndex].get();
    }

    int prix = cartePtrRaw->getCout();
    if (!godMode) {
        if(acheteur.getGold() < prix){
            std::cout << "Achat impossible : fonds insuffisants (cout=" << prix << ", or=" << acheteur.getGold() << ")\n";
            return nullptr;
        }
        // debit du joueur
        acheteur.modiffGold(-prix);
    } else {
        // in god mode, gold is unlimited: do not check or debit
        std::cout << "GodMode: achat gratuit autorisé.\n";
    }

    // take ownership from appropriate container
    if (!fromPioche) {
        carteUptr = std::move(marche[index]);
        marche.erase(marche.begin() + index);
        if(!pioche.empty()){
            std::unique_ptr<Carte> c = std::move(pioche.back());
            pioche.pop_back();
            marche.insert(marche.begin() + index, std::move(c));
        }
    } else {
        int piocheIndex = index - marcheSize;
        carteUptr = std::move(pioche[piocheIndex]);
        pioche.erase(pioche.begin() + piocheIndex);
    }

    Carte* cartePtr = carteUptr.get();

    // decide destination based on godMode or player's flags and card type
    if (godMode) {
        std::cout << "GodMode: carte acquise -> main directement.\n";
        acheteur.getMain().push_back(std::move(carteUptr));
        return acheteur.getMain().back().get();
    }

    if(acheteur.getNextAcquiredToHand()){
        std::cout << "Carte acquise -> main (flag nextAcquiredToHand).\n";
        acheteur.getMain().push_back(std::move(carteUptr));
        acheteur.setNextAcquiredToHand(false);
        return acheteur.getMain().back().get();
    }

    if(acheteur.getNextAcquiredActionToTopDeck() && cartePtr->getType() == TypeCarte::Action){
        std::cout << "Carte action acquise -> dessus du deck (flag nextAcquiredActionToTopDeck).\n";
        acheteur.getDeck().insert(acheteur.getDeck().begin(), std::move(carteUptr));
        acheteur.setNextAcquiredActionToTopDeck(false);
        return acheteur.getDeck().front().get();
    }

    if(acheteur.getNextAcquiredToTopDeck()){
        std::cout << "Carte acquise -> dessus du deck (flag nextAcquiredToTopDeck).\n";
        acheteur.getDeck().insert(acheteur.getDeck().begin(), std::move(carteUptr));
        acheteur.setNextAcquiredToTopDeck(false);
        return acheteur.getDeck().front().get();
    }

    std::cout << "Carte achetee et mise en defausse par defaut.\n";
    acheteur.getDefausse().push_back(std::move(carteUptr));
    return acheteur.getDefausse().back().get();
}

Carte* Game::acheterGemmeDeFeu(Player& acheteur){
    if(gemmesDeFeu.empty()){ 
        std::cout << "Achat impossible : plus de gemmes de feu disponibles\n";
        return nullptr;}

    // Simple purchase logic (no global modifiers): check funds, pay, transfer ownership
    Carte* cartePtr = gemmesDeFeu[0].get();
    int prix = cartePtr->getCout();
    if (!godMode) {
        if(acheteur.getGold() < prix){
            std::cout << "Achat impossible : fonds insuffisants pour gemme de feu (cout=" << prix << ", or=" << acheteur.getGold() << ")\n";
            return nullptr;
        }
        // debit du joueur
        acheteur.modiffGold(-prix);
    } else {
        std::cout << "GodMode: achat gemme gratuit autorisé.\n";
    }

    // take ownership
    std::unique_ptr<Carte> carte = std::move(gemmesDeFeu[0]);
    // remove from gemmesDeFeu
    gemmesDeFeu.erase(gemmesDeFeu.begin());

    // decide destination based on godMode or player's flags
    if (godMode) {
        std::cout << "GodMode: gemme acquise -> main directement.\n";
        acheteur.getMain().push_back(std::move(carte));
        return acheteur.getMain().back().get();
    }

    if(acheteur.getNextAcquiredToHand()){
        std::cout << "Gemme de feu acquise -> main (flag nextAcquiredToHand).\n";
        acheteur.getMain().push_back(std::move(carte));
        acheteur.setNextAcquiredToHand(false);
        return acheteur.getMain().back().get();
    }

    if(acheteur.getNextAcquiredToTopDeck()){
        std::cout << "Gemme de feu acquise -> dessus du deck (flag nextAcquiredToTopDeck).\n";
        acheteur.getDeck().insert(acheteur.getDeck().begin(), std::move(carte));
        acheteur.setNextAcquiredToTopDeck(false);
        return acheteur.getDeck().front().get();
    }

    // Default: go to defausse
    std::cout << "Gemme de feu achetee et mise en defausse par defaut.\n";
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

void Game::setGodMode(bool val) {
    godMode = val;
    std::cout << "GodMode " << (godMode ? "activé" : "désactivé") << "\n";
    if (godMode) {
        // set each player to 1 HP
        for (auto &p : players) {
            int current = p.getHp();
            if (current != 1) {
                // set to 1 by adjusting heal/pv directly via soigner
                // compute difference
                int diff = 1 - current;
                p.soigner(diff);
            }
        }
    }
}

void Game::toggleGodMode() {
    setGodMode(!godMode);
}

bool Game::isGodMode() const {
    return godMode;
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

const Carte* Game::getModeleGemmeDeFeu() const {
    if (!gemmesDeFeu.empty()) {
        return gemmesDeFeu.front().get(); // retourne la première de la pile
    }
    return nullptr;
}

void Game::initialiserGemmesDeFeu() {
    gemmesDeFeu = CardLoader::loadFireGems();
}