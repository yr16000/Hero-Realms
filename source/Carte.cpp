#include "../include/Carte.hpp"
#include <iostream>
#include "../include/ui/CardRenderer.hpp"

Carte::Carte(const std::string& nom, int cout, const Faction faction, TypeCarte typePrimaire,
	std::vector<std::unique_ptr<Effet>>&& effetsCarte,
	std::vector<std::unique_ptr<Effet>>&& effetsFaction,
	const SousTypeCarte typeSecondaire,
	const SousTypeCarte typeTertiaire)
	: nom(nom), cout(cout), faction(faction), typePrimaire(typePrimaire), typeSecondaire(typeSecondaire), typeTertiaire(typeTertiaire),
	  effetCarte(std::move(effetsCarte)), effetFaction(std::move(effetsFaction)), utilisee(false) {}

Carte::~Carte() = default;

void Carte::resetUtilisation() { utilisee = false; }

bool Carte::estUtilisee() const { return utilisee; }

std::string Carte::getNom() const { return nom; }

int Carte::getCout() const { return cout; }

const std::vector<std::unique_ptr<Effet>>& Carte::getEffetsCarte() const { return effetCarte; }

const std::vector<std::unique_ptr<Effet>>& Carte::getEffetsFaction() const { return effetFaction; }

SousTypeCarte Carte::getTypeSecondaire() const { return typeSecondaire; }

SousTypeCarte Carte::getTypeTertiaire() const { return typeTertiaire; }

TypeCarte Carte::getType() const { return typePrimaire; }

void Carte::afficherCarte() const {
	std::cout << toString(60) << std::endl;
}

void Carte::onSacrifice(Player& /*proprietaire*/, Game& /*game*/) {
	// default: nothing
}

Faction Carte::getFaction() const {
    return faction;
}

std::string Carte::toString(unsigned width) const {
	ui::CardRenderer::Options opts;
	opts.width = static_cast<int>(width);
	return ui::CardRenderer::render(*this, opts);
}