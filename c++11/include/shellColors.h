/**
 * @brief Provide color for shell
 *
 * This namespace provide color for shell terminal that support AINSI
 * escape code.
 * source : https://en.wikipedia.org/wiki/ANSI_escape_code
 * ------------
 * Licence : GNU GPL3
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */
#pragma once

#include <string>
#include <sstream>

namespace shell {
    // Style
    constexpr char none         = 0;
    constexpr char bold         = 1;
    constexpr char negative     = 2;
    constexpr char blink        = 3;
    constexpr char underline    = 4;
    constexpr char hide         = 5;

    // Color
    constexpr char black    = 30;
    constexpr char red      = 31;
    constexpr char green    = 32;
    constexpr char yellow   = 33;
    constexpr char blue     = 34;
    constexpr char magenta  = 35;
    constexpr char cyan     = 36;
    constexpr char white    = 37;

    struct Style {
        char color;
        char style;

        Style(const char in_color, const char in_style = none)
        : color(in_color), style(in_style)
        {};
    };

    const Style Red(red);
    const Style Default(black);

    std::ostream& operator<<(std::ostream& os, const Style s) {
        os << "\x1b[" << std::to_string(s.style) << ";" << std::to_string(s.color) << "m";
        return os;
    }
}
