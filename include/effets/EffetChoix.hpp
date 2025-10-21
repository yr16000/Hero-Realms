#ifndef EFFETCHOIX_HPP
#define EFFETCHOIX_HPP

#include "../Effet.hpp"
#include <vector>
#include <memory>

class EffetChoix : public Effet{
private:
    std::vector<std::unique_ptr<Effet>> options;
public:
    EffetChoix(std::vector<std::unique_ptr<Effet>>&& opts);
    void activerEffet(Player& proprietaire, Game& game) override;
};

#endif
