#include <iostream>
#include <vector>
#include <memory>
#include "../include/CardLoader.hpp"
#include "../include/ui/CardRenderer.hpp"

int main() {
    using namespace ui;

    // 1. on charge le paquet
    std::vector<std::unique_ptr<Carte>> cartes = CardLoader::loadMarket();

    std::cout << "Taille du deck : " << cartes.size() << "\n\n";

    // 2. options d'affichage
    CardRenderer::Options opts;
    opts.width = 60;
    opts.artHeight = 0;
    opts.padding = 1;

    // 3. on affiche carte par carte
    for (std::size_t i = 0; i < cartes.size(); ++i) {
        std::cout << " CARTE " << (i + 1) << " / " << cartes.size()
                  << " : " << cartes[i]->getNom() << " \n";
        std::cout << CardRenderer::render(*cartes[i], opts) << "\n";

        // si ce n'est pas la dernière, on attend Entrée
        if (i + 1 < cartes.size()) {
            std::cout << "[Entrée pour la carte suivante] ";
            std::cout.flush();
            std::string dummy;
            std::getline(std::cin, dummy);
            std::cout << "\n";
        }
    }

    std::cout << " FIN DE L'AFFICHAGE \n";
    return 0;
}