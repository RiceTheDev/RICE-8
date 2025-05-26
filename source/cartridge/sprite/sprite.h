#include <raylib.h>

struct color {
    int idx; // index of the color. 0: black, etc...
};

struct sprite {
    Color pixel[8][8] = {};
};