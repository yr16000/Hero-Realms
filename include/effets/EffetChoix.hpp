#ifndef EFFETCHOIX_HPP
#define EFFETCHOIX_HPP

#include "../Effet.hpp"
#include <vector>
#include <memory>
#include "../ai/HeuristicAI.hpp"

class EffetChoix : public Effet{
private:
    std::vector<std::unique_ptr<Effet>> options;
public:
    EffetChoix(std::vector<std::unique_ptr<Effet>>&& opts);
    void activerEffet(Player& proprietaire, Game& game) override;
    std::string toString() const override;
};

#endif
