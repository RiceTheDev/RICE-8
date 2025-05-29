#include "../cartridge/lua/script.h"
#include "pixeleditor.h"
sprite sprites[16];
#include "../cartridge/sprite/sprite.h"
#include "../cartridge/cart.h"
#include "rlImGui.h"
#include "imgui.h"
#include "../theming.h"
#include "TextEditor.h"
#include <iostream>

extern RenderTexture2D gameTexture;

int screenState = 0;

std::string current_path = std::filesystem::current_path().string();

void DrawSprite(int cellSize, int offsetX, int offsetY, Color pixels[8][8]) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            DrawRectangle(x * cellSize + offsetX, y * cellSize + offsetY, cellSize, cellSize, Color { 34, 32, 41, 255 });
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
    // Console Text
    console_lines.push_back({"█", IM_COL32(112, 48, 117, 255), false});
    console_lines.push_back({"█", IM_COL32(204, 49, 132, 255), false});
    console_lines.push_back({"█", IM_COL32(232, 125, 67, 255), false});
    console_lines.push_back({"█", IM_COL32(255, 206, 107, 255), false});
    console_lines.push_back({"█", IM_COL32(218, 240, 209, 255), false});
    console_lines.push_back({"█", IM_COL32(93, 212, 119, 255), false});
    console_lines.push_back({"█", IM_COL32(37, 162, 168, 255), false});
    console_lines.push_back({"█", IM_COL32(75, 86, 235, 255), false});
    console_lines.push_back({"█", IM_COL32(47, 50, 143, 255), false});
    console_lines.push_back({"█", IM_COL32(44, 32, 61, 255), false});
    console_lines.push_back({"█", IM_COL32(122, 135, 153, 255), false});
    console_lines.push_back({"█", IM_COL32(174, 194, 194, 255), true});
    console_lines.push_back({"RICE-8 Version 0.1", IM_COL32(255, 255, 255, 180), true});
    
    float screenWidth = 800;
    float screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Editor");
    SetExitKey(KEY_NULL);
    
    static TextEditor editor;
    TextEditor::Palette Textpalette = TextEditor::GetDarkPalette();
    Textpalette[(int)TextEditor::PaletteIndex::Background] = IM_COL32(33, 33, 35, 255);
    Textpalette[(int)TextEditor::PaletteIndex::Number] = IM_COL32(194, 211, 104, 255);
    Textpalette[(int)TextEditor::PaletteIndex::Keyword] = IM_COL32(251, 86, 7, 255);
    //Textpalette[(int)TextEditor::PaletteIndex::Number] = 0xFFD00AFF;
    editor.SetPalette(Textpalette);
    editor.SetText("InitWindow(242, 136, \"Name of the window\")\ntex = DrawImageFromSprites(0)\nfunction game_frame()\n\tBeginRender()\n\tDrawTexture(tex, 50, 50, 5)\n\tDrawText(\"test123\", 200, 200, 48, 255, 0, 0)\n\tEndRender()\nend");
    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    
    rlImGuiSetup(true);
    Font font = LoadFontEx("PixelifySans.ttf", 128, 0, 250);
    
    Image img_drawing_icon = LoadImage("DrawIcon.png");
    Texture2D drawing_icon = LoadTextureFromImage(img_drawing_icon);
    
    Image img_code_icon = LoadImage("Code.png");
    Texture2D code_icon = LoadTextureFromImage(img_code_icon);
    
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

    ImFont* code_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("PxPlus_IBM_BIOS.ttf", 16.0f, nullptr, ranges);
    SetTextureFilter(*(Texture2D*)&ImGui::GetIO().Fonts->TexID, TEXTURE_FILTER_POINT);
    ImGui::GetIO().Fonts->Build();

    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();  
         
        // Menu
        if (IsKeyPressed(KEY_ESCAPE)) {
            screenState = 4;
        }
        
        // States     
        if (screenState == 0){
            BeginDrawing();
                ClearBackground(Color {69, 69, 69, 255});

                rlImGuiBegin();
                ImGui::PushFont(code_font);

                ShowConsoleWindow(current_path);

                rlImGuiEnd();
            EndDrawing();
        } else if (screenState == 1) {
            BeginDrawing();
                ClearBackground(Color {69, 69, 69, 255});
                
                rlImGuiBegin();
                
                ImGui::SetNextWindowPos(ImVec2(0, 32), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2 {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 32}, ImGuiCond_Always);
                Vector2 mouse = GetMousePosition();
                ImGui::Begin("Code Editor",
                    nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoCollapse
                );
                
                if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) {
                    Cartridge cart;
                    strncpy(cart.name, "Super Tester Brothers", sizeof(cart.name) - 1);
                    cart.name[sizeof(cart.name) - 1] = '\0';
                    char buffer[4096];
                    // set the buffer as editor.GetText()
                    strncpy(buffer, editor.GetText().c_str(), sizeof(buffer) - 1);
                    size_t bytes_read = strlen(buffer);
                    buffer[bytes_read] = '\0';
                    strncpy(cart.code, buffer, sizeof(cart.code) - 1);
                    cart.code[sizeof(cart.code) - 1] = '\0';
                    WriteCartridge("cart_2.r8", cart.name, cart.code, sprites, sizeof(cart.code) - 1);
                    ReadCartridge("cart_2.r8", sprites);
                    std::cout << editor.GetText() << std::endl;
                    screenState = 3;
                }

                set_theme();
                ImGui::PushFont(code_font);
                editor.Render("Code Editor");
                ImGui::PopFont();
                if (CheckCollisionPointRec(mouse, {0, 0, 150, 30})) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        screenState = 2;
                    }
                }
                
                if (CheckCollisionPointRec(mouse, {150, 0, 150, 30})) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                        screenState = 1;
                    }
                }  
                
                ImGui::End();
                
                DrawRectangle(0, 30, 300, screenHeight, Color { 33, 33, 35, 255 });
                DrawRectangle(0, 0, screenWidth, 30, Color { 33, 33, 35, 255 });
                DrawLine(0, 30, screenWidth, 30, Color {34, 22, 30, 255});
                DrawLine(0, 31, screenWidth, 31, Color {34, 22, 30, 255});
                DrawLineEx((Vector2){150, 0}, (Vector2){150, 30}, 2, Color {34, 22, 30, 255});
                DrawLineEx((Vector2){300, 0}, (Vector2){300, screenHeight}, 2, Color {34, 22, 30, 255});
                DrawTextureEx(drawing_icon, Vector2 {25, 0}, 0, 1.8f, WHITE);
                DrawTextureEx(code_icon, Vector2 {250, 0}, 0, 1.8f, WHITE);
                DrawTextEx(font, "Draw", Vector2 {60, 3 }, 22, 0, WHITE);
                DrawTextEx(font, "Code", Vector2 {190, 3}, 22, 0, WHITE);
                rlImGuiEnd();
            EndDrawing();
        } else if (screenState == 2) {
            BeginDrawing();
            ClearBackground(Color {53, 53, 50, 255});
            
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

            DrawPixels(cellSize, offsetX, offsetY, sprites[current_sprite].pixel, palette, current_colorX, current_colorY, mouse);
            
            // GUI Part
            DrawRectangle(0, 30, 300, screenHeight, Color { 33, 33, 35, 255 });
            DrawRectangle(0, 0, screenWidth, 30, Color { 33, 33, 35, 255 });
            DrawLine(0, 30, screenWidth, 30, Color {34, 22, 30, 255});
            DrawLine(0, 31, screenWidth, 31, Color {34, 22, 30, 255});
            DrawLineEx((Vector2){150, 0}, (Vector2){150, 30}, 2, Color {34, 22, 30, 255});
            DrawLineEx((Vector2){300, 0}, (Vector2){300, screenHeight}, 2, Color {34, 22, 30, 255});
            DrawTextureEx(drawing_icon, Vector2 {25, 0}, 0, 1.8f, WHITE);
            
            DrawTextureEx(code_icon, Vector2 {250, 0}, 0, 1.8f, WHITE);
            DrawTextEx(font, "Draw", Vector2 {60, 3}, 22, 0, WHITE);
            DrawTextEx(font, "Code", Vector2 {190, 3}, 22, 0, WHITE);
            DrawTextEx(font, "Palette", Vector2 {100, 45}, 32, 0, WHITE);
            DrawTextEx(font, "Sprites", Vector2 {100, 170}, 32, 0, WHITE);
            
            
            if (CheckCollisionPointRec(mouse, {0, 0, 150, 30})) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    ReadCartridge("cart_2.r8", sprites);
                }
            }

            
            if (CheckCollisionPointRec(mouse, {150, 0, 150, 30})) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Cartridge cart;
                    strncpy(cart.name, "Super Tester Brothers", sizeof(cart.name) - 1);
                    cart.name[sizeof(cart.name) - 1] = '\0';
                    FILE* f = fopen("raylib.lua", "r");
                    char buffer[4096];
                    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, f);
                    buffer[bytes_read] = '\0';
                    fclose(f);
                    strncpy(cart.code, buffer, sizeof(cart.code) - 1);
                    cart.code[sizeof(cart.code) - 1] = '\0';
                    //WriteCartridge("cart_2.r8", cart.name, cart.code, sprites, sizeof(cart.code) - 1);
                    screenState = 3;
                }
            }         
            // Color Palette
            for (int x=0; x < 2; x++) {
                for (int y=0; y < 6; y++) {
                    int px = (y * 32) + (y * 8) + 33;
                    int py = (x * 32) + (x * 8) + 80;
                    if (mouse.x >= px && mouse.x < px + 32 && mouse.y >= py && mouse.y < py + 32) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            current_colorX = x;
                            current_colorY = y;
                        }
                    }
                    DrawRectangle((y * 32) + (y * 8) + 33, (x * 32) + (x * 8) + 80, 32, 32, palette[x][y]);
                    DrawRectangleLinesEx((Rectangle){(float)(y * 32) + (float)(y * 8) + 33, (float)(x * 32) + (float)(x * 8) + 80, 32, 32 }, 2, Color {0, 0, 0, 255});
                    // Drawing the current color with a white border
                    if (current_colorX == x and current_colorY == y) {
                        DrawRectangleLinesEx((Rectangle){(float)(y * 32) + (float)(y * 8) + 33, (float)(x * 32) + (float)(x * 8) + 80, 32, 32 }, 2, Color {255, 255, 255, 255});
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
                    DrawRectangle(cellX, cellY, 48, 48, Color {20, 20, 20, 255});
                    if (mouse.x >= cellX && mouse.x < cellX + 48 && mouse.y >= cellY && mouse.y < cellY + 48) {
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
            EndDrawing();
        } else if (screenState == 3) {
            
            BeginDrawing();
            ClearBackground(Color {69, 69, 69, 255});
            
            rlImGuiBegin();
            
            if (luaState["game_frame"].valid()) {
                luaState["game_frame"]();
            }
            
            ShowGameWindow(gameTexture, "Game Output");

            rlImGuiEnd();
            EndDrawing();
        } else if (screenState == 4) {
            BeginDrawing();
                ClearBackground(Color {53, 53, 50, 255});
            EndDrawing();
        }
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}