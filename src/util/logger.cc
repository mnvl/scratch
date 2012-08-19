
#include "logger.h"

namespace util
{

namespace
{

int log_message(lua_State *L, log4cplus::LogLevel level)
{
	log4cplus::Logger logger = log4cplus::Logger::getRoot();

	if(logger.isEnabledFor(level))
	{
		lua_Debug d;
		lua_getstack(L, 1, &d);
		lua_getinfo(L, "Sln", &d);

        log4cplus::tostringstream formatter;

		for (size_t i = 1, n = lua_gettop(L); i <= n; i++)
		{
			char const *str = lua_tostring(L, i);
			if (!str) str = "<can't_convert_to_string>";
			formatter << str;
		}

		logger.forcedLog(level, formatter.str(), d.short_src, d.currentline);
    }

	lua_pop(L, 1);

	return 0;
}

int log_debug(lua_State *L)
{
	return log_message(L, log4cplus::DEBUG_LOG_LEVEL);
}

int log_info(lua_State *L)
{
	return log_message(L, log4cplus::INFO_LOG_LEVEL);
}

int log_warn(lua_State *L)
{
	return log_message(L, log4cplus::WARN_LOG_LEVEL);
}

int log_error(lua_State *L)
{
	return log_message(L, log4cplus::ERROR_LOG_LEVEL);
}

luaL_reg const lua_functions[] = {
	{"debug", log_debug},
	{"info", log_info},
	{"warn", log_warn},
	{"error", log_error},
	{0, 0}
};

}

void bind_logger(lua_State *L)
{
	luaL_register(L, "log", lua_functions);
}

}
