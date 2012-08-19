#pragma once

#include <set>
#include <vector>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <math/aabb.h>
#include "shape.h"
#include "result_set.h"

namespace spatial
{

class Octree
{
public:
	static size_t const SUBCLUSTERS_COUNT = 8;

	Octree(math::aabb<3> const &bounds);
	~Octree();

	math::aabb<3> get_subcluster_bounds(unsigned index) const;
	unsigned get_subcluster_by_point(math::vec<3> const &point) const;
	size_t get_subclusters_by_aabb(math::aabb<3> const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT]) const;
	unsigned get_subclusters_mask_by_aabb(math::aabb<3> const &bounds) const;

	bool is_leaf() const;

	void add_shape(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result_set = ResultSetPtr());
	void remove_shape(ShapePtr const &shape, math::aabb<3> const &bounds);
	void move_shape(ShapePtr const &shape, math::aabb<3> const &old_bounds, math::aabb<3> const &new_bounds, ResultSetPtr const &result_set = ResultSetPtr());
	void intersect_shallow(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result);
	void query_intersection(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result); // does not clear result

	void add_shape(ShapePtr const &shape)
		{ add_shape(shape, shape->get_bounds(), ResultSetPtr()); }
	void add_shape(ShapePtr const &shape, ResultSetPtr const &result_set)
		{ add_shape(shape, shape->get_bounds(), result_set); }
	void remove_shape(ShapePtr const &shape)
		{ remove_shape(shape, shape->get_bounds()); }
	void pre_move_shape(ShapePtr const &shape, math::aabb<3> const &new_bounds)
		{ move_shape(shape, shape->get_bounds(), new_bounds); }
	void pre_move_shape(ShapePtr const &shape, math::aabb<3> const &new_bounds, ResultSetPtr const &result_set)
		{ move_shape(shape, shape->get_bounds(), new_bounds, result_set); }
	void post_move_shape(ShapePtr const &shape, math::aabb<3> const &old_bounds)
		{ move_shape(shape, old_bounds, shape->get_bounds()); }
	void post_move_shape(ShapePtr const &shape, math::aabb<3> const &old_bounds, ResultSetPtr const &result_set)
		{ move_shape(shape, old_bounds, shape->get_bounds(), result_set); }
	void query_intersection(ShapePtr const &shape, ResultSetPtr const &result)
		{ query_intersection(shape, shape->get_bounds(), result); }

	bool build_subclusters();

	static void bind(lua_State *L);

private:
	static unsigned const X_MASK = 1, Y_MASK = 2, Z_MASK = 4;
	static size_t const LEAF_SHAPES_THRESHOLD = 4;
	static math::scalar const SUBCLUSTER_DIAGONAL_SQ_THRESHOLD;

	typedef std::vector<Octree> subclusters_t;
	typedef std::set<ShapeWeakPtr> shapes_t;

	math::aabb<3> bounds_;
	math::vec<3> centre_;

	shapes_t shapes_;
	subclusters_t subclusters_;
};

}
