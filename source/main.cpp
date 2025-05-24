#include <string.h>
#include <filesystem>
#include <stdio.h>
#include "Cartridge/Lua/Script.h"
#include "Cartridge/Cartridge.h"

int main(int argc, char *argv[]) {
    //Cartridge cart;
    //strncpy(cart.name, "Super Tester Brothers", sizeof(cart.name) - 1);
    //cart.name[sizeof(cart.name) - 1] = '\0';
    //FILE* f = fopen("raylib.lua", "r");
    //char buffer[4096];
    //size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, f);
    //buffer[bytes_read] = '\0';
    //fclose(f);
    //strncpy(cart.code, buffer, sizeof(cart.code) - 1);
    //cart.code[sizeof(cart.code) - 1] = '\0';
    //WriteCartridge("cart.r8", cart.name, cart.code, sizeof(cart.code) - 1);
    
    if (argc != 1) {
        for (int i = 1; i < argc; i++) {
            // Help
            if (!strcmp(argv[i], "--help") or !strcmp(argv[i], "-h")) {
                printf("Commands:\n./rice8 <filename>         - Execute the cartridge\n./rice8 (-h) --help        - Shows the available commands.\n./rice8 (-v) --version     - Shows the version of the emulator\n");
                break;
            }
            
            // Version
            if (!strcmp(argv[i], "--version") or !strcmp(argv[i], "-v")) {
                printf("RICE-8, Version 0.1\nMade by raic.e (https://github.com/RiceTheDev)\n");
                break;
            }
            
            // Execution of the cartridge
            if (argc == 2) {
                // TODO: Check if is a valid cartridge (.r8 file)
                if (std::filesystem::exists(argv[i])) {
                    if (not std::filesystem::is_directory(argv[i])) {
                        std::string filepath = argv[i];
                        ReadCartridge(filepath.c_str());
                    } else {
                        fprintf(stderr, "ERROR: You need to provide a file, not a directory", argv[i]);
                        return 1;
                    }
                } else {
                    fprintf(stderr, "ERROR: The file %s does not exist", argv[i]);
                    return 1;
                }
            }
        }
    } else {
        printf("No arguments provided. See --help for more information.\n");
    }
}