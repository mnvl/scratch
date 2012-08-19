
#include <boost/test/unit_test.hpp>
#include "line.h"
#include "capsule.h"
#include "sphere.h"

BOOST_AUTO_TEST_SUITE(test_sphere)

BOOST_AUTO_TEST_CASE (test_sphere_line_intersection)
{
	math::sphere<2> s;
	s.centre.set(1, 0);
	s.radius = 1;

	math::line<2> l;
	l.A.set(0, 1);
	l.B.set(0, -1);

	math::vec<2> p1, p2;
	int npoints = s.query_intersection(l, p1, p2);

	BOOST_REQUIRE (npoints == 1);
	BOOST_REQUIRE ((p1 - math::vec<2>(0, 0)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_capsule_intersection_1)
{
	math::sphere<3> s(math::vec<3>(0, 0, 0), 0.5f);
	math::capsule<3> c(math::vec<3>(0.7f, -1, 0), math::vec<3>(0.7f, 1, 0), 0.5f);

	BOOST_REQUIRE (s.test_intersection(c) == true);
}

BOOST_AUTO_TEST_CASE (test_sphere_capsule_intersection_2)
{
	math::sphere<3> s(math::vec<3>(0, 0, 0), 0.5f);
	math::capsule<3> c(math::vec<3>(1.7f, -1, 0), math::vec<3>(1.7f, 1, 0), 0.5f);

	BOOST_REQUIRE (s.test_intersection(c) == false);
}

BOOST_AUTO_TEST_SUITE_END()
