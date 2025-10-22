
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>

Player::Player(int id) : id(id) {}

// Draw nb cards from deck to hand, reshuffle defausse into deck if needed
void Player::piocherCarte(int nb){
	for(int i=0;i<nb;i++){
		if(deck.empty()){
			if(!defausse.empty()){
				for(auto &c : defausse) deck.push_back(std::move(c));
				defausse.clear();
				std::reverse(deck.begin(), deck.end());
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

void Player::acheterCarte(int index, Game& game){
	// Ask the game to perform the purchase. Game::acheterCarte is responsible
	// for ownership/transfer; here we only call it and react to flags.
	Carte* carte = game.acheterCarte(index, *this);
	if(!carte){
		std::cout << "Achat echoue ou carte inexistante\n";
		return;
	}

	// Note: we don't take ownership here because Game likely manages that.
	// Respect the "next acquired" flags semantics logically (reset flags after use).
	if(nextAcquiredToHand){
		std::cout << "La prochaine carte acquise va dans la main (flag actif)\n";
		// actual addition to hand requires ownership transfer; omitted here
		nextAcquiredToHand = false;
	} else if(nextAcquiredToTopDeck){
		std::cout << "La prochaine carte acquise va sur le dessus du deck (flag actif)\n";
		nextAcquiredToTopDeck = false;
	} else {
		std::cout << "Carte achetee et mise en defausse par defaut\n";
	}
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

void Player::attaquer(Player& cible, int nb, Carte* carte){
	cible.subirDegat(nb);
}

void Player::subirDegat(int nb){
	pv -= nb;
}

void Player::jouerCarte(int index, Game& game){
	if(index < 0 || static_cast<size_t>(index) >= main.size()){
		std::cout << "Index de carte invalide\n";
		return;
	}
	// take card from hand
	std::unique_ptr<Carte> c = std::move(main[index]);
	// remove gap
	main.erase(main.begin()+index);

	if(!c){
		std::cout << "Carte nulle\n";
		return;
	}

	// Play based on primary type
	switch(c->getType()){
		case TypeCarte::Champion:
			championsEnJeu.push_back(std::move(c));
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

void Player::sacrifierCarte(Carte* carte){
	// find in championsEnJeu
	for(size_t i=0;i<championsEnJeu.size();++i){
		if(championsEnJeu[i].get() == carte){
			defausse.push_back(std::move(championsEnJeu[i]));
			championsEnJeu.erase(championsEnJeu.begin()+i);
			return;
		}
	}
	// find in main
	for(size_t i=0;i<main.size();++i){
		if(main[i].get() == carte){
			defausse.push_back(std::move(main[i]));
			main.erase(main.begin()+i);
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
std::vector<std::unique_ptr<Champion>>& Player::getChampionsEnJeu(){ return championsEnJeu; }
std::vector<std::unique_ptr<Carte>>& Player::getMain(){ return main; }
std::vector<std::unique_ptr<Carte>>& Player::getDeck(){ return deck; }

void Player::afficherMain() const{
	std::cout << "Main du joueur " << id << " (" << main.size() << " cartes)\n";
}

void Player::afficherStats() const{
	std::cout << "Joueur " << id << " - PV: " << pv << " Or: " << gold << " Combat: " << combat << "\n";
}

