#include "sosi2osm.h"

#include <string.h>
#include <stdlib.h>
#include <libgen.h>
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
    FILE *file_exist_check;
    int r;
    state = luaL_newstate();
    luaL_openlibs(state);
    
    char filename_string[256];
    snprintf(filename_string, 256, "%s/lua/sosi2osm.lua", dirname(execname));
    if(file_exist_check = fopen(filename_string, "r")){
        fclose(file_exist_check);
        r = luaL_loadfile(state, filename_string);
        if (r) {
            fprintf(stderr, "Failed to load lua library file %s\n", filename_string);
            exit(1);
        }
    }
    else {
        snprintf(filename_string, 256, "/usr/local/share/sosi2osm/lua/sosi2osm.lua");
        if(file_exist_check = fopen(filename_string, "r")){
            fclose(file_exist_check);
            r = luaL_loadfile(state, filename_string);
            if (r) {
                fprintf(stderr, "Failed to load lua library file %s\n", filename_string);
                exit(1);
            }
        }
        else {
            snprintf(filename_string, 256, "/usr/share/sosi2osm/lua/sosi2osm.lua");
            if(file_exist_check = fopen(filename_string, "r")){
                fclose(file_exist_check);
                r = luaL_loadfile(state, filename_string);
                if (r) {
                    fprintf(stderr, "Failed to load lua library file %s\n", filename_string);
                    exit(1);
                }
                
            }
            else {
                fprintf(stderr, "Could not find sosi2osm.lua. It has to be installed in /usr/share/sosi2osm/lua/ or /usr/local/share/sosi2osm/lua/\n");
                exit(1);
            }
        }
    }
    
    lua_call(state, 0, 0);
    
    if(file_exist_check = fopen(filename, "r")) {
        fclose(file_exist_check);
        r = luaL_loadfile(state, filename);
        if (r) {
            fprintf(stderr, "Failed to load lua file %s\n", filename);
            exit(1);
        }
    }
    else {
        snprintf(filename_string, 256, "/usr/local/share/sosi2osm/lua/%s",filename);
        if(file_exist_check = fopen(filename_string, "r")) {
            fclose(file_exist_check);
            r = luaL_loadfile(state, filename_string);
            if (r) {
                fprintf(stderr, "Failed to load lua file %s\n", filename_string);
                exit(1);
            }
        }
        else {
            snprintf(filename_string, 256, "/usr/share/sosi2osm/lua/%s",filename);
            if(file_exist_check = fopen(filename_string, "r")) {
                fclose(file_exist_check);
                r = luaL_loadfile(state, filename_string);
                if(r) {
                    fprintf(stderr, "Failed to load lua file %s\n", filename_string);
                    exit(1);
                }
            }
            else {
                fprintf(stderr, "Could not find %s. It has to be installed in /usr/share/sosi2osm/lua/ or /usr/local/share/sosi2osm/lua/\n",filename);
                exit(1);
            }
        }
    }
    
    
    lua_setglobal(state, "getTagsFromInfoTable");
}

void outputTags(FILE* output) {
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
        fprintf(output, "<tag k=\"%s\" v=\"%s\"/>", key, value);
        lua_pop(state, 1);
    }
    
    lua_pop(state, 1);
}
