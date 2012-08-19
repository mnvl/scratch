
#include <boost/test/unit_test.hpp>
#include "matrix.h"
#include "obb.h"
#include "frustum.h"

BOOST_AUTO_TEST_SUITE(frustum)

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_1)
{
	math::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(-1000, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_2)
{
	math::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(-199.5f, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_3)
{
	math::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(0, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_4)
{
	math::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(199.5f, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_5)
{
	math::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(1000, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_1)
{
	math::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	math::matrix<4,4> world;
	world.rotation(-math::PI/4, 0, 0);

	math::frustum<> fr(world * projection);

	math::obb<3> bound;
	bound.origin.set(0, 1000, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_2)
{
	math::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	math::matrix<4,4> world;
	world.rotation(-math::PI/4, 0, 0);

	math::frustum<> fr(world * projection);

	math::obb<3> bound;
	bound.origin.set(0, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_3)
{
	math::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	math::matrix<4,4> world;
	world.rotation(-math::PI/4, 0, 0);

	math::frustum<> fr(world * projection);

	math::obb<3> bound;
	bound.origin.set(0, -1000, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(perspective_projection_with_rotation_translation_and_obb_1)
{
	math::matrix<4,4> projection;
	projection.perspective(math::PI/2, 1.5f, 0, 1000);

	math::matrix<4,4> world;
	world.translation(-500, -10, 500);
	world.rotate(-math::PI/4, 0, 0);

	math::frustum<> fr(world * projection);

	math::obb<3> bound;
	bound.origin.set(510, 2, 490);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.contains(bound.origin) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.tangent) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.normal) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.binormal) == true);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(from_matrix_and_intersection_with_obb_1)
{
	math::matrix<4,4> tf;
	tf._11 =  0.20000000f;
	tf._12 =  0.00000000f;
	tf._13 =  0.00000000f;
	tf._14 =  0.00000000f;
	tf._21 =  0.00000000f;
	tf._22 =  0.26937401f;
	tf._23 = -0.00070781459f;
	tf._24 =  0.00000000f;
	tf._31 =  0.00000000f;
	tf._32 =  0.26937401f;
	tf._33 =  0.00070781459f;
	tf._34 =  0.00000000f;
	tf._41 = -0.19687501f;
	tf._42 = -0.0061834427f;
	tf._43 =  0.012431229f;
	tf._44 =  1.0000000f;

	math::frustum<> fr(tf);

	math::obb<3> bound;
	bound.origin.set(-5.6245117f, 0.00000000f, 23.716305f);
	bound.tangent.set(2.0000000f, 0.00000000f, 0.00000000f);
	bound.normal.set(0.00000000f, 1.0000000f, 0.00000000f);
	bound.binormal.set(0.00000000f, 0.00000000f, 2.0000000f);

	BOOST_REQUIRE (fr.contains(bound.origin) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.tangent) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.normal) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.binormal) == false);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_SUITE_END()
