#include "sosi2osm.h"

#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

iconv_t charDescriptor;
void setEncoding(char* encoding) {
    charDescriptor = iconv_open("UTF-8", encoding);
}

char* toUTF8(char* in, char* outBuf, size_t outlen) {
    if (in == NULL) {
        return NULL;
    }
    size_t inlen = strlen(in)+1;
    char* out = outBuf;
    
    iconv(charDescriptor, NULL, NULL, NULL, NULL);
    int r = iconv(charDescriptor, &in, &inlen, &out, &outlen);
    
    if (r == -1) {
        fprintf(stderr, "Unknown character '%c' (0x%hhx)\n", in[0], in[0]);
        exit(1);
    }
    
    return outBuf;
}

lua_State *state;
void loadLua(char* filename) {
    state = luaL_newstate();
    luaL_openlibs(state);
    int r = luaL_loadfile(state, filename);
    if (r) {
        fprintf(stderr, "Failed to load lua file %s\n", filename);
        exit(1);
    }
    
    luaL_loadfile(state, "lua/sosi2osm.lua");
    lua_call(state, 0, 0);
    
    lua_setglobal(state, "getTagsFromInfoTable");
}

void outputTags() {
    lua_getglobal(state, "getTagsFromInfoTable");
    lua_newtable(state);
    int size = 0;

    long lines = getSOSILinesLength();
    for (int i = 0; i < lines; i++) {
        char buffer[256];
        char* line = toUTF8(getSOSILine(i), buffer, 256);
        if (line != NULL) {
            lua_pushinteger(state, ++size);
            lua_pushstring(state, line);
            lua_settable(state, -3);
        }
    }
    
    lua_setglobal(state, "info");
    lua_call(state, 0, 1);
    
    lua_pushnil(state);
    while (lua_next(state, 1) != 0) {
        if (lua_type(state, -2) != LUA_TSTRING) {
            fprintf(stderr, "Error in lua script: key must be string, got '%s'\n", lua_tostring(state, -2));
            exit(1);
        }
        const char* key = lua_tostring(state, -2);
        const char* value = lua_tostring(state, -1);
        printf("<tag k=\"%s\" v=\"%s\"/>", key, value);
        lua_pop(state, 1);
    }
    
    lua_pop(state, 1);
}
