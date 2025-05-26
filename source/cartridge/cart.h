#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct Cartridge
{
    char name[32];
    char author[32];
    char version[8];
    char description[128];
    char code[4096];
    uint32_t code_size;
};

// int WriteCartridge(const char *filename, const char* name, const char* author, const char* version, const char* description, const char* code, uint32_t code_size) {
int WriteCartridge(const char *filename, const char* name, const char* code, uint32_t code_size);
int ReadCartridge(const char* filename);