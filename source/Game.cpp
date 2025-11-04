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
#include "../include/ai/HeuristicAI.hpp"

Game::Game(){
    // creer deux joueurs
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


void Game::tourDeJeu(Player& joueur){
    std::cout << "Tour du joueur " << joueur.getId() << "\n";
}

void Game::afficherMarche() const {
    // 1) Concaténer Marché puis (si GodMode) la Pioche dans un unique vecteur
    std::vector<const Carte*> visibles;
    visibles.reserve(marche.size() + (godMode ? pioche.size() : 0));
    for (const auto& up : marche)  visibles.push_back(up.get());
    if (godMode) {
        for (const auto& up : pioche) visibles.push_back(up.get());
    }

    // 2) En-tête
    const size_t total = visibles.size();
    std::cout << " 🏪 Marché : " << total << " carte" << (total > 1 ? "s" : "");
    if (godMode) std::cout << "  |  GodMode: pioche visible";
    std::cout << "\n\n";
   

    // 3) Options d'affichage (2 par ligne, largeur 60)
    ui::CardRenderer::Options opts;
    opts.width = 60;
    opts.perRow = 2;         // 2 cartes par ligne
    opts.showIndices = true; // affiche [1], [2], ...

    // 4) Rendu unique (indices continus sur marché + pioche)
    try {
        std::cout << ui::CardRenderer::renderMultiple(visibles, opts) << "\n";
    } catch (...) {
        // Fallback : noms/ coûts au cas où
        for (size_t i = 0; i < visibles.size(); ++i) {
            const Carte* c = visibles[i];
            if (!c) continue;
            std::cout << "[" << (i + 1) << "] "
                      << c->getNom() << " (cout=" << c->getCout() << ")\n";
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
        // God mode: achat gratuit
        std::cout << "GodMode: achat gratuit autorisé.\n";
    }

    // Prendre possession de la carte
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
    acheteur.incrFactionCount(cartePtr->getFaction());

    // decide de la destination 
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

    // Gestion des gemmes de feu
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

    
    std::unique_ptr<Carte> carte = std::move(gemmesDeFeu[0]);
    // supprimer de gemmesDeFeu
    gemmesDeFeu.erase(gemmesDeFeu.begin());

    // destination achat GodMode
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

    // Par default: aller en defausse
    std::cout << "Gemme de feu achetee et mise en defausse par defaut.\n";
    acheteur.getDefausse().push_back(std::move(carte));
    return acheteur.getDefausse().back().get();
}

void Game::ajouterCarteMarche(std::unique_ptr<Carte> carte){
    marche.push_back(std::move(carte));
}

void Game::melangerPioche(){
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pioche.begin(), pioche.end(), g);
}

void Game::afficherEtatJoueurs() const{
    for(const auto &p : players) p.afficherStats();
}

bool Game::estTerminee() const{ 
    for(const auto &p : players) {
        if (!p.estVivant()) {
            return true;
        }
    }
    return false;
}

void Game::afficherGagnant() const {
    int survivants = 0;
    int gagnantId = -1;

    for (const auto& p : players) {
        if (p.estVivant()) {
            survivants++;
            gagnantId = p.getId();
        }
    }

    if (survivants == 1) {
        std::cout << "Le joueur " << (gagnantId + 1) << " a gagné la partie !\n";
    } else {
        // couvre : 0 survivants (double KO) ET >1 survivants (partie quittée/interrompue)
        std::cout << "Match nul ! Aucun gagnant.\n";
    }
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
        // mettre les PV de tous les joueurs à 1
        for (auto &p : players) {
            int current = p.getHp();
            if (current != 1) {
               
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

int Game::getMarketSize() const {
    int marcheSize = (int)marche.size();
    if (godMode) return marcheSize + (int)pioche.size();
    return marcheSize;
}

const std::vector<std::unique_ptr<Carte>>& Game::getMarche() const {
    return marche;
}

void Game::initialiserMarche() {
    //Extraire les 5 premières cartes de la pioche pour le marché
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
    //premier joueur pioche 3 cartes, second joueur pioche 5 cartes au début de la partie
    player1.piocherCarte(3);
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

// Methodes IA
void Game::setAIPlayer(std::unique_ptr<HeuristicAI> ai, int playerIndex) {
    if (playerIndex >= 0 && playerIndex < static_cast<int>(players.size())) {
        aiPlayer = std::move(ai);
        aiPlayerIndex = playerIndex;
        std::cout << "IA activée pour le joueur " << (playerIndex + 1) << "\n";
    }
}

HeuristicAI* Game::getAIPlayer() const {
    return aiPlayer.get();
}

int Game::getAIPlayerIndex() const {
    return aiPlayerIndex;
}

bool Game::isAIPlayer(int playerIndex) const {
    return aiPlayerIndex == playerIndex && aiPlayer != nullptr;
}

std::vector<std::unique_ptr<Carte>>& Game::getFireGems() { return gemmesDeFeu; }