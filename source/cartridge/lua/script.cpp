#include <stdio.h>
#include "script.h"
#include "funcs.h"

sol::state luaState;

int Start(const char* script){
    setfuncs();
    try {
        luaState.open_libraries(sol::lib::base);
        luaState.script(script);
        return 0;    
    } catch (const sol::error& e) {
        printf("ERROR: %s\n", e.what());
        return 1;
    }
}

int StartFile(const char* path) {
    setfuncs();
    try {
        luaState.open_libraries(sol::lib::base);
        luaState.script_file(path);
        return 0;
    } catch (const sol::error& e) {
        printf("ERROR: %s\n", e.what());
        return 1;
    }
}