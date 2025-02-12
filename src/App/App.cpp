#include <iostream>
#include "Core/Core.hpp"

int main()
{
    std::cout << "Core Version:\t" << CORE_VERSION << "\n\n";
    Core::SayHello();
    return 0;
}
