#include <stdlib.h>
#include "lua/script.h"
#include "cart.h"

int WriteCartridge(const char *filename, const char* name, const char* code, uint32_t code_size = 22) {
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    Cartridge cart = {};
    strncpy(cart.name, name, sizeof(cart.name)-1);
    cart.code_size = code_size;
    
    fwrite(&cart, sizeof(cart), 1, file);
    fwrite(code, 1, code_size, file);
    fclose(file);
    return 0;
}

int ReadCartridge(const char* filename) {
    printf("Loading %s...\n", filename);
    
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;
    
    Cartridge cart;
    fread(&cart, sizeof(cart), 1, file);
    
    char* code = (char*)malloc(cart.code_size + 1);
    fread(code, 1, cart.code_size, file);
    code[cart.code_size] = '\0';
    
    printf("name: %s\n", cart.name);
    printf("game: %s\n", code);
    
    Start(code);
    
    free(code);
    fclose(file);
    return 0;
}