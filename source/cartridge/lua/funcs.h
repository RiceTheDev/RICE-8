#ifndef FUNCTIONS
#define FUNCTIONS
    #include <sol/sol.hpp>
    #include <string>
    #include <raylib.h>
    #include "script.h"
    
    void init_window(int width, int height, std::string title);

    void begin_render();

    void draw_text(std::string text, int x, int y, int fontsize, unsigned char r, unsigned char g, unsigned char b);
    
    void draw_image_from_sprites(int idx);
    
    void end_render();
    
    void setfuncs();
#endif