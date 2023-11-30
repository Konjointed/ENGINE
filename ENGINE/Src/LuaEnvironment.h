#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

class LuaEnvironment {
public:
	LuaEnvironment() {
		L = luaL_newstate();  // Create a new Lua state
		luaL_openlibs(L);     // Open all standard Lua libraries
	}

	~LuaEnvironment() { lua_close(L); }

    void DoFile(const char* filename) {
        if (luaL_dofile(L, filename) != 0) {
            // Handle errors, for example, print the error message
            fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        }
    }
private:
	lua_State* L;
};

#endif 