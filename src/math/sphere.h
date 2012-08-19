#pragma once

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "aabb.h"

namespace math
{

template<int N, class T> class ray;
template<int N, class T> class line;
template<int N, class T> class capsule;

template<int N, class T = scalar>
class sphere {
public:
	static size_t const ARITY = N;
	typedef T scalar_t;
	typedef vec<ARITY,scalar_t> vec_t;

	vec_t centre;
	scalar_t radius;

	sphere()
	{
	}

	sphere(aabb<N, T> const &a)
	{
		construct(a);
	}

	sphere(vec_t const &c, scalar_t r):
		centre(c),
		radius(r)
	{
	}

	void construct(aabb<N, T> const &a)
	{
		centre = (a.lo + a.hi) / 2;
		radius = (a.hi - centre).length();
	}

	void set(vec_t const &c, scalar_t r)
	{
		centre = c;
		radius = r;
	}

	aabb<N, T> get_aabb() const
	{
		assert(radius >= 0);

		vec_t r;
		r.set_all(radius);
		return aabb<N, T>(centre - r, centre + r);
	}

	bool contains(vec_t const &v) const
	{
		return (v - centre).length_sq() < radius * radius;
	}

	bool test_intersection(sphere<N, T> const &s) const
	{
		return (centre - s.centre).length_sq() <= square(radius + s.radius);
	}

	bool test_intersection(ray<N, T> const &r) const;
	int query_intersection(ray<N, T> const &r, vec_t &p1, vec_t &p2) const;
	bool test_intersection(line<N, T> const &l) const;
	int query_intersection(line<N, T> const &l, vec_t &p1, vec_t &p2) const;
	bool test_intersection(capsule<N, T> const &c) const;
};

void bind_sphere(lua_State *L);

}
