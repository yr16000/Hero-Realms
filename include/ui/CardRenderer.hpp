#pragma once
#include <string>
#include <vector>

class Carte;

namespace ui {

// Options séparées pour autoriser Options{} en valeur par défaut
struct RenderOptions {
    int  width   = 58;   // largeur intérieure + 2 bords => total = width
    int  perRow  = 2;    // nb de cartes par ligne
    bool showIndices = true; // affiche [1] [2] ... au-dessus de chaque carte
};

struct CardRenderer {
    using Options = RenderOptions;

    // Rendu d'une seule carte (identique à ton rendu actuel ASCII)
    static std::string render(const Carte& carte, const Options& opts = Options{});

    // Rendu d'un groupe de cartes, en perRow colonnes (ex: marché 2 par ligne)
    // - cards : vecteur de pointeurs (ou références, au choix), nullptr possible en fin de ligne
    static std::string renderMultiple(const std::vector<const Carte*>& cards,
                                      const Options& opts = Options{});

    // Utilitaires
    static std::string padRight(const std::string& s, int width);
    static std::vector<std::string> wrapText(const std::string& text, int maxWidth);

private:
    // Transforme 1 carte -> lignes (pour composer plusieurs colonnes)
    static std::vector<std::string> renderToLines(const Carte& carte, const Options& opts);

    // Concatène N colonnes (même largeur), en normalisant la hauteur
    static std::string joinColumns(const std::vector<std::vector<std::string>>& columns,
                                   int spacing = 2);

    // Construit une "ligne d'index" pour une ligne de cartes (ex: "[1]   [2]")
    static std::string buildIndexRow(size_t startIndex, size_t count, int colWidth, int spacing);
};

} // namespace ui
