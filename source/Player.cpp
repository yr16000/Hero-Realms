
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include "../include/Champion.hpp"

static std::mt19937 rng((std::random_device())());

Player::Player(int id) : id(id) {}


// Draw nb cards from deck to hand, reshuffle defausse into deck if needed
void Player::piocherCarte(int nb){
	for(int i=0;i<nb;i++){
		if(deck.empty()){
			if(!defausse.empty()){
				// move all cards from defausse to deck
				for(auto &c : defausse) deck.push_back(std::move(c));
				defausse.clear();
				melangerDeck();
			}
		}
		if(deck.empty()){
			std::cout << "Joueur "<<id<<" : plus de cartes à piocher\n";
			break;
		}
		main.push_back(std::move(deck.back()));
		deck.pop_back();
	}
}

void Player::melangerDeck(){
	if(deck.empty()) return;
	std::shuffle(deck.begin(), deck.end(), rng);
}

void Player::melangerDefausse(){
	if(defausse.empty()) return;
	std::shuffle(defausse.begin(), defausse.end(), rng);
}

void Player::acheterCarte(int index, Game& game){
	// Ask the game to perform the purchase; Game will transfer ownership into
	// the appropriate player container and return a pointer to the acquired card.
	Carte* carte = game.acheterCarte(index, *this);
	if(!carte){
		std::cout << "Achat echoue ou carte inexistante\n";
		return;
	}

	// The transfer and flag consumption are handled inside Game::acheterCarte.
	std::cout << "Achat reussi: " << carte->getNom() << " (cout=" << carte->getCout() << ")\n";
}

void Player::modiffGold(int nb){
	gold += nb;
}

void Player::modiffCombat(int nb){
	combat += nb;
}

void Player::modiffHeal(int nb){
	heal += nb;
}


void Player::soigner(int nb){
	pv += nb;
}

void Player::attaquer(Player& cible, Champion* carte){
	bool attaqueBloquee = false;
	// Check if any defending champions block the attack
	for(auto &defenseur : cible.getChampionsEnJeu()){
		Champion* champ = dynamic_cast<Champion*>(defenseur.get());
		if(champ && champ->getEstGarde()){
			attaqueBloquee = true;
			break;
		}

	}

	if(attaqueBloquee){
		if(carte){
			Champion* champAtt = dynamic_cast<Champion*>(carte);
			if(champAtt && champAtt->getEstGarde()){
				std::cout << "Attaque bloquée par le champion garde. Attaque autorisée contre ce champion.\n";
				if(combat < champAtt->getPv()){
					std::cout << "Pas assez de points de combat pour attaquer avec ce champion.\n";
					return;
				}
				combat -= champAtt->getPv();
				champAtt->subirDegat(champAtt->getPv());
			} else {
				std::cout << "Attaque bloquée par un champion garde. Choisissez un champion garde pour attaquer.\n";
				return;
			}
		} else {
			std::cout << "Attaque bloquée par un champion garde. Choisissez un champion garde pour attaquer.\n";
			return;
		}
	}

	 else {
		if(carte){
			Champion* champAtt = dynamic_cast<Champion*>(carte);
			if(combat < champAtt->getPv()){
					std::cout << "Pas assez de points de combat pour attaquer avec ce champion.\n";
					return;
				}
			combat -= champAtt->getPv();
			champAtt->subirDegat(champAtt->getPv());
		}
		cible.subirDegat(combat);
		combat = 0;
	}
}

void Player::subirDegat(int nb){
	pv -= nb;
}

void Player::jouerCarte(int index, Game& game){
	if(index < 1 || static_cast<size_t>(index) > main.size()){
		std::cout << "Index de carte invalide\n";
		return;
	}
	// take card from hand
	std::unique_ptr<Carte> c = std::move(main[index-1]);
	// remove gap
	main.erase(main.begin()+index-1);

	if(!c){
		std::cout << "Carte nulle\n";
		return;
	}

	// Play based on primary type
	switch(c->getType()){
		case TypeCarte::Champion:
			{
				// When playing a champion from hand, let the player choose to simply
				// put it into play or to put it into play and activate it immediately.
				std::cout << "Vous jouez un champion: " << c->getNom() << "\n";
				std::cout << "1) Mettre en jeu seulement\n2) Mettre en jeu et activer maintenant\nChoix (1/2): ";
				int choix = 1;
				if(!(std::cin >> choix)) choix = 1;
				// Place champion into play
				championsEnJeu.push_back(std::move(c));
				if(choix == 2){
					Champion* champ = dynamic_cast<Champion*>(championsEnJeu.back().get());
					if(champ) champ->activer(*this, game);
				}
			}
			break;
		case TypeCarte::Action:
			// activate then send to defausse
			c->activer(*this, game);
			defausse.push_back(std::move(c));
			break;
		case TypeCarte::Objet:
			// put into play (treat like champion for now)
			defausse.push_back(std::move(c));
			break;
		default:
			defausse.push_back(std::move(c));
			break;
	}
}

void Player::sacrifierCarte(Carte* carte, Game& game){
	if(!carte) return;
	// Call card-specific sacrifice behavior
	carte->onSacrifice(*this, game);
	// find in championsEnJeu
	for(size_t i=0;i<championsEnJeu.size();++i){
		if(championsEnJeu[i].get() == carte){
			sacrifices.push_back(std::move(championsEnJeu[i]));
			championsEnJeu.erase(championsEnJeu.begin()+i);
			return;
		}
	}
	// find in main
	for(size_t i=0;i<main.size();++i){
		if(main[i].get() == carte){
			sacrifices.push_back(std::move(main[i]));
			main.erase(main.begin()+i);
			return;
		}
	}
	// find in defausse
	for(size_t i=0;i<defausse.size();++i){
		if(defausse[i].get() == carte){
			sacrifices.push_back(std::move(defausse[i]));
			defausse.erase(defausse.begin()+i);
			return;
		}
	}
}

bool Player::estVivant() const{ return pv>0; }

int Player::getHp() const{ return pv; }

int Player::getGold() const{ return gold; }

int Player::getAtk() const{ return combat; }

int Player::getId() const { return id; }

bool Player::getNextAcquiredToTopDeck() const { 
    return nextAcquiredToTopDeck; 
}
void Player::setNextAcquiredToTopDeck(bool val){ 
    nextAcquiredToTopDeck = val; 
}
bool Player::getNextAcquiredToHand() const { return nextAcquiredToHand; }
void Player::setNextAcquiredToHand(bool val){ nextAcquiredToHand = val; }
bool Player::getNextAcquiredActionToTopDeck() const { return this->nextAcquiredActionToTopDeck; }
void Player::setNextAcquiredActionToTopDeck(bool val) { this->nextAcquiredActionToTopDeck = val; }

std::vector<std::unique_ptr<Carte>>& Player::getDefausse(){ return defausse; }
std::vector<std::unique_ptr<Carte>>& Player::getChampionsEnJeu(){ return championsEnJeu; }
std::vector<std::unique_ptr<Carte>>& Player::getMain(){ return main; }
std::vector<std::unique_ptr<Carte>>& Player::getDeck(){ return deck; }

void Player::afficherMain() const{
	std::cout << "Main du joueur " << id << " (" << main.size() << " cartes)\n";
}

void Player::afficherChampionsEnJeu() const{
	std::cout << "Champions en jeu du joueur " << id << " (" << championsEnJeu.size() << " cartes)\n";
	for(size_t i=0;i<championsEnJeu.size();++i){
		std::cout << i+1 << ". " << championsEnJeu[i]->getNom() << "\n";
	}
}

void Player::afficherDefausse() const{
	std::cout << "Defausse du joueur " << id << " (" << defausse.size() << " cartes)\n";
}

void Player::afficherStats() const{
	std::cout << "Joueur " << id << " - PV: " << pv << " Or: " << gold << " Combat: " << combat << "\n";
}

void Player::resetPourNouveauTour(){
	combat = 0;
	gold = 0;
	heal = 0;
	nextAcquiredToHand = false;
	nextAcquiredToTopDeck = false;
	nextAcquiredActionToTopDeck = false;
}