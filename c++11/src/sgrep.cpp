#include <iostream>
#include "shellColors.h"

int main(int argc, char** argv) {
    std::stringstream red = shell::generate_ascii(shell::Style::Bold, shell::Color::Red);
    std::cout << red.str() << "Test\e[Om" << std::endl;
    std::cout << "\e[1;31mHello, world!\e[0m" << std::endl;
    return 0;
}