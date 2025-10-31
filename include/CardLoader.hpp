#pragma once
#ifndef CARDLOADER_HPP
#define CARDLOADER_HPP

#include <string>
#include <vector>
#include <memory>
#include "Enums.h"

// Forward declaration to avoid heavy includes in this header.
class Carte;

namespace CardLoader {
    // Charge le "base set" codé en dur (utilisé si tu veux une initialisation statique)
    std::vector<std::unique_ptr<Carte>> loadPioche();

    std::vector<std::unique_ptr<Carte>> loadFireGems(int count = 16);

    std::vector<std::unique_ptr<Carte>> loadPersonalDeck();

}

#endif // CARDLOADER_HPP
