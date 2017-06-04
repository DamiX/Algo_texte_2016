#include "shellColors.h"

namespace shell {
    // namespace { //< Simulate private scope
        std::stringstream generate_ascii(Style s, Color c) {
            std::stringstream res;
            res << "\x1b[" << std::to_string(static_cast<char>(s)) << ";" << std::to_string(static_cast<char>(c)) << "m";
            // \x1b[4;32m
            return res;
        }
    // }
}