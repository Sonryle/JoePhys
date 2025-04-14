// Define 'palette' in a source file so that it can be accessed by
// any other file which includes "Colour.hpp" (since 'palette' is declared
// as an extern Palette in the "Colour.hpp", but not initialised. It is
// initialised here in "Colour.cpp")
#include <Colour.hpp>

Palette palette;
