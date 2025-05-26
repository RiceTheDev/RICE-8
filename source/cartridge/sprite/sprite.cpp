#include "sprite.h"
#include <raylib.h>

struct color {
    int idx; // index of the color. 0: black, etc...
};

Color pixel[8][8] = {};