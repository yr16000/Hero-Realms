#include "../include/Effet.hpp"

Effet::Effet(const std::string& desc) : description(desc) {}
Effet::~Effet() = default;
std::string Effet::toString() const { return description; }
