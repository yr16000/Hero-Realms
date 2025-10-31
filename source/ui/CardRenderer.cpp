#include "../../include/ui/CardRenderer.hpp"

#include <sstream>
#include <algorithm>
#include <iomanip>

#include "../../include/Champion.hpp"
#include "../../include/Enums.h"

using namespace ui;

static std::string toStr(TypeCarte t) {
    return to_string(t); // tu avais déjà to_string dans Enums.h
}

static std::string toStr(Faction f) {
    return to_string(f);
}

std::string CardRenderer::padRight(const std::string& s, int width) {
    if ((int)s.size() >= width) return s.substr(0, width);
    return s + std::string(width - (int)s.size(), ' ');
}

// écrit UNE ligne de contenu entre les deux barres verticales, sans déborder
static void writeContentLine(std::ostringstream& out, const std::string& content, int inner) {
    out << "|" << CardRenderer::padRight(content, inner) << "|\n";
}

std::vector<std::string> CardRenderer::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> res;
    if (maxWidth <= 0) return res;
    std::istringstream iss(text);
    std::string word;
    std::string line;
    while (iss >> word) {
        int need = (int)word.size() + (line.empty() ? 0 : 1);
        if ((int)line.size() + need <= maxWidth) {
            if (!line.empty()) line += " ";
            line += word;
        } else {
            if (!line.empty()) res.push_back(line);
            if ((int)word.size() > maxWidth) {
                for (size_t i = 0; i < word.size(); i += maxWidth)
                    res.push_back(word.substr(i, maxWidth));
                line.clear();
            } else {
                line = word;
            }
        }
    }
    if (!line.empty()) res.push_back(line);
    return res;
}

std::string CardRenderer::render(const Carte& carte, const Options& opts) {
    const int inner = opts.width - 2;      // largeur "utile" entre les |
    const int textWidth = inner - 2;       // quand on met "  - " devant
    std::ostringstream out;

    // 1. ligne du haut
    out << "+" << std::string(inner, '-') << "+\n";

    // 2. ligne titre : nom + coût
    {
        std::string name = " " + carte.getNom();
        std::string cost = "(Coût: " + std::to_string(carte.getCout()) + ")";
        int spaces = inner - (int)name.size() - (int)cost.size();
        if (spaces < 1) spaces = 1;
        out << "|" << name << std::string(spaces, ' ') << cost << " |\n";
    }

    // 3. ligne type / faction
    {
        std::ostringstream line;
        line << " Type : " << toStr(carte.getType())
             << "    Faction : " << toStr(carte.getFaction());
        writeContentLine(out, line.str(), inner);
    }

    // afficher types secondaire / tertiaire seulement s'ils existent
{
    // il faut que ta classe Carte ait bien ces getters :
    // getTypeSecondaire() et getTypeTertiaire()
    {
    auto ts = carte.getTypeSecondaire();
    auto tt = carte.getTypeTertiaire();

    if (ts != TypeSecondaire::Aucun) {
        std::ostringstream l2;
        l2 << " Type secondaire : " << to_string(ts);
        writeContentLine(out, l2.str(), inner);
    }

    if (tt != TypeTertiaire::Aucun) {
        std::ostringstream l3;
        l3 << " Type tertiaire  : " << to_string(tt);
        writeContentLine(out, l3.str(), inner);
    }
}

}

    // 4. ligne PV/Garde pour Champion
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

    // 5. ligne de séparation
    out << "+" << std::string(inner, '-') << "+\n";

    // 6. Effets normaux
    writeContentLine(out, " Effets :", inner);

    if (carte.getEffetsCarte().empty()) {
        writeContentLine(out, "  - (aucun)", inner);
    } else {
        for (const auto& eff : carte.getEffetsCarte()) {
            if (!eff) continue;
            auto txt = eff->toString();
            // wrap en tenant compte de "  - "
            auto wrapped = wrapText(txt, textWidth - 3); // 3 pour "  -"
            bool first = true;
            for (auto& w : wrapped) {
                std::string line = (first ? "  - " : "    ") + w;
                writeContentLine(out, line, inner);
                first = false;
            }
        }
    }

    // 7. Effets de faction
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

    // 8. bas
    out << "+" << std::string(inner, '-') << "+\n";

    return out.str();
}