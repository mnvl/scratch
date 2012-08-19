#pragma once

#include <luabind/luabind.hpp>
#include "line.h"

namespace math
{

template<int N, class T> class aabb;
template<int N, class T> class obb;

template<int N, class T = scalar>
class capsule {
public:
    static size_t const ARITY = N;
    typedef T scalar_t;
	typedef vec<ARITY, scalar_t> vec_t;
	typedef line<ARITY, scalar_t> line_t;

	line_t axe;
	scalar_t radius;

    capsule()
	{
    }

    capsule(vec_t const &a, vec_t const &b, scalar_t r):
		axe(a, b),
		radius(r)
	{
    }

	bool contains(vec_t const &point) const
	{
		return axe.distance(point) <= radius;
	}

	scalar_t distance(vec_t const &point) const
	{
		scalar_t d = axe.distance(point);
		if (d <= radius) return 0;
		return d - radius;
	}

	void get_aabb(aabb<ARITY, scalar_t> &bounds) const;
};

void bind_capsule(lua_State *L);

}

