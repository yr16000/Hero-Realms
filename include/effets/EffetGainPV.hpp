#ifndef EFFETGAINPV_HPP
#define EFFETGAINPV_HPP
#include "../Effet.hpp"

class EffetGainPV : public Effet{
	private:
		int montant;
	public:
		EffetGainPV(int montant);
		void activerEffet(Player& proprietaire, Game& game) override;
};

#endif

