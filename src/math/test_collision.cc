
#include <boost/test/unit_test.hpp>
#include "collision.h"

BOOST_AUTO_TEST_SUITE (test_collision)

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_1)
{
	math::sphere<2> sph;
	sph.centre.set(10, 10);
	sph.radius = 1;

	math::line<2> l;
	l.A.set(-100, 0);
	l.B.set(100, 0);

	math::contact_info<2> ci;
	math::collide(l, sph, math::vec<2>(-2, -1), ci);

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (math::abs(ci.time - 9.0f) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_2)
{
	math::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	math::line<2> l;
	l.A.set(0, 0);
	l.B.set(0, 1);

	math::contact_info<2> ci;
	math::collide(l, sph, math::vec<2>(-1, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(math::abs(ci.time - 9.0f) < math::EPSILON);
	BOOST_REQUIRE((ci.position - math::vec<2>(0, 0.5f)).length_sq() < math::EPSILON);
	BOOST_REQUIRE((ci.normal - math::vec<2>(1, 0)).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_3)
{
	math::line<2> l;
	l.A.set(1, -10);
	l.B.set(1, 10);

	math::sphere<2> s;
	s.centre.set(10, 10);
	s.radius = 1;

	math::contact_info<2> ci;
	math::collide(l, s, math::vec<2>(-1, -1), ci);

	BOOST_REQUIRE ((ci.position - math::vec<2>(1, 2)).length() < math::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_1)
{
	math::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	math::sphere<2> sph;
	sph.centre.set(0.5, 10);
	sph.radius = 1;

	math::contact_info<2> ci;
	math::collide(obb, sph, math::vec<2>(0, -1), ci);

	BOOST_REQUIRE ((ci.position - math::vec<2>(0.5, 1)).length() < math::EPSILON);
	BOOST_REQUIRE ((ci.normal - math::vec<2>(0, 1)).length() < math::EPSILON);
	BOOST_REQUIRE (math::abs(ci.time - 8) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_2)
{
	math::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	math::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	math::contact_info<2> ci;

	math::collide(obb, sph, math::vec<2>(-1.0f, -0.01f), ci);

	BOOST_REQUIRE ((ci.position - math::vec<2>(1, 0.42f)).length() < math::EPSILON);
	BOOST_REQUIRE ((ci.normal - math::vec<2>(1, 0)).length() < math::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
