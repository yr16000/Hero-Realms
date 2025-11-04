
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include "../include/Champion.hpp"
#include "../include/ui/CardRenderer.hpp"

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

void Player::incrFactionCount(Faction f){
	factionCount[f] += 1;
}

int Player::getFactionCount(Faction f) const{
	auto it = factionCount.find(f);
	if(it == factionCount.end()) return 0;
	return it->second;
}

void Player::acheterCarte(Game& game){
	// Let the player choose an index or 'F' to buy a Fire Gem. Use getline to
	// avoid mixing operator>> with getline in the main loop.
	int visible = game.getMarketSize();
	std::cout << "Entrez l'index de la carte à acheter (1-" << visible << ") ou F pour gemme de feu: ";
	std::string line;
	std::getline(std::cin, line);
	if(line.empty()){
		std::cout << "Achat annulé (entrée vide).\n";
		return;
	}
	for (auto &ch : line) ch = (char)std::toupper((unsigned char)ch);
	if(line == "F"){
		// delegate to gem purchase helper which prints result
		acheterGemmeDeFeu(game);
		return;
	}

	int inputIndex = 0;
	try {
		inputIndex = std::stoi(line);
	} catch(...) {
		std::cout << "Entrée invalide.\n";
		return;
	}
	// convert to zero-based
	int index = inputIndex - 1;

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

void Player::acheterGemmeDeFeu(Game& game){
	Carte* carte = game.acheterGemmeDeFeu(*this);
	if(!carte){
		std::cout << "Achat de gemme de feu echoue ou carte inexistante\n";
		return;
	}
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
	int indexDefenseurGarde = -1;
	// Check if any defending champions block the attack
	for(size_t i = 0; i < cible.getChampionsEnJeu().size(); ++i){
		Champion* champ = dynamic_cast<Champion*>(cible.getChampionsEnJeu()[i].get());
		if(champ && champ->getEstGarde()){
			attaqueBloquee = true;
			indexDefenseurGarde = static_cast<int>(i);
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
				// Move the defending champion's unique_ptr into the defender's defausse
				if(indexDefenseurGarde >= 0){
					cible.getDefausse().push_back(std::move(cible.getChampionsEnJeu()[indexDefenseurGarde]));
					cible.getChampionsEnJeu().erase(cible.getChampionsEnJeu().begin() + indexDefenseurGarde);
				}
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
			cible.getDefausse().push_back(std::move(cible.getChampionsEnJeu()[indexDefenseurGarde]));
			cible.getChampionsEnJeu().erase(cible.getChampionsEnJeu().begin() + indexDefenseurGarde);
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
				std::string tmp;
				std::getline(std::cin, tmp);
				int choix = 1;
				if(!tmp.empty()){
					try{ choix = std::stoi(tmp); } catch(...) { choix = 1; }
				}
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
			// record that an Action of this faction was played this turn
			defausse.push_back(std::move(c));
			break;
		case TypeCarte::Objet:
			// For objects: activate immediately (they may grant gold/combat/effects)
			// then send to defausse by default.
			{
				c->activer(*this, game);
				defausse.push_back(std::move(c));
			}
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

void Player::afficherMainDetaillee() const{
	auto const& main   = this->main;
	auto const& champs = this->championsEnJeu;

	ui::CardRenderer::Options opts;
	opts.width = 60;

	if (!main.empty()) {
		std::cout << "Votre main actuelle :\n";
		for (size_t i = 0; i < main.size(); ++i) {
			std::cout << "\n(" << i + 1 << ")\n";
			std::cout << ui::CardRenderer::render(*main[i], opts);
		}
	} else {
		std::cout << "Votre main est vide.\n";
	}

	if (!champs.empty()) {
		std::cout << "\nVos champions en jeu :\n";
		for (size_t i = 0; i < champs.size(); ++i) {
			std::cout << "\n[" << i + 1 << "]\n";
			std::cout << ui::CardRenderer::render(*champs[i], opts);
		}
	}
}

void Player::afficherChampionsEnJeu() const{
	std::cout << "Champions en jeu du joueur " << id << " (" << championsEnJeu.size() << " cartes)\n";
	for(size_t i=0;i<championsEnJeu.size();++i){
		std::cout << i+1 << ". " << championsEnJeu[i]->getNom() << "\n";
	}
}

void Player::afficherDefausse() const{
	auto const& defausse = this->defausse;
	ui::CardRenderer::Options opts;
	opts.width = 60;

	if (!defausse.empty()) {
		std::cout << "Votre défausse actuelle :\n";
		for (size_t i = 0; i < defausse.size(); ++i) {
			std::cout << "\n(" << i + 1 << ")\n";
			std::cout << ui::CardRenderer::render(*defausse[i], opts);
		}
	} else {
		std::cout << "Votre défausse est vide.\n";
	}
}

void Player::afficherSacrifices() const{
	std::cout << "Sacrifices du joueur " << id << " (" << sacrifices.size() << " cartes)\n";
	for(size_t i=0;i<sacrifices.size();++i){
		std::cout << i+1 << ". " << sacrifices[i]->getNom() << "\n";
	}
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
	for(auto &c : championsEnJeu){
		Champion* champ = dynamic_cast<Champion*>(c.get());
		if(champ){
			champ->setEstActiver(false);
		}
	}
	for(auto &c:main){
		defausse.push_back(std::move(c));
	}
	main.clear();
	piocherCarte(5);

	// compute faction counts at the start of the turn: include cards in
	// hand and champions currently in play so that faction effects can
	// be triggered even when the first card of the faction is played.
	factionCount.clear();
	for (const auto &carte : main) {
		factionCount[carte->getFaction()] += 1;
	}
	for (const auto &c : championsEnJeu) {
		factionCount[c->getFaction()] += 1;
	}

}
