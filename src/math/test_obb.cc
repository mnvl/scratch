
#include <boost/test/unit_test.hpp>
#include "obb.h"

BOOST_AUTO_TEST_SUITE(obb)

BOOST_AUTO_TEST_CASE(world_to_local_vector)
{
	math::obb<3> bounds;
	bounds.origin.set(0, 0, 0);
	bounds.tangent.set(1, 2, 3);
	bounds.normal.set(3, 1, 2);
	bounds.binormal.set(2, 3, 1);

	math::vec<3> point, point_local, point_world;

	point.set(100, 50, -2);

	bounds.world_to_local_vector(point_local, point);
	bounds.local_to_world_vector(point_world, point_local);

	BOOST_REQUIRE((point - point_world).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

