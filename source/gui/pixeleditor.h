#pragma once
#include "../cartridge/sprite/sprite.h"
#include "../cartridge/cart.h"
#include <imgui.h>
#include <stdint.h>
#include <filesystem>

struct ConsoleLine {
    std::string text;
    ImU32 color;
    bool newline = true;
};

static const ImWchar ranges[] = {
    0x0020, 0x00FF,
    0x2580, 0x259F,
    0,             
};
static bool focused = false;

static bool scroll_to_bottom = false;
static std::vector<ConsoleLine> console_lines;
static char console_input[256] = {0};

extern sprite sprites[16];

extern int screenState;

inline void ShowConsoleWindow(std::string& current_path){
    static bool should_scroll = false; 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(33, 33, 35, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 180));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(33, 33, 35, 255));
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2 {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y}, ImGuiCond_Always);
    ImGui::Begin("Console",
        nullptr,
        ImGuiWindowFlags_NoTitleBar    |
        ImGuiWindowFlags_NoResize      |
        ImGuiWindowFlags_NoMove        |
        ImGuiWindowFlags_NoCollapse    |
        ImGuiWindowFlags_NoDecoration
    );
    
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    for (size_t i = 0; i < console_lines.size(); i++) {
        ImGui::PushStyleColor(ImGuiCol_Text, console_lines[i].color);
        ImGui::TextUnformatted(console_lines[i].text.c_str());
        ImGui::PopStyleColor();
        if (i + 1 < console_lines.size() && !console_lines[i].newline) {
            ImGui::SameLine(0.0f, 0.0f);
        }
    }
    
    float scroll_max = ImGui::GetScrollMaxY();
    bool at_bottom = ImGui::GetScrollY() >= scroll_max - 1.0f;
    
    
    ImGui::PushItemWidth(-1);
    
    bool input_active = ImGui::IsItemActive();
    
    bool printable_key_pressed = false;
    ImGuiIO& io = ImGui::GetIO();
    for (int key = ImGuiKey_Space; key <= ImGuiKey_Z; ++key) {
        if (ImGui::IsKeyPressed((ImGuiKey)key, false)) {
            printable_key_pressed = true;
            break;
        }
    }
    if (printable_key_pressed && !input_active) {
        ImGui::SetKeyboardFocusHere();
    }
    
    static bool blink = false;
    static float blinkTime = 0.0f;
    blinkTime += ImGui::GetIO().DeltaTime;
    if (blinkTime > 0.5f) { blink = !blink; blinkTime = 0.0f; }
    std::string prompt = std::string(std::filesystem::relative(current_path, std::filesystem::current_path().string()).string() + "> ") + console_input + (blink ? "_" : " ");
    ImGui::TextUnformatted(prompt.c_str());
    
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
    
    ImGui::SetNextItemWidth(1.0f);
    
    if (ImGui::InputText("##console_input", console_input, sizeof(console_input), ImGuiInputTextFlags_EnterReturnsTrue)) {
        console_lines.push_back({std::string(std::filesystem::relative(current_path, std::filesystem::current_path().string()).string() + "> ") + console_input, IM_COL32(255, 255, 255, 180)});
        
        if (console_input[0] != '\0') {
            if (!strcmp(console_input, "clear")) {
                console_lines.clear();
            } else if (!strncmp(console_input, "run ", 4)){
                if (std::string(console_input + 4) != ""){
                    std::string path = std::string(console_input + 4);
                    if (std::filesystem::exists(path)) {
                        if (not std::filesystem::is_directory(path)) {
                            ReadCartridge(path.c_str(), sprites);
                        } else {
                            console_lines.push_back({"ERROR: ", IM_COL32(204, 49, 132, 255), false});
                            console_lines.push_back({"You need to provide a file, not a directory", IM_COL32(255, 255, 255, 180)});
                        }
                        
                    } else {
                        console_lines.push_back({"ERROR: ", IM_COL32(204, 49, 132, 255), false});
                        console_lines.push_back({"The file provided does not exist.", IM_COL32(255, 255, 255, 180)});
                    }
                } else {
                    console_lines.push_back({"ERROR: ", IM_COL32(204, 49, 132, 255), false});
                    console_lines.push_back({"You need to provide a file to run.", IM_COL32(255, 255, 255, 180)});
                    console_lines.push_back({"Example: run mycart.r8", IM_COL32(255, 255, 255, 180)});
                }
            } else if (!strncmp(console_input, "run", 3)) {
                console_lines.push_back({"ERROR: ", IM_COL32(204, 49, 132, 255), false});
                console_lines.push_back({"You need to provide a file to run.", IM_COL32(255, 255, 255, 180)});
                console_lines.push_back({"Example: run mycart.r8", IM_COL32(255, 255, 255, 180)});
            } else if (!strncmp(console_input, "cd ", 3)) {
                std::string new_path = std::string(current_path + "/" + (console_input + 3));
                if (std::filesystem::is_directory(new_path)) {
                    if (std::filesystem::exists(new_path)) {
                        current_path = new_path;
                        console_lines.push_back({current_path, IM_COL32(75, 86, 235, 255)});      
                    }
                }
            } else if (!strcmp(console_input, "ls")) {
                
                for (const auto& entry: std::filesystem::directory_iterator(current_path)) {
                    if (entry.is_directory()) {
                        console_lines.push_back({entry.path().filename().string(), IM_COL32(75, 86, 235, 255)});                    
                    }
                    else if (entry.is_regular_file()){
                        console_lines.push_back({entry.path().filename().string(), IM_COL32(255, 255, 255, 180)});
                    }
                }  
                
            }
        }
        
        if (!strncmp(console_input, "exit", 4)) {
            ImGui::PopItemWidth();
            ImGui::PopFont();
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
            ImGui::End();
            exit(0);
        }
        
        console_input[0] = '\0';
        should_scroll = true;
    }
    if (should_scroll || (at_bottom && !console_lines.empty())) {
        ImGui::SetScrollHereY(1.0f);
        should_scroll = false;
    }

    ImGui::PopItemWidth();
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
    ImGui::End();
}

inline void ShowGameWindow(RenderTexture2D& gameTexture, const char* windowTitle) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2 {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y}, ImGuiCond_Always);
    ImGui::Begin(windowTitle,
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize   |
        ImGuiWindowFlags_NoMove     |
        ImGuiWindowFlags_NoCollapse
    );
    
    if (gameTexture.id != 0) {
        ImVec2 avail = ImGui::GetContentRegionAvail();
        ImGui::Image(
            (ImTextureID)(uintptr_t)gameTexture.texture.id,
            avail,
            ImVec2(0, 1), ImVec2(1, 0)
        );
    } else {
        ImGui::Text("Game not running!");
    }
    ImGui::End();
}
#pragma endregion