
#include <log4cplus/configurator.h>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace
{

struct Dummy
{
	Dummy()
	{
		log4cplus::BasicConfigurator::doConfigure();
	}
};

Dummy dummy;

}
