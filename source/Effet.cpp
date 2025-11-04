#include "../include/Effet.hpp"
// Constructeur et destructeur de la classe Effet
Effet::Effet(const std::string& desc) : description(desc) {}
Effet::~Effet() = default;
std::string Effet::toString() const { return description; }
