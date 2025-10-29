#pragma once
#ifndef CARD_RENDERER_HPP
#define CARD_RENDERER_HPP

#include <string>
#include "../Carte.hpp"

namespace ui {

struct CardRenderer {
    struct Options {
        int width = 40;
        bool useUnicode = false;
        int artHeight = 4;
        int padding = 1;
    };

    static std::string render(const Carte& carte, const Options& opts);

private:
    static std::string padCenter(const std::string& s, int width);
    static std::string padRight(const std::string& s, int width);
    static std::vector<std::string> wrapText(const std::string& text, int maxWidth);
};

} // namespace ui

#endif
