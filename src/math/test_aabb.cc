
#include <boost/test/unit_test.hpp>
#include "ray.h"
#include "aabb.h"

BOOST_AUTO_TEST_SUITE(aabb)

BOOST_AUTO_TEST_CASE(trace_through_corners)
{
	math::aabb<3> b(math::vec<3>(0, 0, 0), 20.0f);
	math::ray<3> r(math::vec<3>(-20, -20, -20), math::vec<3>(40, 40, 40));

	math::scalar t0, t1;

	b.trace(r, &t0, &t1);

	BOOST_REQUIRE (math::abs(t0 - 0.25f) < math::EPSILON);
	BOOST_REQUIRE (math::abs(t1 - 0.75f) < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

