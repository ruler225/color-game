// Tentative name, used to store data that is used by all object, and fits in

#pragma once
#include <array>

using std::array;

// none the colours for the filters, `GREEN` is tentative
enum Color { RED, BLUE, GREEN };

using Vector3 = array<float, 3>; // common data alias

const float GRAVITY_CONST = 5.0;