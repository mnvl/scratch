
#include <iomanip>
#include <windows.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <log4cplus/configurator.h>
#include <util/lua_state.h>
#include <util/logger.h>

std::vector< std::pair<std::string, std::string> > tests;

void search_for_tests(boost::filesystem::path const &path)
{
	for (boost::filesystem::directory_iterator it(path), end; it != end; ++it)
	{
		if (boost::filesystem::is_directory(*it))
		{
			search_for_tests(*it);
		}
		else if (boost::filesystem::is_regular_file(*it))
		{
			if (boost::regex_match(it->path().filename().string(), boost::regex("test_.*.lua")))
			{
				tests.push_back(std::make_pair(it->path().filename().string(), it->path().string()));
			}
		}
	}
}

int main(int argc, char *argv[])
{
    try
    {
		log4cplus::BasicConfigurator::doConfigure();

		search_for_tests(boost::filesystem::path(SCRATCH_ROOT "/lua"));

		std::sort(tests.begin(), tests.end());

		for (std::vector< std::pair<std::string, std::string> >::const_iterator it = tests.begin(); it != tests.end(); ++it)
		{
			LOG_INFO("*** [" << std::right << std::setw(3) << it - tests.begin() << " / " << std::right << std::setw(3) << tests.size() << "] " << it->first);

			util::LuaState().run(it->second.c_str());
		}

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
