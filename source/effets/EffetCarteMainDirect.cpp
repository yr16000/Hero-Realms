#include "../../include/effets/EffetCarteMainDirect.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>
#include <algorithm>

EffetCarteMainDirect::EffetCarteMainDirect(TypeCarte type)
    : Effet("Piocher et révéler une carte."), typeRecherche(type) {}

// Helper: draw one card from player's deck, reshuffle defausse into deck if needed.
static std::unique_ptr<Carte> drawOne(Player& p) {
    auto& deck = p.getDeck();
    auto& defausse = p.getDefausse();

    if(deck.empty()) {
        // if defausse has cards, move them to deck (simple reshuffle: move all)
        if(!defausse.empty()) {
            // move defausse into deck
            for(auto &c : defausse) {
                deck.push_back(std::move(c));
            }
            defausse.clear();
            p.melangerDeck();
        }
    }

    if(deck.empty()) return nullptr;

    // draw from back
    std::unique_ptr<Carte> drawn = std::move(deck.back());
    deck.pop_back();
    return drawn;
}

void EffetCarteMainDirect::activerEffet(Player& proprietaire, Game& /*game*/) {
    auto carte = drawOne(proprietaire);
    if(!carte) {
        std::cout << "Aucune carte à piocher.\n";
        return;
    }

    // Révéler
    std::cout << "Carte révélée : " << carte->getNom() << "\n";

    if(carte->getType() == typeRecherche) {
        std::cout << "Type correspondant -> ajout à la main.\n";
        proprietaire.getMain().push_back(std::move(carte));
    } else {
        std::cout << "Type non correspondant -> remise sur le dessus du deck.\n";
        // put on top of deck: push_back is top (we draw from back)
        proprietaire.getDeck().push_back(std::move(carte));
    }
}

std::string EffetCarteMainDirect::toString() const {
    return std::string("Piocher et révéler une carte; si elle est du type ") + ::to_string(typeRecherche) + 
        std::string(" elle va dans la main, sinon elle retourne sur le dessus du deck");
}
