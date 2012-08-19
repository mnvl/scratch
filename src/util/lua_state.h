#pragma once

#include <luabind/lua_include.hpp>

namespace util
{

class LuaState {
public:
	LuaState();
	~LuaState();

	void run(char const *filename);

private:
	lua_State *L_;
};

}

