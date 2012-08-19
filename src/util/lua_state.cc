
#include <xercesc/util/PlatformUtils.hpp>
#include <luabind/luabind.hpp>
#include <luabind/detail/pcall.hpp>
#include <graphic/bind.h>
#include <math/bind.h>
#include <spatial/bind.h>
#include <util/logger.h>
#include <util/archive.h>
#include <util/bind_boost.h>
#include <util/bind_stl.h>
#include "lua_state.h"

extern "C"
{
	#include <lualib.h>
}

namespace util
{

LuaState::LuaState():
	L_(lua_open())
{
	xercesc::XMLPlatformUtils::Initialize();

	luaL_openlibs(L_);
	luabind::open(L_);

	graphic::bind(L_);

	math::bind(L_);
	spatial::bind(L_);

	util::bind_logger(L_);
	util::ArchiveReader::bind(L_);
	util::ArchiveWriter::bind(L_);
	util::bind_boost(L_);
	util::bind_stl(L_);
}

LuaState::~LuaState()
{
	lua_close(L_);

	xercesc::XMLPlatformUtils::Terminate();
}

void LuaState::run(char const *filename)
{
	lua_getglobal(L_, "debug");
	lua_getfield(L_, -1, "traceback");
	lua_remove(L_, -2);

	int result = luaL_loadfile(L_, filename);

	if (result == 0) result = lua_pcall(L_, 0, 0, -2);

	switch (result)
	{
	case 0:
		// success
		break;

	case LUA_ERRSYNTAX:
		throw std::runtime_error("syntax eror");

	case LUA_ERRRUN:
		throw std::runtime_error(lua_tostring(L_, -1));

	case LUA_ERRFILE:
		throw std::runtime_error("file not found");

	default:
		throw std::runtime_error("unknown error");
	}
}

}

