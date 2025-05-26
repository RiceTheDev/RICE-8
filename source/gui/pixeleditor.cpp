#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "cartridge/sprite/sprite.h"

void DrawSprite(int cellSize, int offsetX, int offsetY, Color pixels[8][8]) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            DrawRectangle(x * cellSize + offsetX, y * cellSize + offsetY, cellSize, cellSize, Color { 56, 56, 56, 255 });
            DrawRectangle(x * cellSize + offsetX, y * cellSize + offsetY, cellSize, cellSize, pixels[y][x]);
        }
    }
}

void DrawPixels(int cellSize, int offsetX, int offsetY, Color pixels[8][8], Color palette[2][6], int current_colorX, int current_colorY, Vector2 mouse) {
    int mouseX = (mouse.x);
    int mouseY = (mouse.y);
    int blockX = (mouseX - offsetX) / cellSize;
    int blockY = (mouseY - offsetY) / cellSize;
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (blockX >= 0 && blockX < 8 && blockY >= 0 && blockY < 8) {
            pixels[blockY][blockX] = palette[current_colorX][current_colorY];
        }
    } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        if (blockX >= 0 && blockX < 8 && blockY >= 0 && blockY < 8) {
            pixels[blockY][blockX] = Color {0, 0, 0, 0};
        }
    }
}

int main(void)
{
    const float screenWidth = 800.0f;
    const float screenHeight = 450.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Editor");

    RenderTexture2D screen = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(screen.texture, TEXTURE_FILTER_POINT);
    
    Font font = LoadFontEx("PixelifySans.ttf", 128, 0, 250);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
    
    Image img_drawing_icon = LoadImage("DrawIcon.png");
    Texture2D drawing_icon = LoadTextureFromImage(img_drawing_icon);
    
    Image img_code_icon = LoadImage("Code.png");
    Texture2D code_icon = LoadTextureFromImage(img_code_icon);
    
    sprite pixels;
    sprite pixels2;
            
    pixels.pixel[8][8] = {};
    pixels2.pixel[8][8] = {};
    
    sprite sprites[16] = {
        pixels,
        pixels2
    };
    
    Color palette[2][6] = {
        { Color {112, 48, 117, 255}, Color {204, 49, 132, 255}, Color {232, 125, 67, 255}, Color {255, 206, 107, 255}, Color {218, 240, 209, 255}, Color {93, 212, 119, 255} },
        { Color {37, 162, 168, 255}, Color {75, 86, 235, 255}, Color {47, 50, 143, 255}, Color {44, 32, 61, 255}, Color {122, 135, 153, 255}, Color {174, 194, 194, 255} }
    };
    
    SetTargetFPS(60);
    
    float zoom = 2.0f;
    
    int offsetX = 450;
    int offsetY = 100;
    
    int current_colorX = 0;
    int current_colorY = 0;
    
    int current_sprite = 0;
    
    while (!WindowShouldClose())
    {
        
        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();

        float scaleX = (float)currentScreenWidth / screenWidth;
        float scaleY = (float)currentScreenHeight / screenHeight;
        float scale = fminf(scaleX, scaleY);

        BeginTextureMode(screen);
            ClearBackground(Color {69, 69, 69, 255});    
            
            // Drawing Part    
            int cellSize = (int)(16 * zoom);
            
            float wheel = GetMouseWheelMove();
            if (wheel != 0) {
                zoom += wheel * 0.2f;
                if (zoom < 0.2f) zoom = 0.2f; // Prevent negative/zero zoom
                if (zoom > 10.0f) zoom = 10.0f;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
                int MouseDeltaX = GetMouseDelta().x;
                int MouseDeltaY = GetMouseDelta().y;
                offsetX += MouseDeltaX; 
                offsetY += MouseDeltaY; 
            }
            
            DrawSprite(cellSize, offsetX, offsetY, sprites[current_sprite].pixel);
            Vector2 mouse = GetMousePosition();
            Vector2 virtualMouse = {
                mouse.x * (screenWidth / currentScreenWidth),
                mouse.y * (screenHeight / currentScreenHeight)
            };

            DrawPixels(cellSize, offsetX, offsetY, sprites[current_sprite].pixel, palette, current_colorX, current_colorY, virtualMouse);
            
            // GUI Part
            DrawRectangle(0, 30, 300, screenHeight, Color { 46, 46, 46, 255 });
            DrawRectangle(0, 0, screenWidth, 30, Color { 33, 33, 33, 255 });
            DrawLine(0, 30, screenWidth, 30, Color {20, 20, 20, 255});
            DrawLine(0, 31, screenWidth, 31, Color {20, 20, 20, 255});
            DrawLineEx((Vector2){150, 0}, (Vector2){150, 30}, 2, Color {20, 20, 20, 255});
            DrawLineEx((Vector2){300, 0}, (Vector2){300, screenHeight}, 2, Color {20, 20, 20, 255});

            
            
            if (CheckCollisionPointRec(virtualMouse, {0, 0, 150, 30})) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    std::cout << "Drawing Mode" << std::endl;
                }
            }
            
            if (CheckCollisionPointRec(virtualMouse, {150, 0, 150, 30})) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    std::cout << "Drawing Modea" << std::endl;
                }
            }        
            // Color Palette
            for (int x=0; x < 2; x++) {
                for (int y=0; y < 6; y++) {
                    int px = (y * 32) + (y * 8) + 33;
                    int py = (x * 32) + (x * 8) + 80;
                    if (virtualMouse.x >= px && virtualMouse.x < px + 32 && virtualMouse.y >= py && virtualMouse.y < py + 32) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            current_colorX = x;
                            current_colorY = y;
                        }
                    }
                    DrawRectangle((y * 32) + (y * 8) + 33, (x * 32) + (x * 8) + 80, 32, 32, palette[x][y]);
                    DrawRectangleLinesEx((Rectangle){(y * 32) + (y * 8) + 33, (x * 32) + (x * 8) + 80, 32, 32 }, 2, Color {0, 0, 0, 255});
                    // Drawing the current color with a white border
                    if (current_colorX == x and current_colorY == y) {
                        DrawRectangleLinesEx((Rectangle){(y * 32) + (y * 8) + 33, (x * 32) + (x * 8) + 80, 32, 32 }, 2, Color {255, 255, 255, 255});
                    }
                };
            };
            DrawTextEx(font, TextFormat("%.f", (zoom / 2) * 100), Vector2 {screenWidth - 40, screenHeight - 27}, 22, 0, WHITE);
            
            // Sprites
            for (int y1 = 0; y1 < 4; y1++) {
                for (int x1 = 0; x1 < 4; x1++) {
                    int c = y1 * 4 + x1;
                    int cellX = (x1 * 48) + (x1 * 5) + 45;
                    int cellY = (y1 * 48) + (y1 * 5) + 210;
                    if (c >= 16) break;
                    sprite spr = sprites[c];
                    DrawRectangle(cellX, cellY, 48, 48, Color {31, 31, 31, 255});
                    if (virtualMouse.x >= cellX && virtualMouse.x < cellX + 48 && virtualMouse.y >= cellY && virtualMouse.y < cellY + 48) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            current_sprite = c;
                        }
                    }
                    for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                            DrawRectangle(cellX + (x * 6), cellY + (y * 6), 6, 6, spr.pixel[y][x]);
                        }
                    }
                    if (sprites[c].pixel == sprites[current_sprite].pixel) {
                        DrawRectangleLinesEx({(float)(x1 * 48) + (x1 * 5) + 45, (float)(y1 * 48) + (y1 * 5) + 210, 48, 48}, 2, WHITE);
                        
                    }
                }
            }
            EndTextureMode();
            
            BeginDrawing();
                DrawTexturePro(
                    screen.texture,
                    (Rectangle){ 0, 0, (float)screen.texture.width, -(float)screen.texture.height }, // flip Y
                    (Rectangle){ 0, 0, currentScreenWidth, currentScreenHeight },
                    (Vector2){ 0, 0 },
                    0.0f,
                    
                    
                    
                    WHITE
                );
                
            DrawTextureEx(drawing_icon, Vector2 {25 * scale, 0}, 0, 1.8f * scale, WHITE);
            DrawTextureEx(code_icon, Vector2 {250 * ((float)GetScreenWidth() / screenWidth), 0}, 0, 1.8f * scale, WHITE);
            DrawTextEx(font, "Draw", Vector2 {60 * scale, 3 * scale}, 22 * scale, 0, WHITE);
            DrawTextEx(font, "Code", Vector2 {190 * ((float)GetScreenWidth() / screenWidth), 3 * scale}, 22 * scale, 0, WHITE);
            DrawTextEx(font, "Palette", Vector2 {(100) * ((float)GetScreenWidth() / screenWidth), 45 * scale}, 32 * scale, 0, WHITE);
            DrawTextEx(font, "Sprites", Vector2 {(100) * ((float)GetScreenWidth() / screenWidth), 170 * scale}, 32 * scale, 0, WHITE);
            EndDrawing();
        }

    UnloadRenderTexture(screen);
    CloseWindow();
    return 0;
}