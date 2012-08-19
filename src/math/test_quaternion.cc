
#include <stdlib.h>
#include <boost/test/unit_test.hpp>
#include "matrix.h"
#include "quaternion.h"

#include <iostream>

namespace
{

void test_matrix_quaternion_matrix(math::matrix<3, 3> const& tf) {
	BOOST_REQUIRE (abs(tf.determinant() - 1) < math::EPSILON);

	math::quaternion<> q;
	q.set_unit(tf);

	math::matrix<3, 3> m;
	m.rotation(q.normalized());

	BOOST_REQUIRE (math::equal(m, tf, math::scalar(0.001)));
}

void test_slerp_i(math::vec<3> const& p, math::vec<3> const& q) {
	math::matrix<3, 3> tf1;
	tf1.rotation(p);
	math::quaternion<> q1;
	q1.set_unit(tf1);

	math::matrix<3, 3> tf2;
	tf2.rotation(q);
	math::quaternion<> q2;
	q2.set_unit(tf2);

	math::quaternion_slerper<> slerper;
	slerper.setup(q1, q2);

	for (int i = 0; i <= 100; ++i) {
		math::scalar x = math::scalar(i) / 100;

		math::quaternion<> q3 = slerper.interpolate(x);
		math::matrix<3, 3> tf3;
		tf3.rotation(q3);

		math::vec<3> angles = p * (1 - x) + q * x;
		math::matrix<3, 3> tf4;
		tf4.rotation(angles);

		BOOST_REQUIRE(equal(tf3, tf4, math::scalar(0.001)));
	}
}

void test_slerp(math::vec<3> const& p, math::vec<3> const& q) {
	test_slerp_i(p, q);
	test_slerp_i(q, p);
}

}

BOOST_AUTO_TEST_SUITE (test_quaternion)

BOOST_AUTO_TEST_CASE (identity_matrix)
{
	math::matrix<3, 3> tf;
	tf.identity();

	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_90_degrees)
{
	math::matrix<3, 3> tf;

	tf.rotation(math::PI / 2, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-math::PI / 2, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, math::PI / 2, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -math::PI / 2, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, math::PI / 2);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -math::PI / 2);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_60_degrees)
{
	math::matrix<3, 3> tf;

	tf.rotation(math::PI / 3, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-math::PI / 3, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, math::PI / 3, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -math::PI / 3, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, math::PI / 3);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -math::PI / 3);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_30_degrees)
{
	math::matrix<3, 3> tf;

	tf.rotation(math::PI / 6, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-math::PI / 6, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, math::PI / 6, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -math::PI / 6, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, math::PI / 6);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -math::PI / 6);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_random_degrees)
{
	math::matrix<3, 3> tf;

	for (int i = 0; i < 1000; ++i) {
		math::vec<3> angles(
			2 * math::scalar(rand()) / RAND_MAX * math::PI,
			2 * math::scalar(rand()) / RAND_MAX * math::PI,
			2 * math::scalar(rand()) / RAND_MAX * math::PI);
		tf.rotation(angles);
		test_matrix_quaternion_matrix(tf);
	}
}

BOOST_AUTO_TEST_CASE (slerp_identity)
{
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 0));
}

BOOST_AUTO_TEST_CASE (slerp_90_degrees)
{
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(math::PI / 2, 0, 0));
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(-math::PI / 2, 0, 0));
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(0, math::PI / 2, 0));
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(0, -math::PI / 2, 0));
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, math::PI / 2));
	test_slerp(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, -math::PI / 2));

	test_slerp(math::vec<3>(-math::PI/2, 0, 0), math::vec<3>(math::PI / 2, 0, 0));
	test_slerp(math::vec<3>(0, -math::PI/2, 0), math::vec<3>(0, math::PI / 2, 0));
	test_slerp(math::vec<3>(0, 0, -math::PI/2), math::vec<3>(0, 0, math::PI / 2));
}

BOOST_AUTO_TEST_CASE (slerp_random_degrees)
{
	for (int i = 0; i < 1000; ++i) {
		math::vec<3> axis(
			2 * math::scalar(rand()) / RAND_MAX - 1,
			2 * math::scalar(rand()) / RAND_MAX - 1,
			2 * math::scalar(rand()) / RAND_MAX - 1);

		if (axis.length_sq() < 0.1) continue;

		math::scalar angle1 = math::scalar(rand()) / RAND_MAX * math::PI * 2;
		math::scalar angle2 = angle1 + math::scalar(rand() - 1) / RAND_MAX * math::PI;

		math::matrix<3,3> tf1;
		tf1.rotation(axis, angle1);

		math::matrix<3,3> tf2;
		tf2.rotation(axis, angle2);

		math::quaternion<> q1;
		q1.set_unit(tf1);

		math::quaternion<> q2;
		q2.set_unit(tf2);

		math::quaternion_slerper<> slerper;
		slerper.setup(q1, q2);

		for (int j = 0; j <= 100; ++j) {
			math::scalar x = math::scalar(j) / 100;

			math::quaternion<> q = slerper.interpolate(x);

			math::matrix<3,3> m1;
			m1.rotation(q);

			math::matrix<3,3> m2;
			m2.rotation(axis, angle1 * (1 - x) + angle2 * x);

			BOOST_REQUIRE(equal(m1, m2, math::scalar(0.001)));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

