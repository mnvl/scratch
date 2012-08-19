
#include <boost/test/unit_test.hpp>
#include "matrix.h"

namespace
{

template<int N> void
test_axis_angle_i(math::vec<3> const &src, math::vec<3> const &axis, math::scalar angle, math::vec<3> const &res)
{
	math::matrix<N,N> m;
	m.rotation(axis, angle);

	math::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < math::EPSILON);
	BOOST_REQUIRE ((res - result).length() < math::EPSILON);
}

void test_axis_angle(math::vec<3> const &src, math::vec<3> const &axis, math::scalar angle, math::vec<3> const &res)
{
	test_axis_angle_i<3>(src, axis, angle, res);
	test_axis_angle_i<4>(src, axis, angle, res);

	test_axis_angle_i<3>(res, axis, -angle, src);
	test_axis_angle_i<4>(res, axis, -angle, src);
}

template<int N> void
test_rotation_i(math::vec<3> const &src, math::vec<3> const &angles, math::vec<3> const &res)
{
	math::matrix<N,N> m;
	m.rotation(angles);

	math::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < math::EPSILON);
	BOOST_REQUIRE ((res - result).length() < math::EPSILON);
}

void test_rotation(math::vec<3> const &src, math::vec<3> const &angles, math::vec<3> const &res)
{
	test_rotation_i<3>(src, angles, res);
	test_rotation_i<4>(src, angles, res);

	test_rotation_i<3>(res, -angles, src);
	test_rotation_i<4>(res, -angles, src);
}

}

BOOST_AUTO_TEST_SUITE (test_matrix)

BOOST_AUTO_TEST_CASE (test_ijk)
{
	math::vec<3> const i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);

	test_axis_angle(i, j, -math::PI / 2, k);
	test_rotation(i, -math::PI / 2 * j, k);

	test_axis_angle(j, k, -math::PI / 2, i);
	test_rotation(j, -math::PI / 2 * k, i);

	test_axis_angle(k, i, -math::PI / 2, j);
	test_rotation(k, -math::PI / 2 * i, j);
}

BOOST_AUTO_TEST_CASE (test_pi4)
{
	{
		math::vec<3> src(1, 0, 0);
		math::vec<3> res = math::normalize(math::vec<3>(1, -1, 0));

		test_rotation(src, math::vec<3>(0, 0, -math::PI / 4), res);
		test_axis_angle(src, math::vec<3>(0, 0, 1), -math::PI / 4, res);
	}

	{
		math::vec<3> src(0, 1, 0);
		math::vec<3> res = math::normalize(math::vec<3>(1, 1, 0));

		test_rotation(src, math::vec<3>(0, 0, -math::PI / 4), res);
		test_axis_angle(src, math::vec<3>(0, 0, 1), -math::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_pi4_number_2)
{
	{
		math::vec<3> src(1, 0, 0);
		math::vec<3> res = math::normalize(math::vec<3>(1, 1, 0));

		test_rotation(src, math::vec<3>(0, 0, math::PI / 4), res);
		test_axis_angle(src, math::vec<3>(0, 0, 1), math::PI / 4, res);
	}

	{
		math::vec<3> src(0, 1, 0);
		math::vec<3> res = math::normalize(math::vec<3>(-1, 1, 0));

		test_rotation(src, math::vec<3>(0, 0, math::PI / 4), res);
		test_axis_angle(src, math::vec<3>(0, 0, 1), math::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_1)
{
	math::matrix<4,4> M, I, R, U;

	M.scaling(math::scalar(rand() + 1), math::scalar(rand() + 1), math::scalar(rand() + 1));

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(math::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_2)
{
	math::matrix<4,4> M, I, R, U;

	M.rotation(rand() * math::PI / RAND_MAX , rand() * math::PI / RAND_MAX, rand() * math::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(math::equal(R, U));
}


BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_3)
{
	math::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(math::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_4)
{
	math::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);
	M.rotate(rand() * math::PI / RAND_MAX , rand() * math::PI / RAND_MAX, rand() * math::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(math::equal(R, U));
}

BOOST_AUTO_TEST_SUITE_END()
