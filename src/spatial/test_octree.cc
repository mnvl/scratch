
#include <stdlib.h>
#include <boost/test/unit_test.hpp>
#include "sphere_shape.h"
#include "octree.h"

BOOST_AUTO_TEST_SUITE (test_octree)

BOOST_AUTO_TEST_CASE (test_subcluster_bounds)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(0);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(-1, -1, -1)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(1);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(0, -1, -1)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(1, 0, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(2);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(-1, 0, -1)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(0, 1, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(3);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(0, 0, -1)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(1, 1, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(4);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(-1, -1, 0)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(0, 0, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(5);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(0, -1, 0)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(1, 0, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(6);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(-1, 0, 0)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(0, 1, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(7);
		BOOST_REQUIRE ((bounds.lo - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
		BOOST_REQUIRE ((bounds.hi - math::vec<3>(1, 1, 1)).length_sq() < math::EPSILON);
	}
}

BOOST_AUTO_TEST_CASE (test_subcluster_by_point)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-10, -100, -1000), math::vec<3>(10, 100, 1000)));

	for (size_t i = 0; i < 1000; i++)
	{
		math::vec<3> v(math::scalar((rand() % 20) - 10), math::scalar((rand() % 200) - 100),
			math::scalar((rand() % 2000) - 1000));

		unsigned index = octree.get_subcluster_by_point(v);
		math::aabb<3> bounds = octree.get_subcluster_bounds(index);

		BOOST_REQUIRE (bounds.contains(v));
	}
}

BOOST_AUTO_TEST_CASE (test_subclusters_mask_by_aabb)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-5, -5, -5), math::vec<3>(5, 5, 5)));

	// -2,-2 -1,-2 | +1,-2 +2,-2
    // -2,-1 -1,-1 | +1,-1 +2,-1
	// ------------+------------
    // -2,+1 -1,+1 | +1,+1 +2,+1
    // -2,+1 -1,+2 | +1,+1 +2,+2

	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1, -1))) == 1);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, -2), math::vec<3>(2, -1, -1))) == 2);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, -2), math::vec<3>(-1, 2, -1))) == 4);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, 1, -2), math::vec<3>(2, 2, -1))) == 8);

	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(-1, -1, 2))) == 16);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, 1), math::vec<3>(2, -1, 2))) == 32);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, 1), math::vec<3>(-1, 2, 2))) == 64);
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, 1, 1), math::vec<3>(2, 2, 2))) == 128);

	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, -1, -1))) == (1 | 2));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, 2, -1))) == (1 | 4));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, -2), math::vec<3>(2, 2, -1))) == (2 | 8));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, -2), math::vec<3>(2, 2, -1))) == (4 | 8));

	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(2, -1, 2))) == (16 | 32));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(-1, 2, 2))) == (16 | 64));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, 1), math::vec<3>(2, 2, 2))) == (32 | 128));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, 1), math::vec<3>(2, 2, 2))) == (64 | 128));


	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, 2, -1))) == (1 | 2 | 4 | 8));
	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(2, 2, 2))) == (16 | 32 | 64 | 128));

	BOOST_REQUIRE (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, 2, 2))) == (1 | 2 | 4 | 8 | 16 | 32 | 64 | 128));
}

BOOST_AUTO_TEST_CASE (test_intersect_correctness_1)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(8);

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>(-0.5f, -0.5f, -0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>( 0.5f, -0.5f, -0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>(-0.5f,  0.5f, -0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>( 0.5f,  0.5f, -0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>(-0.5f, -0.5f,  0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>( 0.5f, -0.5f,  0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>(-0.5f,  0.5f,  0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(math::vec<3>(0.5f,  0.5f,  0.5f), 0.1f)));
	octree.add_shape(shapes.back());

	octree.build_subclusters();

	for (size_t i = 0; i < 100; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		boost::shared_ptr<spatial::SphereShape> shape(new spatial::SphereShape(centre, radius));

		spatial::ResultSetPtr result(new spatial::ResultSet());
		octree.query_intersection(shape, result);

		for (size_t j = 0; j < shapes.size(); j++)
		{
			bool p1 = shapes[j]->test_intersection(shape.get());
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_intersect_correctness_2)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 500;
	static size_t const M = 100;

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(centre, radius)));
		octree.add_shape(shapes.back());
	}

	for (size_t i = 0; i < M; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::ShapePtr shape(new spatial::SphereShape(centre, radius));

		spatial::ResultSetPtr result(new spatial::ResultSet());
		octree.query_intersection(shape, result);

		for (size_t j = 0; j < N; j++)
		{
			bool p1 = shapes[j]->test_intersection(shape.get());
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_remove_correctness)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 500;
	static size_t const M = 100;

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(centre, radius)));
		octree.add_shape(shapes.back());
	}

	{
		std::vector<spatial::ShapePtr> shapes_clone;
		shapes_clone.swap(shapes);

		shapes.reserve(N);

		for (size_t i = 0; i < N; i++)
		{
			if (rand() & 1)
			{
				shapes.push_back(shapes_clone[i]);
			}
			else
			{
				octree.remove_shape(shapes_clone[i]);
			}
		}
	}

	for (size_t i = 0; i < M; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::ShapePtr shape(new spatial::SphereShape(centre, radius));

		spatial::ResultSetPtr result(new spatial::ResultSet());		octree.query_intersection(shape, result);

		for (size_t j = 0; j < shapes.size(); j++)
		{
			bool p1 = shapes[j]->test_intersection(shape.get());
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_move_correctness)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 500;
	static size_t const M = 100;

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(centre, radius)));
		octree.add_shape(shapes.back());
	}

	for (size_t i = 0; i < N; i++)
	{
		if (rand() & 1)
		{
			spatial::SphereShape *shape = static_cast<spatial::SphereShape *>(shapes[i].get());

			math::aabb<3> old_bounds = shape->get_bounds();

			shape->centre += math::vec<3>(math::random(-1, 1),
				math::random(-1, 1),
				math::random(-1, 1));

			shape->radius *= math::random(0, 2);

			octree.post_move_shape(shapes[i], old_bounds);
		}
	}

	for (size_t i = 0; i < M; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::ShapePtr shape(new spatial::SphereShape(centre, radius));

		spatial::ResultSetPtr result(new spatial::ResultSet());
		octree.query_intersection(shape, result);

		for (size_t j = 0; j < shapes.size(); j++)
		{
			bool p1 = shapes[j]->test_intersection(shape.get());
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_add_shape_result)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 500;
	static size_t const M = 100;

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::ShapePtr shape(new spatial::SphereShape(centre, radius));
		shapes.push_back(shape);

		 // result may not contain original shape
		spatial::ResultSetPtr result(new spatial::ResultSet());
		octree.add_shape(shape, result);

		for (size_t j = 0; j < i; j++)
		{
			bool p1 = shapes[j]->test_intersection(shape.get());
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_move_result)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 500;
	static size_t const M = 100;

	std::vector<spatial::ShapePtr> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::random(-1, 1), math::random(-1, 1),
			math::random(-1, 1));
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		shapes.push_back(spatial::ShapePtr(new spatial::SphereShape(centre, radius)));
		octree.add_shape(shapes.back());
	}

	for (size_t i = 0; i < N; i++)
	{
		spatial::SphereShape *shape = static_cast<spatial::SphereShape *>(shapes[i].get());

		math::aabb<3> old_bounds = shape->get_bounds();

		shape->centre += math::vec<3>(math::random(-1, 1), math::random(-1, 1), math::random(-1, 1));
		shape->radius *= math::random(0, 2);

		spatial::ResultSetPtr result(new spatial::ResultSet());
		octree.post_move_shape(shapes[i], old_bounds, result);

		for (size_t j = 0; j < i; j++)
		{
			bool p1 = shapes[j]->test_intersection(shape);
			bool p2 = result->have(shapes[j]);

			BOOST_REQUIRE (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
