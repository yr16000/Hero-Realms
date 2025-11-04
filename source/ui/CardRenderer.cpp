#include "../../include/ui/CardRenderer.hpp"

#include <sstream>
#include <algorithm>
#include <iomanip>

#include "../../include/Champion.hpp"
#include "../../include/Action.hpp"
#include "../../include/Objet.hpp"
#include "../../include/Enums.h"

using namespace ui;

// conversions déjà dispo dans Enums.h
static std::string toStr(TypeCarte t) { return to_string(t); }
static std::string toStr(Faction f)   { return to_string(f); }

/* ---------------- ANSI couleurs ---------------- */
namespace Ansi {
    static constexpr const char* RESET  = "\033[0m";
    static constexpr const char* BOLD   = "\033[1m";
    static constexpr const char* RED    = "\033[31m";
    static constexpr const char* GREEN  = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE   = "\033[34m";
    static constexpr const char* GRAY   = "\033[90m";
}
static const char* colorForFaction(Faction f) {
    using namespace Ansi;
    switch (f) {
        case Faction::Sauvage:   return GREEN;
        case Faction::Necros:    return RED;
        case Faction::Guilde:    return BLUE;
        case Faction::Imperiale: return YELLOW;
        default:                 return Ansi::RESET;
    }
}

/* ---------------- UTF-8 + ANSI helpers ---------------- */
static int utf8DisplayWidthAnsi(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size();) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        // séquence ANSI ?
        if (c == 0x1B && i + 1 < s.size() && s[i+1] == '[') {
            i += 2;
            while (i < s.size() && s[i] != 'm') ++i;
            if (i < s.size()) ++i; // consommer 'm'
            continue;
        }
        // nouveau point de code UTF-8 = octet non "continuation"
        if ((c & 0xC0) != 0x80) ++w;
        ++i;
    }
    return w;
}

static std::string padRightAnsiUtf8(const std::string& s, int width) {
    int vis = utf8DisplayWidthAnsi(s);
    if (vis >= width) return s;
    return s + std::string(width - vis, ' ');
}

std::string CardRenderer::padRight(const std::string& s, int width) {
    return padRightAnsiUtf8(s, width);
}

static void writeContentLine(std::ostringstream& out, const std::string& content, int inner) {
    out << "|" << padRightAnsiUtf8(content, inner) << "|\n";
}

std::vector<std::string> CardRenderer::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> res;
    if (maxWidth <= 0) return res;
    std::istringstream iss(text);
    std::string word, line;
    while (iss >> word) {
        int need = (int)utf8DisplayWidthAnsi(word) + (line.empty() ? 0 : 1);
        if (utf8DisplayWidthAnsi(line) + need <= maxWidth) {
            if (!line.empty()) line += " ";
            line += word;
        } else {
            if (!line.empty()) res.push_back(line);
            if (utf8DisplayWidthAnsi(word) > maxWidth) {
                int accum = 0, start = 0;
                for (size_t i = 0; i < word.size(); ++i) {
                    unsigned char c = (unsigned char)word[i];
                    if ((c & 0xC0) != 0x80) {
                        if (accum == maxWidth) {
                            res.push_back(word.substr(start, i - start));
                            start = (int)i;
                            accum = 0;
                        }
                        ++accum;
                    }
                }
                if (start < (int)word.size()) res.push_back(word.substr(start));
                line.clear();
            } else {
                line = word;
            }
        }
    }
    if (!line.empty()) res.push_back(line);
    return res;
}

/* ---------------- Rendu 1 carte ---------------- */
std::string CardRenderer::render(const Carte& carte, const Options& opts) {
    using namespace Ansi;
    const int inner = opts.width - 2;
    const int textWidth = inner - 2;
    const char* facColor = colorForFaction(carte.getFaction());
    std::ostringstream out;

    // Haut
    out << "┏" << std::string(inner, '-') << "┓\n";

    // Titre : nom (gras + couleur) + coût (neutre)
    {
        const std::string leftPlain  = " " + carte.getNom();
        const std::string rightPlain = "(Coût: " + std::to_string(carte.getCout()) + ")";
        int spaces = inner - utf8DisplayWidthAnsi(leftPlain) - utf8DisplayWidthAnsi(rightPlain);
        if (spaces < 1) spaces = 1;

        std::string leftColored = " " + std::string(facColor) + BOLD + carte.getNom() + RESET;
        writeContentLine(out, leftColored + std::string(spaces, ' ') + rightPlain, inner);
    }

    // Type / Faction : labels gris, valeur faction colorée
    {
        std::ostringstream line;
        line << Ansi::GRAY << " Type : " << Ansi::RESET << toStr(carte.getType())
             << "    " << Ansi::GRAY << "Faction : " << Ansi::RESET
             << facColor << toStr(carte.getFaction()) << Ansi::RESET;
        writeContentLine(out, line.str(), inner);
    }

    // Types secondaire / tertiaire (labels gris)
    {
        auto ts = carte.getTypeSecondaire();
        auto tt = carte.getTypeTertiaire();

        if (ts != TypeSecondaire::Aucun) {
            std::ostringstream l2;
            l2 << Ansi::GRAY << " Type secondaire : " << Ansi::RESET << to_string(ts);
            writeContentLine(out, l2.str(), inner);
        }

        if (tt != TypeTertiaire::Aucun) {
            std::ostringstream l3;
            l3 << Ansi::GRAY << " Type tertiaire  : " << Ansi::RESET << to_string(tt);
            writeContentLine(out, l3.str(), inner);
        }
    }

    // PV/Garde
    if (carte.getType() == TypeCarte::Champion) {
        if (auto champ = dynamic_cast<const Champion*>(&carte)) {
            std::ostringstream line;
            line << " PV : " << champ->getPv()
                 << "    Garde : " << (champ->getEstGarde() ? "oui" : "non");
            writeContentLine(out, line.str(), inner);
        } else {
            writeContentLine(out, "", inner);
        }
    } else {
        writeContentLine(out, "", inner);
    }

    // Séparateur
    out << "┣" << std::string(inner, '-') << "┫\n";

    // Effets
    writeContentLine(out, " Effets :", inner);

    if (carte.getEffetsCarte().empty()) {
        writeContentLine(out, "  - (aucun)", inner);
    } else {
        for (const auto& eff : carte.getEffetsCarte()) {
            if (!eff) continue;
            auto txt = eff->toString();
            auto wrapped = wrapText(txt, textWidth - 3);
            bool first = true;
            for (auto& w : wrapped) {
                std::string line = (first ? "  - " : "    ") + w;
                writeContentLine(out, line, inner);
                first = false;
            }
        }
    }

    // Effets de sacrifice (Action / Objet)
    if (carte.getType() == TypeCarte::Action) {
        if (auto action = dynamic_cast<const Action*>(&carte)) {
            const auto& sac = action->getEffetsSacrifice();
            if (!sac.empty()) {
                writeContentLine(out, "", inner);
                writeContentLine(out, " Effets de sacrifice :", inner);
                for (const auto& eff : sac) {
                    if (!eff) continue;
                    auto txt = eff->toString();
                    auto wrapped = wrapText(txt, textWidth - 3);
                    bool first = true;
                    for (auto& w : wrapped) {
                        std::string line = (first ? "  - " : "    ") + w;
                        writeContentLine(out, line, inner);
                        first = false;
                    }
                }
            }
        }
    } else if (carte.getType() == TypeCarte::Objet) {
        if (auto obj = dynamic_cast<const Objet*>(&carte)) {
            const auto& sac = obj->getEffetsSacrifice();
            if (!sac.empty()) {
                writeContentLine(out, "", inner);
                writeContentLine(out, " Effets de sacrifice :", inner);
                for (const auto& eff : sac) {
                    if (!eff) continue;
                    auto txt = eff->toString();
                    auto wrapped = wrapText(txt, textWidth - 3);
                    bool first = true;
                    for (auto& w : wrapped) {
                        std::string line = (first ? "  - " : "    ") + w;
                        writeContentLine(out, line, inner);
                        first = false;
                    }
                }
            }
        }
    }

    // Effets de faction
    if (!carte.getEffetsFaction().empty()) {
        writeContentLine(out, "", inner);
        writeContentLine(out, " Effets de faction :", inner);
        for (const auto& eff : carte.getEffetsFaction()) {
            if (!eff) continue;
            auto txt = eff->toString();
            auto wrapped = wrapText(txt, textWidth - 3);
            bool first = true;
            for (auto& w : wrapped) {
                std::string line = (first ? "  - " : "    ") + w;
                writeContentLine(out, line, inner);
                first = false;
            }
        }
    }

    // Bas
    out << "┗" << std::string(inner, '-') << "┛\n";

    return out.str();
}

/* 1 carte -> lignes */
std::vector<std::string> CardRenderer::renderToLines(const Carte& carte, const Options& opts) {
    std::string block = render(carte, opts);
    std::vector<std::string> lines;
    lines.reserve(32);
    std::istringstream iss(block);
    std::string L;
    while (std::getline(iss, L)) lines.push_back(L);
    return lines;
}

/* Concaténer N colonnes — ANSI/UTF-8 safe pour les cartes hautes */
static int visibleWidthOfCardLine(const std::string& line) {
    return utf8DisplayWidthAnsi(line);
}

std::string CardRenderer::joinColumns(const std::vector<std::vector<std::string>>& cols, int spacing) {
    if (cols.empty()) return "";
    size_t h = 0, w = cols.size();
    for (const auto& c : cols) h = std::max(h, c.size());

    // largeur visible d’une carte (toutes les lignes d’une carte ont la même largeur visible)
    int colWidthVis = cols[0].empty() ? 0 : visibleWidthOfCardLine(cols[0][0]);
    std::string gap(spacing, ' ');
    std::ostringstream out;

    for (size_t row = 0; row < h; ++row) {
        for (size_t c = 0; c < w; ++c) {
            if (c) out << gap;
            if (row < cols[c].size()) {
                // on écrit la ligne, puis on complète au besoin pour s'assurer que
                // la largeur visible atteigne colWidthVis (utile si présence d'ANSI)
                const std::string& L = cols[c][row];
                int vis = visibleWidthOfCardLine(L);
                out << L;
                if (vis < colWidthVis) out << std::string(colWidthVis - vis, ' ');
            } else {
                out << std::string(colWidthVis, ' ');
            }
        }
        if (row + 1 < h) out << "\n";
    }
    return out.str();
}

/* Ligne d'index */
std::string CardRenderer::buildIndexRow(size_t startIndex, size_t count, int colWidth, int spacing) {
    std::ostringstream out;
    std::string gap(spacing, ' ');
    for (size_t i = 0; i < count; ++i) {
        if (i) out << gap;
        std::string tag = "[" + std::to_string(startIndex + i + 1) + "]";
        if ((int)tag.size() < colWidth) tag += std::string(colWidth - (int)tag.size(), ' ');
        out << tag;
    }
    return out.str();
}

/* Rendu multiple */
std::string CardRenderer::renderMultiple(const std::vector<const Carte*>& cards, const Options& opts) {
    if (cards.empty()) return "";

    const int colWidth = opts.width; // largeur totale par carte (visible)
    const int spacing  = 2;          // espaces entre colonnes
    std::ostringstream out;

    for (size_t i = 0; i < cards.size(); i += (size_t)opts.perRow) {
        std::vector<std::vector<std::string>> cols;
        cols.reserve((size_t)opts.perRow);

        size_t batchCount = 0;
        for (int k = 0; k < opts.perRow; ++k) {
            size_t idx = i + (size_t)k;
            if (idx < cards.size() && cards[idx]) {
                cols.push_back(renderToLines(*cards[idx], opts));
                ++batchCount;
            } else {
                // colonne vide (pour compléter la ligne si impair)
                std::vector<std::string> empty;
                empty.push_back(std::string(colWidth, ' ')); // ligne vide brute
                cols.push_back(std::move(empty));
            }
        }

        if (opts.showIndices && batchCount > 0) {
            out << buildIndexRow(i, batchCount, colWidth, spacing) << "\n";
        }

        out << joinColumns(cols, spacing);

        if (i + (size_t)opts.perRow < cards.size()) out << "\n";
    }

    return out.str();
}