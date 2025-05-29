#include <stdlib.h>
#include "lua/script.h"
#include "sprite/sprite.h"
#include "cart.h"

int WriteCartridge(const char *filename, const char* name, const char* code, sprite sprites_out[16], uint32_t code_size = 22) {
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    Cartridge cart = {};
    strncpy(cart.name, name, sizeof(cart.name)-1);
    cart.code_size = code_size;
    memcpy(cart.sprites, sprites_out, sizeof(cart.sprites));
    fwrite(&cart, sizeof(cart), 1, file);
    fwrite(code, 1, code_size, file);
    fclose(file);
    return 0;
}

int ReadCartridge(const char* filename, sprite sprites_out[16]) {
    printf("Loading %s...\n", filename);
    
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;
    
    Cartridge cart;
    fread(&cart, sizeof(cart), 1, file);
    
    memcpy(sprites_out, cart.sprites, sizeof(cart.sprites));
    
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