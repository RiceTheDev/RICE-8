#ifndef FUNCTIONS
#define FUNCTIONS
    #include "sol/sol.hpp"
    #include "raylib.h"
    #include "script.h"
    #include "string"
    
    void init_window(int width, int height, std::string title);

    void begin_render();

    void draw_text(std::string text, int x, int y, int fontsize, unsigned char r, unsigned char g, unsigned char b);
    
    void end_render();
    
    void setfuncs();
#endif