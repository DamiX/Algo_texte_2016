#include <iostream>
#include "shellColors.h"

int main(int argc, char** argv) {
    std::stringstream red = shell::generate_ascii(shell::Style::Bold, shell::Color::Blue);
    std::cout << red.str() << "Test\x1b[0m" << std::endl;
    std::cout << "\x1b[4;32mHello, world!\e[0m" << std::endl;
    return 0;
}