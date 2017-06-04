#pragma once

#include <string>
#include <sstream>
#include <unordered_map> //TODO : Replace w/ sorted array later

/**
 * @brief Provide color for shell
 *
 * This namespace tends to provide color
 * for shell terminal that support AINSI
 * color codes (aka ANSI escape code).
 * source : https://en.wikipedia.org/wiki/ANSI_escape_code
 *
 * Author : Damien Mehala
 * Mail : damien.mehala@gmail.com
 * ------------
 * Licence : GNU GPL3
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */
namespace shell {
    // Defs
    enum class Style : char {
        None        =       0,
        Bold        =       1,
        Underline   =       4,
        Blink       =       5,
        Negative    =       7,
        Hide        =       8
    };

    enum class Color : char {
        Black       =       30,
        Red         =       31,
        Green       =       32,
        Yellow      =       33,
        Blue        =       34,
        Magenta     =       35,
        Cyan        =       36,
        White       =       37
    };
    
    std::stringstream generate_ascii(Style s, Color c);
}