#include "shellColors.h"

namespace shell {
    // namespace { //< Simulate private scope
        std::stringstream generate_ascii(Style s, Color c) {
            std::stringstream res;
            res << "\e[" << "1;" << static_cast<char>(c) << "m";
            return res;
        }
    // }
}