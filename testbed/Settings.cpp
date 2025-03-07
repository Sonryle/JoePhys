// Define 'settings' in a source file so that it can be accessed by
// any other file which includes "Settings.hpp" (since 'settings' is declared
// as an extern Settings in "Settings.hpp", but not initialised. It is
// initialised here in "Settings.cpp")
#include "Settings.hpp"

Settings settings;
