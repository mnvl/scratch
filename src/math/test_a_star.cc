
#include <boost/test/unit_test.hpp>
#include "a_star.h"

BOOST_AUTO_TEST_SUITE(a_star)

BOOST_AUTO_TEST_CASE(simple_path_1)
{
	typedef math::a_star::point point;

	struct dummy {
		static math::scalar score(point const &p) {
			return (p - point(1, 0)).length_sq() + (p.x == 0 && (p.y == 0 || p.y == -1) ? 1000.0f : 0.0f);
		}
	};

	math::a_star a_star(point(-1, 0), point(1, 0), &dummy::score);
	a_star.calculate_path();
	BOOST_REQUIRE(!a_star.have_failed());

	std::vector<point> path = a_star.build_path();
	BOOST_REQUIRE(path.size() == 3);
	BOOST_REQUIRE(path[0] == point(-1, 0));
	BOOST_REQUIRE(path[1] == point( 0, 1));
	BOOST_REQUIRE(path[2] == point( 1, 0));
}

BOOST_AUTO_TEST_CASE(simple_path_2)
{
	typedef math::a_star::point point;

	struct dummy {
		static math::scalar score(point const &p) {
			return (p - point(1, 0)).length_sq() + (p.x == 0 && (p.y == 0 || p.y == -1) ? 1000.0f : 0.0f);
		}
	};

	math::a_star a_star(point(-2, 0), point(1, 0), &dummy::score);
	a_star.calculate_path();
	BOOST_REQUIRE(!a_star.have_failed());

	std::vector<point> path = a_star.build_path();
	BOOST_REQUIRE(path.size() == 4);
	BOOST_REQUIRE(path[0] == point(-2, 0));
	BOOST_REQUIRE(path[1] == point(-1, 0));
	BOOST_REQUIRE(path[2] == point( 0, 1));
	BOOST_REQUIRE(path[3] == point( 1, 0));
}

BOOST_AUTO_TEST_CASE(simple_path_3)
{
	typedef math::a_star::point point;

	struct dummy {
		static math::scalar score(point const &p) {
			return (p - point(1, 0)).length_sq() + (p.x == 0 && (p.y == 0 || p.y == -1) ? 1000.0f : 0.0f);
		}
	};

	math::a_star a_star(point(-1, 0), point(2, 0), &dummy::score);
	a_star.calculate_path();
	BOOST_REQUIRE(!a_star.have_failed());

	std::vector<point> path = a_star.build_path();
	BOOST_REQUIRE(path.size() == 4);
	BOOST_REQUIRE(path[0] == point(-1, 0));
	BOOST_REQUIRE(path[1] == point( 0, 1));
	BOOST_REQUIRE(path[2] == point( 1, 0));
	BOOST_REQUIRE(path[3] == point( 2, 0));
}

BOOST_AUTO_TEST_CASE(simple_path_without_obstacles)
{
	typedef math::a_star::point point;

	struct dummy {
		static math::scalar score(point const &p) {
			return math::scalar((p - point(10, 0)).length_sq());
		}
	};

	math::a_star a_star(point(0, 0), point(10, 0), &dummy::score);
	a_star.calculate_path();
	BOOST_REQUIRE(!a_star.have_failed());

	std::vector<point> path = a_star.build_path();

	for (int i = 0; i <= 10; ++i)
	{
		BOOST_REQUIRE (path[i] == point(i, 0));
	}
}

BOOST_AUTO_TEST_CASE(simple_path_4)
{
	typedef math::a_star::point point;

	struct dummy {
		static math::scalar score(point const &p) {
			return (p - point(10, 0)).length_sq() + (p.x == 5 && (p.y > -5 && p.y < 3) ? 1000.0f : 0.0f);
		}
	};

	math::a_star a_star(point(0, 0), point(10, 0), &dummy::score);
	a_star.calculate_path();
	BOOST_REQUIRE(!a_star.have_failed());

	std::vector<point> path = a_star.build_path();

	BOOST_REQUIRE(path.size() == 12);
	BOOST_REQUIRE(path[0] == point(0, 0));
	BOOST_REQUIRE(path[1] == point(1, 0));
	BOOST_REQUIRE(path[2] == point(2, 0));
	BOOST_REQUIRE(path[3] == point(3, 0));
	BOOST_REQUIRE(path[4] == point(4, 1));
	BOOST_REQUIRE(path[5] == point(4, 2));
	BOOST_REQUIRE(path[6] == point(5, 3));
	BOOST_REQUIRE(path[7] == point(6, 2));
	BOOST_REQUIRE(path[8] == point(7, 1));
	BOOST_REQUIRE(path[9] == point(8, 0));
	BOOST_REQUIRE(path[10] == point(9, 0));
	BOOST_REQUIRE(path[11] == point(10, 0));
}

BOOST_AUTO_TEST_SUITE_END()
