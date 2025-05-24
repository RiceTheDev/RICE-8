#ifndef SCRIPT
#define SCRIPT
    #include "sol/sol.hpp"
    extern sol::state luaState;
    int Start(const char* code);
    int StartFile(const char* path);
#endif