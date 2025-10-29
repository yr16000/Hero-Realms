#include "../../include/ui/CardRenderer.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace ui;

static std::vector<std::string> splitLines(const std::string& s) {
    std::vector<std::string> out;
    std::istringstream iss(s);
    std::string line;
    while(std::getline(iss, line)) out.push_back(line);
    return out;
}

std::string CardRenderer::padCenter(const std::string& s, int width) {
    if((int)s.size() >= width) return s.substr(0, width);
    int total = width - (int)s.size();
    int left = total/2;
    int right = total - left;
    return std::string(left, ' ') + s + std::string(right, ' ');
}

std::string CardRenderer::padRight(const std::string& s, int width) {
    if((int)s.size() >= width) return s.substr(0, width);
    return s + std::string(width - (int)s.size(), ' ');
}

std::vector<std::string> CardRenderer::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> res;
    if(maxWidth <= 0) return res;
    std::istringstream iss(text);
    std::string word;
    std::string line;
    while(iss >> word) {
        if((int)line.size() + (int)word.size() + (line.empty()?0:1) <= maxWidth) {
            if(!line.empty()) line += " ";
            line += word;
        } else {
            if(!line.empty()) res.push_back(line);
            if((int)word.size() > maxWidth) {
                // hard split
                for(size_t i=0;i<word.size(); i+= maxWidth) {
                    res.push_back(word.substr(i, maxWidth));
                }
                line.clear();
            } else {
                line = word;
            }
        }
    }
    if(!line.empty()) res.push_back(line);
    return res;
}

std::string CardRenderer::render(const Carte& carte, const Options& opts) {
    int inner = opts.width - 2; // borders
    std::ostringstream out;
    // top border
    out << "+" << std::string(inner, '-') << "+\n";
    // title line: name left, cost right
    std::ostringstream title;
    title << carte.getNom();
    std::string cost = "[" + std::to_string(carte.getCout()) + "]";
    int spaceForName = inner - (int)cost.size() - 1;
    std::string namePart = title.str();
    std::string line;
    if(spaceForName <= 0) {
        // no room for name, just show cost right-aligned
        line = std::string(inner - (int)cost.size(), ' ') + cost;
        out << "|" << line << "|\n";
    } else {
        if((int)namePart.size() > spaceForName) {
            if(spaceForName > 3)
                namePart = namePart.substr(0, spaceForName-3) + "...";
            else
                namePart = namePart.substr(0, spaceForName);
        }
        line = namePart + std::string(spaceForName - (int)namePart.size(), ' ') + " " + cost;
        out << "|" << line << "|\n";
    }
    out << "|" << line << "|\n";
    // subtitle: type and faction
    std::string typeStr;
    switch(carte.getType()){
        case TypeCarte::Champion: typeStr = "Champion"; break;
        case TypeCarte::Action: typeStr = "Action"; break;
        case TypeCarte::Objet: typeStr = "Objet"; break;
        default: typeStr = ""; break;
    }
    std::string subtitle = typeStr + " • " + ::to_string(carte.getFaction());
    out << "|" << padRight(subtitle, inner) << "|\n";
    // empty or art area
    for(int i=0;i<opts.artHeight;i++) {
        out << "|" << std::string(inner, ' ') << "|\n";
    }
    // effects area
    // gather effect strings
    std::vector<std::string> effectLines;
    for(const auto& eptr : carte.getEffetsCarte()){
        if(!eptr) continue;
        auto s = eptr->toString();
        auto wrapped = wrapText(s, inner - opts.padding*2);
        for(auto &w : wrapped) effectLines.push_back(w);
        effectLines.push_back(std::string()); // blank between effects
    }
    if(!carte.getEffetsFaction().empty()){
        for(const auto& eptr : carte.getEffetsFaction()){
            if(!eptr) continue;
            auto s = "[Ally] " + eptr->toString();
            auto wrapped = wrapText(s, inner - opts.padding*2);
            for(auto &w : wrapped) effectLines.push_back(w);
            effectLines.push_back(std::string());
        }
    }
    if(effectLines.empty()) effectLines.push_back(std::string());
    int maxContentWidth = inner - opts.padding*2;
    for(auto &ln : effectLines){
        std::string content = ln;
        if(maxContentWidth < 0) maxContentWidth = 0;
        if((int)content.size() > maxContentWidth){
            if(maxContentWidth > 3)
                content = content.substr(0, maxContentWidth - 3) + "...";
            else
                content = content.substr(0, maxContentWidth);
        }
        out << "|" << std::string(opts.padding, ' ') << padRight(content, maxContentWidth) << std::string(opts.padding, ' ') << "|\n";
    }
    // bottom padding
    out << "+" << std::string(inner, '-') << "+\n";
    return out.str();
}
