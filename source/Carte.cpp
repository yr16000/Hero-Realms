#include "../include/Carte.hpp"
#include <iostream>

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

SousTypeCarte Carte::getTypeSecondaire() const { return typeSecondaire; }

SousTypeCarte Carte::getTypeTertiaire() const { return typeTertiaire; }

TypeCarte Carte::getType() const { return typePrimaire; }

void Carte::afficherCarte() const {
	std::cout << "Carte: " << nom << " Cout: " << cout << "\n";
}

Faction Carte::getFaction() const {
    return faction;
}