#ifndef EFFETPIOCHERCARTE_HPP
#define EFFETPIOCHERCARTE_HPP
#include "../Effet.hpp"

class EffetPiocherCarte : public Effet{
	private:
		int nb;
	public:
		EffetPiocherCarte(int nb);
		void activerEffet(Player& proprietaire, Game& game) override;
};

#endif

