#pragma once

#include <limits>
#include <luabind/lua_include.hpp>
#include <math/scalar.h>
#include <math/vec.h>

namespace math {

template<int N,class T=scalar>
class ray {
public:
    static const int ARITY = N;
    typedef T scalar_t;
    typedef vec<ARITY,scalar_t> vec_t;

    vec_t r0, a;

    ray() {
    }

    ray(const vec_t &origin, const vec_t &direction):
		r0(origin),
		a(direction)
	{
    }

    void set(const vec_t &origin,const vec_t &direction) {
        r0 = origin;
		a = direction;
    }

    vec_t apply(scalar_t t) const
	{
        return r0 + a * t;
    }

	bool contains(vec_t const &point, scalar_t epsilon = EPSILON)
	{
		return (point - r0).is_collinear(a, epsilon);
	}

	scalar_t distance_sq(vec_t const &point)
	{
		scalar_t t = ((point - r0) & a) / a.length_sq();
		vec_t projection = apply(t);
		return (projection - point).length_sq();
	}

	scalar_t distance(vec_t const &point)
	{
		return sqrt(distance_sq(point));
	}

	scalar_t distance(ray const &r) const;

	bool test_intersection(ray const &r) const
	{
		return distance(r) < EPSILON;
	}

	// intersection point is apply(t)
	bool query_intersection(ray const &r, scalar_t &t) const;
};

void bind_ray(lua_State *L);

}

