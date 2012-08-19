
#include <assert.h>
#include "octree.h"

namespace spatial
{

math::scalar const Octree::SUBCLUSTER_DIAGONAL_SQ_THRESHOLD = 1;

Octree::Octree(math::aabb<3> const &bounds):
	bounds_(bounds),
	centre_(bounds.centre())
{
}

Octree::~Octree()
{
}

math::aabb<3> Octree::get_subcluster_bounds(unsigned index) const
{
	math::vec<3> origin = bounds_.lo;
	math::vec<3> extents = bounds_.diagonal() / math::scalar(2);

	if (index & X_MASK) origin.x += extents.x;
	if (index & Y_MASK) origin.y += extents.y;
	if (index & Z_MASK) origin.z += extents.z;

	return math::aabb<3>(origin, origin + extents);
}

unsigned Octree::get_subcluster_by_point(math::vec<3> const &point) const
{
	unsigned index = 0;

	if (point.x >= centre_.x) index |= X_MASK;
	if (point.y >= centre_.y) index |= Y_MASK;
	if (point.z >= centre_.z) index |= Z_MASK;

	return index;
}

size_t Octree::get_subclusters_by_aabb(math::aabb<3> const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT]) const
{
	assert(bounds.lo <= bounds.hi);

	unsigned c1 = get_subcluster_by_point(bounds.lo);
	unsigned c2 = get_subcluster_by_point(bounds.hi);

	if (c1 == c2)
	{
		subclusters[0] = c1;
		return 1;
	}

	if (c1 == 0 && c2 == SUBCLUSTERS_COUNT - 1)
	{
		for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++) subclusters[i] = i;
		return SUBCLUSTERS_COUNT;
	}

	unsigned diff = c1 ^ c2;

	unsigned hi = diff & (diff - 1);
	unsigned lo = diff & ~hi;

	assert ((c1 | lo | hi) == c2);

	if (hi)
	{
		subclusters[0] = c1;
		subclusters[1] = c1 | lo;
		subclusters[2] = c1 | hi;
		subclusters[3] = c2;
		return 4;
	}
	else
	{
		subclusters[0] = c1;
		subclusters[1] = c2;
		return 2;
	}
}

unsigned Octree::get_subclusters_mask_by_aabb(math::aabb<3> const &bounds) const
{
	unsigned result = 0;

	unsigned subclusters[SUBCLUSTERS_COUNT];
	size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

	for (size_t i = 0; i < nsubclusters; i++)
	{
		result |= 1u << subclusters[i];
	}

	return result;
}

bool Octree::is_leaf() const
{
	return subclusters_.empty();
}

void Octree::add_shape(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result_set)
{
	if (is_leaf() && !build_subclusters())
	{
		if (result_set) query_intersection(shape, bounds, result_set);
		shapes_.insert(shape);
		return;
	}

	unsigned subclusters[SUBCLUSTERS_COUNT];
	size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

	if (nsubclusters == SUBCLUSTERS_COUNT)
	{
		if (result_set) query_intersection(shape, bounds, result_set);

		shapes_.insert(shape);
	}
	else
	{
		if (result_set) intersect_shallow(shape, bounds, result_set);

		for (size_t i = 0; i < nsubclusters; i++)
		{
			Octree *subcluster = &subclusters_[subclusters[i]];
			subcluster->add_shape(shape, bounds, result_set);
		}
	}
}

void Octree::remove_shape(ShapePtr const &shape, math::aabb<3> const &bounds)
{
	if (is_leaf())
	{
		shapes_.erase(shape);
	}
	else
	{
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		if (nsubclusters == SUBCLUSTERS_COUNT)
		{
			shapes_.erase(shape);
		}
		else
		{
			for (size_t i = 0; i < nsubclusters; i++)
			{
				subclusters_[subclusters[i]].remove_shape(shape, bounds);
			}
		}
	}
}

void Octree::move_shape(ShapePtr const &shape, math::aabb<3> const &old_bounds, math::aabb<3> const &new_bounds, ResultSetPtr const &result_set)
{
	if (is_leaf())
	{
		if (result_set) intersect_shallow(shape, new_bounds, result_set);
		return;
	}

	unsigned subclusters_old[SUBCLUSTERS_COUNT];
	size_t nsubclusters_old = get_subclusters_by_aabb(old_bounds, subclusters_old);

	unsigned subclusters_new[SUBCLUSTERS_COUNT];
	size_t nsubclusters_new = get_subclusters_by_aabb(new_bounds, subclusters_new);

	if (nsubclusters_old == SUBCLUSTERS_COUNT)
	{
		if (nsubclusters_new == SUBCLUSTERS_COUNT)
		{
			if (result_set) query_intersection(shape, new_bounds, result_set);
			return;
		}
		else
		{
			shapes_.erase(shape);
			add_shape(shape, new_bounds, result_set);
			return;
		}
	}
	else
	{
		if (nsubclusters_new == SUBCLUSTERS_COUNT)
		{
			remove_shape(shape, old_bounds);
			if (result_set) query_intersection(shape, new_bounds, result_set);
			shapes_.insert(shape);
			return;
		}
	}

	if (result_set) intersect_shallow(shape, new_bounds, result_set);

	size_t i = 0, j = 0;

	while (i < nsubclusters_old && j < nsubclusters_new)
	{
		if (subclusters_old[i] < subclusters_new[j])
		{
			// shape moved out of subclusters_old[i]
			subclusters_[subclusters_old[i]].remove_shape(shape, old_bounds);
			++i;
		}
		else if (subclusters_old[i] > subclusters_new[j])
		{
			// shape moved into subclusters_new[j]
			subclusters_[subclusters_new[j]].add_shape(shape, new_bounds, result_set);
			++j;
		}
		else
		{
			subclusters_[subclusters_old[i]].move_shape(shape, old_bounds, new_bounds, result_set);
			++i;
			++j;
		}
	}

	while (i < nsubclusters_old)
	{
		// shape moved out of subclusters_old[i]
		subclusters_[subclusters_old[i]].remove_shape(shape, old_bounds);
		++i;
	}

	while (j < nsubclusters_new)
	{
		// shape moved into subclusters_new[j]
		subclusters_[subclusters_new[j]].add_shape(shape, new_bounds, result_set);
		++j;
	}
}

void Octree::intersect_shallow(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result)
{
	shapes_t::iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			if (sh != shape && sh->get_bounds().test_intersection(bounds))
			{
				// maybe check here that result does not contain shape

				if (sh->test_intersection(shape.get()))
				{
					result->insert(sh);
				}
			}

			++it;
		}
		else
		{
			// garbage-collect weak pointers to deleted shapes
			shapes_.erase(it++);
		}
	}
}

void Octree::query_intersection(ShapePtr const &shape, math::aabb<3> const &bounds, ResultSetPtr const &result)
{
	if (result) intersect_shallow(shape, bounds, result);

	if (!is_leaf())
	{
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		for (size_t i = 0; i < nsubclusters; i++)
		{
			subclusters_[subclusters[i]].query_intersection(shape, bounds, result);
		}
	}
}

bool Octree::build_subclusters()
{
	if (shapes_.size() < LEAF_SHAPES_THRESHOLD) return false;
	if (bounds_.diagonal().length_sq() < SUBCLUSTER_DIAGONAL_SQ_THRESHOLD) return false;

	// create subclusters
	subclusters_.reserve(SUBCLUSTERS_COUNT);

	for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++)
	{
		subclusters_.push_back(Octree(get_subcluster_bounds(i)));
	}

	// move some shapes to subclusters
	shapes_t::iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			math::aabb<3> bounds = sh->get_bounds();

			unsigned subclusters[SUBCLUSTERS_COUNT];
			size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

			if (nsubclusters == SUBCLUSTERS_COUNT)
			{
				it++;
			}
			else
			{
				for (size_t i = 0; i < nsubclusters; i++)
				{
					subclusters_[subclusters[i]].add_shape(sh, sh->get_bounds());
				}

				shapes_.erase(it++);
			}
		}
		else
		{
			shapes_.erase(it++);
		}
	}

	return true;
}

}
