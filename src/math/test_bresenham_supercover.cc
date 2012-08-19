
#include "bresenham_supercover.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (brezenham_supercover)

BOOST_AUTO_TEST_CASE (one_point)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(2, 6));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_2)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(2, 4));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}


BOOST_AUTO_TEST_CASE (two_points_digonal_3)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(2, 6), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_4)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(2, 4), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(3, 6));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_2)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(3, 4));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_3)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(3, 6), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_4)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(3, 4), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (many_points_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 1), math::vec<2, int>(12, 9));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 1);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 1);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 2);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 2);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 3);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 4);
	BOOST_REQUIRE (position.y == 3);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 4);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 5);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 6);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 6);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 7);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 7);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 8);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 9);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 9);
	BOOST_REQUIRE (position.y == 7);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 10);
	BOOST_REQUIRE (position.y == 7);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 10);
	BOOST_REQUIRE (position.y == 8);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 11);
	BOOST_REQUIRE (position.y == 8);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 11);
	BOOST_REQUIRE (position.y == 9);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 12);
	BOOST_REQUIRE (position.y == 9);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (many_points_2)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(4, 7), math::vec<2, int>(0, 0));

	math::vec<2, int> position, prev_position;
	bool flag;

	do
	{
		prev_position = position;
		boost::tie(position, flag) = bs.get();
	}
	while (flag == false);
}

BOOST_AUTO_TEST_CASE (final_point_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(2, 5), math::vec<2, int>(0, 0));

	math::vec<2, int> position, prev_position;
	bool flag;

	do
	{
		prev_position = position;
		boost::tie(position, flag) = bs.get();
	}
	while (flag == false);

	BOOST_REQUIRE (prev_position.x == 0);
	BOOST_REQUIRE (prev_position.y == 0);
}

BOOST_AUTO_TEST_CASE (final_point_2)
{
	for (int i = 0; i < 100; ++i)
	{
		int x1 = rand();
		int x2 = rand();
		int y1 = rand();
		int y2 = rand();

		math::bresenham_supercover<> bs(math::vec<2, int>(x1, y1), math::vec<2, int>(x2, y2));

		math::vec<2, int> position, prev_position;
		bool flag;

		do
		{
			prev_position = position;
			boost::tie(position, flag) = bs.get();
		}
		while (flag == false);

		BOOST_REQUIRE (prev_position.x == x2);
		BOOST_REQUIRE (prev_position.y == y2);
	}
}

BOOST_AUTO_TEST_SUITE_END()


