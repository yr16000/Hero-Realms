#ifndef EFFETOPTIONNEL_HPP
#define EFFETOPTIONNEL_HPP

#include "../Effet.hpp"
#include <memory>

class EffetOptionnel : public Effet {
    private:
        std::unique_ptr<Effet> effet;
    public:
        EffetOptionnel(std::unique_ptr<Effet> effet);  // prend possession de l'effet
        void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif