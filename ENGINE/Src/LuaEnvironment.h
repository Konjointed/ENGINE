#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include "Editor/UI/Console.h"

class LuaEnvironment {
public:
    Console& console; 

	LuaEnvironment(Console& console): console(console) {
		L = luaL_newstate();  // Create a new Lua state
		luaL_openlibs(L);     // Open all standard Lua libraries

        // Store 'this' in the Lua registry
        lua_pushlightuserdata(L, this);
        lua_setglobal(L, "LuaEnvironment_instance");

		// Override the Lua 'print' function
		lua_pushcfunction(L, LuaPrintOverride);
		lua_setglobal(L, "print");
	}

	~LuaEnvironment() { lua_close(L); }

    static int LuaPrintOverride(lua_State* L) {
        // Retrieve the LuaEnvironment instance from the Lua registry
        lua_getglobal(L, "LuaEnvironment_instance");
        LuaEnvironment* env = static_cast<LuaEnvironment*>(lua_touserdata(L, -1));
        lua_pop(L, 1);  // Remove it from the stack

        // Build the message string
        int nargs = lua_gettop(L);
        std::string message;
        for (int i = 1; i <= nargs; ++i) {
            if (lua_isstring(L, i)) {
                message += lua_tostring(L, i);
                if (i < nargs) message += " ";
            }
        }

        // Use the 'env' pointer to access the console
        if (env) {
            env->console.AddLog(message);
        }

        return 0;
    }

    void DoFile(const char* filename) {
        if (luaL_dofile(L, filename) != 0) {
            std::string error = lua_tostring(L, -1);
            console.AddLog("[Error] " + error); // Replace with your method to add text to the console
        }
    }
private:
	lua_State* L;
};

#endif 