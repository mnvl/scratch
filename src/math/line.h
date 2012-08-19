#pragma once

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "ray.h"
#include "aabb.h"

namespace math
{

template<int N,class T=scalar>
class line {
public:
    static size_t const ARITY = N;
    typedef T scalar_t;
    typedef vec<ARITY, scalar_t> vec_t;
    typedef ray<ARITY, scalar_t> ray_t;
    typedef line<ARITY, scalar_t> line_t;
	typedef aabb<ARITY, scalar_t> aabb_t;

    vec_t A, B;

    line() {
    }

    line(const vec_t &a,const vec_t &b) {
        construct(a,b);
    }

    void construct(const vec_t &a,const vec_t &b) {
        A=a;
        B=b;
    }

    vec_t direction() const {
        return B - A;
    }

    ray_t get_ray() const {
        return ray_t(A, direction());
    }

	aabb_t get_aabb() const {
		aabb_t bounds(A, B);
		minmax(bounds.lo, bounds.hi);
		return bounds;
	}

    scalar_t length_sq() const {
        return direction().length_sq();
    }

	bool contains(vec_t const &point) const
	{
		vec_t u(point - A), v(direction());
		return u.is_collinear(v) && u.length_sq() < v.length_sq() && (u & v) > 0;
	}

	scalar_t distance_sq(vec_t const &point) const
	{
		vec_t dir(direction());
		scalar_t t = ((point - A) & dir) / dir.length_sq();

		if (t < 0) return (point - A).length();
		if (t > 1) return (point - B).length();

		vec_t projection = A + dir * t;
		return (projection - point).length_sq();
	}

	scalar_t distance(vec_t const &point) const
	{
		return sqrt(distance_sq(point));
	}
};

void bind_line(lua_State *L);

}

