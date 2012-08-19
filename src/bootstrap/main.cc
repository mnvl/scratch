
#include <windows.h>
#include <boost/filesystem/v3/path.hpp>
#include <boost/filesystem/v3/operations.hpp>
#include <log4cplus/configurator.h>
#include <util/lua_state.h>
#include <util/logger.h>

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
		if (boost::filesystem::exists(boost::filesystem::path("log4cplus.properties")))
		{
			log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
		}
		else
		{
			log4cplus::BasicConfigurator::doConfigure();
		}

		util::LuaState().run(lpstrCmdLine);
    }
    catch (std::exception const &error)
    {
        LOG_ERROR("exception caught: " << error.what());
        return 1;
    }
    catch (...)
    {
        LOG_ERROR("unknown exception caught");
        return 1;
    }

    return 0;
}
