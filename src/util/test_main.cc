
#include <time.h>
#include <stdlib.h>

namespace
{

struct Dummy
{
	Dummy()
	{
		srand((unsigned) time(0));
	}
};

Dummy dummy;
}

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
