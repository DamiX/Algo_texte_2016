#include <iostream>
#include "shellColors.h"

int main(int argc, char** argv) {
    std::cout << shell::Red << "Coucou, le monde!" << shell::Default << std::endl;
    std::cout << shell::Style(shell::green, shell::underline) << "Hello, world!" << shell::Default << std::endl;
    return 0;
}
