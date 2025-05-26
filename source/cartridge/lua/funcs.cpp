#include "funcs.h"

void init_window(int width, int height, std::string title) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

void begin_render() {
    BeginDrawing();
    ClearBackground(WHITE);
}

void draw_text(std::string text, int x, int y, int fontsize, unsigned char r, unsigned char g, unsigned char b) {
    DrawText(text.c_str(), x, y, fontsize, Color {r, g, b, 255});
}

void end_render() {
    EndDrawing();
}

void setfuncs() {
    luaState["ShouldClose"] = &WindowShouldClose;
    luaState["InitWindow"] = &init_window;
    luaState["BeginRender"] = &begin_render;
    luaState["DrawText"] = &draw_text;
    luaState["EndRender"] = &end_render;
}