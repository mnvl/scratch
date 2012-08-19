
#include <boost/test/unit_test.hpp>
#include "camera.h"

BOOST_AUTO_TEST_SUITE(camera)

BOOST_AUTO_TEST_CASE(translation_rotation_1)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(translation_rotation_2)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(-123, -456, -789), math::vec<3>(0, 0, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(123, 456, 789)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(123, 456, 789) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(translation_rotation_3)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(-math::PI/4, 0, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, -0.707106f, 0.707106f) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0.707106f, 0.707106f) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(translation_rotation_4)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(math::PI/2, 0, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 1, 0) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, -1) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(eye_lookat_up_1)
{
	graphic::Camera c;
	c.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1), math::vec<3>(0, 1, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(eye_lookat_up_2)
{
	graphic::Camera c;
	c.set_eye_lookat_up(math::vec<3>(123, 456, 789), math::vec<3>(123, 456, 790), math::vec<3>(0, 1, 0));

	BOOST_REQUIRE (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(123, 456, 789)).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(123, 456, 789) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_REQUIRE ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
