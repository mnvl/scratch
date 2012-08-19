
#include <boost/test/unit_test.hpp>
#include <util/logger.h>
#include "aabb.h"
#include "obb.h"
#include "capsule.h"

BOOST_AUTO_TEST_SUITE (capsule)

BOOST_AUTO_TEST_CASE (bounds_1)
{
	using math::random;

	unsigned false_positives = 0;

	for (int i = 0; i < 100; i++)
	{
		math::capsule<3> c(math::vec<3>(random(), random(), random()), math::vec<3>(random(), random(), random()),
			math::abs(random()) + 0.001f);

		math::aabb<3> b;
		c.get_aabb(b);

		for (int j = 0; j < 100; j++)
		{
			math::vec<3> point(random(), random(), random());

			bool b1 = c.contains(point);
			bool b2 = b.contains(point);

			if (b1 != b2)
			{
				// bounding box could have only false positioves
				BOOST_REQUIRE(b1 == false && b2 == true);

				++false_positives;
			}
		}
	}

	LOG_DEBUG("false_positives = " << false_positives);
}

BOOST_AUTO_TEST_SUITE_END()
