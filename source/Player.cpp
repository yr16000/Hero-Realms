#include "../include/Player.hpp"
#include <iostream>

Player::Player(int id) : id(id) {}

void Player::piocherCarte(int nb){
	// stub: just print for now
	std::cout << "Joueur " << id << " pioche " << nb << " carte(s)\n";
}

void Player::acheterCarte(int index, Game& game){
	// stub
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
	// stub
}

void Player::sacrifierCarte(Carte* carte){
	// stub
}

bool Player::estVivant() const{ return pv>0; }

int Player::getHp() const{ return pv; }

int Player::getGold() const{ return gold; }

int Player::getAtk() const{ return combat; }

void Player::afficherMain() const{
	std::cout << "Main du joueur " << id << " (" << main.size() << " cartes)\n";
}

void Player::afficherStats() const{
	std::cout << "Joueur " << id << " - PV: " << pv << " Or: " << gold << " Combat: " << combat << "\n";
}

