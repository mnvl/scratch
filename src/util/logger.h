#pragma once

#include <log4cplus/logger.h>
#include <luabind/lua_include.hpp>

#define LOG_DEBUG(M) LOG4CPLUS_DEBUG(log4cplus::Logger::getRoot(), M)
#define LOG_INFO(M)	 LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), M)
#define LOG_WARN(M)	 LOG4CPLUS_WARN(log4cplus::Logger::getRoot(), M)
#define LOG_ERROR(M) LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), M)

namespace util
{

void bind_logger(lua_State *L);

}


