#ifndef EFFETGAINPV_HPP
#define EFFETGAINPV_HPP
#include "../Effet.hpp"
#include "../Player.hpp"
#include "../Game.hpp"

class EffetGainPV : public Effet{
	private:
		int montant;
	public:
		EffetGainPV(int montant);
		void activerEffet(Player& proprietaire, Game& game) override;
		std::string toString() const override;
};

#endif

