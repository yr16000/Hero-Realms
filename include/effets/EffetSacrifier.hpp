#ifndef EFFETSACRIFIER_HPP
#define EFFETSACRIFIER_HPP

#include "../Effet.hpp"

class EffetSacrifier : public Effet{
private:
    int nombre;
    std::unique_ptr<Effet> effetSiSacrifie;
public:
    EffetSacrifier(int n, std::unique_ptr<Effet>&& effetSiSacrifie = nullptr);
    void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif
