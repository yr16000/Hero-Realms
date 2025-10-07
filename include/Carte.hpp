#ifndef CARTE_HPP
#define CARTE_HPP
#include "Effet.hpp"
#include <string>
#include <vector>
#include <memory>

enum class TypeCarte {};

class Carte{
    private:
        std::string nom;
        int cout;
        TypeCarte typePrimaire;
        std::vector<std::unique_ptr<Effet>> effetCarte;

    public:

};

#endif