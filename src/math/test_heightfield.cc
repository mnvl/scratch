
#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "heightfield.h"

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE(test_raytrace_simple_1)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(0, 10.0f, 0), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(0, 0, 0)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_2)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(0.5f, 10.0f, 0), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(0.5f, 0.5f, 0)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_3)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(1, 10.0f, 0), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(1, 1, 0)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_4)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(0, 10.0f, 0.5f), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(0, 1.5f, 0.5f)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_5)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(0, 10.0f,1), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(0, 3, 1)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_6)
{
	math::matrix<4,4> tf;
    tf.identity();

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

    static boost::uint8_t const heights[] = {
		0, 1,
		3, 7
    };

    hf.load_from_raw_buffer(heights);

	math::contact_info<3> ci = hf.trace(math::ray<3>(math::vec<3>(1, 10.0f,1), math::vec<3>(0, -1.0f, 0)));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(1, 7, 1)).length_sq() < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_complex_1)
{
	math::matrix<4,4> tf;
    tf.scaling(10, 10, 10);

	math::heightfield<boost::uint8_t> hf(tf, 11, 11);

    static boost::uint8_t const heights[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

    hf.load_from_raw_buffer(heights);

	math::vec<3> p1(0, 10, 0);
	math::vec<3> p2(6.6737876f, 0, 4.9803157f);

	math::contact_info<3> ci = hf.trace(math::ray<3>(p1, p2 - p1));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE (abs(ci.position.y / ci.position.x - 1) < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_complex_2)
{
	math::matrix<4,4> tf;
    tf.scaling(10, 10, 10);

	math::heightfield<boost::uint8_t> hf(tf, 11, 11);

    static boost::uint8_t const heights[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

    hf.load_from_raw_buffer(heights);

	math::vec<3> p1(0, 10, 0);
	math::vec<3> p2(2.0899076f, 0, 5.1918087f);

	math::contact_info<3> ci = hf.trace(math::ray<3>(p1, p2 - p1));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE (abs(ci.position.y / ci.position.x - 1) < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_complex_3)
{
	math::matrix<4,4> tf;
    tf.scaling(10, 10, 10);

	math::heightfield<boost::uint8_t> hf(tf, 11, 11);

    static boost::uint8_t const heights[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };
	hf.load_from_raw_buffer(heights);

	math::vec<3> p1(0, 10, 0);
	math::vec<3> p2(4.0656757f, 0, 7.5460067f);

	math::contact_info<3> ci = hf.trace(math::ray<3>(p1, p2 - p1));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE (abs(ci.position.y / ci.position.x - 1) < 0.01f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_raytrace_complex_4)
{
	math::matrix<4,4> tf;
    tf.scaling(10, 10, 10);

	math::heightfield<boost::uint8_t> hf(tf, 11, 11);

    static boost::uint8_t const heights[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };

    hf.load_from_raw_buffer(heights);

	for (int i = 0; i < 100; ++i)
	{
		math::vec<3> p1(0, 10, 0);
		math::vec<3> p2(math::scalar(rand()) / RAND_MAX * 10, 0, math::scalar(rand()) / RAND_MAX * 10);

		math::contact_info<3> ci = hf.trace(math::ray<3>(p1, p2 - p1));

 		BOOST_REQUIRE (ci.happened == true);
		BOOST_REQUIRE (ci.penetrated == false);
		BOOST_REQUIRE (abs(ci.position.y / ci.position.x - 1) < 0.01f);
		BOOST_REQUIRE (ci.normal.y > 0);
	}
}

BOOST_AUTO_TEST_CASE(test_raytrace_simple_save_load_1)
{
	std::string dump;

	{
		math::matrix<4,4> tf;
		tf.scaling(10, 10, 10);

		math::heightfield<boost::uint8_t> hf(tf, 11, 11);

		static boost::uint8_t const heights[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		};

		hf.load_from_raw_buffer(heights);

		std::stringstream ss;
		boost::archive::binary_oarchive oa(ss);
		oa & hf;

		dump = ss.str();
	}

	math::heightfield<boost::uint8_t> hf(math::matrix<4,4>(), 10, 10);
	std::stringstream stream(dump);
	boost::archive::binary_iarchive(stream) & hf;

	math::vec<3> p1(0, 10, 0);
	math::vec<3> p2(2.0899076f, 0, 5.1918087f);

	math::contact_info<3> ci = hf.trace(math::ray<3>(p1, p2 - p1));

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (ci.penetrated == false);
	BOOST_REQUIRE ((ci.position - math::vec<3>(1.7286383f, 1.7286377f, 4.2943335f)).length_sq() < 1e-6f);
	BOOST_REQUIRE (ci.normal.y > 0);
}

BOOST_AUTO_TEST_CASE(test_y_under)
{
	math::matrix<4,4> tf;
	tf.scaling(1, 1, 1);

	math::heightfield<boost::uint8_t> hf(tf, 2, 2);

	static boost::uint8_t heights[2 * 2] = {
		0, 1,
		4, 9,
	};

	hf.load_from_raw_buffer(heights);

	for (size_t i = 0; i < 100; ++i)
	{
		math::scalar height = hf.y_under(math::vec<3>(i / 100.0f, 100, 0));
		BOOST_REQUIRE (abs(height - i / 100.0) < 1e-3);
	}

	for (size_t i = 0; i < 100; ++i)
	{
		math::scalar height = hf.y_under(math::vec<3>(0, 100, i / 100.0f));
		BOOST_REQUIRE (abs(height - 4 * i / 100.0f) < 1e-3);
	}

	for (size_t i = 0; i < 100; ++i)
	{
		math::scalar height1 = hf.y_under(math::vec<3>(i / 100.0f, 100, 1 - 1e-6f));
		math::scalar height2 = 4 + (9 - 4) * i / 100.0f;
		BOOST_REQUIRE (abs(height1 - height2) < 0.01f);
	}

	for (size_t i = 0; i < 100; ++i)
	{
		math::scalar height1 = hf.y_under(math::vec<3>(1 - 1e-6f, 100, i / 100.0f));
		math::scalar height2 = 1 + (9 - 1) * i / 100.0f;
		BOOST_REQUIRE (abs(height1 - height2) < 0.01f);
	}
}

BOOST_AUTO_TEST_SUITE_END()
