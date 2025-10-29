#include <iostream>
#include <vector>
#include <memory>

#include "../include/Action.hpp"
#include "../include/Champion.hpp"
#include "../include/effets/EffetGainCombat.hpp"
#include "../include/effets/EffetPiocherCarte.hpp"
#include "../include/effets/EffetGainPvParChampion.hpp"
#include "../include/effets/EffetChampionDefausseTopDeck.hpp"

int main(){
    using namespace std;
    vector<unique_ptr<Effet>> effs1;
    effs1.push_back(make_unique<EffetGainCombat>(5));
    effs1.push_back(make_unique<EffetGainPvParChampion>(2));

    Action closeRanks("Close Ranks", 3, Faction::Imperiale, false, move(effs1));

    vector<unique_ptr<Effet>> effs2;
    effs2.push_back(make_unique<EffetChampionDefausseTopDeck>());
    vector<unique_ptr<Effet>> faction2;
    faction2.push_back(make_unique<EffetPiocherCarte>(1));
    Champion arkus("Arkus, Imperial Dragon", 8, Faction::Imperiale, 6, true, move(effs2), move(faction2));

    cout << closeRanks.toString(60) << endl;
    cout << arkus.toString(60) << endl;
    return 0;
}
