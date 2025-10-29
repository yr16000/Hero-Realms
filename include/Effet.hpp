#ifndef EFFET_HPP
#define EFFET_HPP
#include <string>
#include <memory>

class Player;
class Game;

class Effet{
    private:
        std::string description;
    public:
        Effet(const std::string& desc);
        virtual ~Effet();
        virtual void activerEffet(Player& proprietaire, Game& game)=0;
        // Retourne une représentation lisible de l'effet pour l'affichage.
        virtual std::string toString() const;
};

#endif