#include "funcs.h"
#include "../../gui/pixeleditor.h"

RenderTexture2D gameTexture;

Font font = LoadFontEx("PixelifySans.ttf", 128, 0, 250);
void init_window(int width, int height, std::string title) {
    gameTexture = LoadRenderTexture(width, height);
    SetTargetFPS(60);
}

void begin_render() {
    BeginTextureMode(gameTexture);
    ClearBackground(WHITE);
}

void close_window() {
    CloseWindow();
}

void drawtext(std::string text, int x, int y, float fontsize, unsigned char r, unsigned char g, unsigned char b) {
    //DrawText(text.c_str(), x, y, (int)fontsize, Color {r, g, b, 255});
    DrawTextEx(font, text.c_str(), Vector2{(float)x, (float)y}, fontsize, 2.0f, Color {r, g, b, 255});
}

void draw_texture(Texture2D tex, int posX, int posY, float scale) {
    DrawTextureEx(tex, Vector2 { (float)posX, (float)posY }, 0.0f, scale, WHITE);
}

Texture2D create_texture_from_sprites(int idx) {
    Image img = GenImageColor(8, 8, BLANK);
    for (int y=0; y < 8; y++) {
        for (int x=0; x < 8; x++) {
            ImageDrawPixel(&img, x, y, sprites[idx].pixel[y][x]);
        }
    }
    return LoadTextureFromImage(img);
    //UnloadTexture(tex);
    //UnloadImage(img);
}

void end_render() {
    EndTextureMode();
}

void setfuncs() {
    luaState["ShouldClose"] = &WindowShouldClose;
    luaState["InitWindow"] = &init_window;
    luaState["BeginRender"] = &begin_render;
    luaState["DrawText"] = &drawtext;
    luaState["EndRender"] = &end_render;
    luaState["DrawImageFromSprites"] = &create_texture_from_sprites;
    luaState["DrawTexture"] = &draw_texture;
    luaState["CloseWindow"] = &close_window;
    
}