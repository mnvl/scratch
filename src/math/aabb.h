#pragma once

#include <limits>
#include <iosfwd>
#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"

#undef min
#undef max

namespace math
{

template<int N,class T> class triangle;
template<int N,class T> class ray;

template<int N,class T = scalar>
class aabb {
public:
	// типы и константы
	typedef T scalar_t;
	typedef std::numeric_limits<scalar_t> limits_t;
	static size_t const ARITY = N;
	typedef vec<ARITY,scalar_t> vec_t;
	static size_t const vertices_count = 1 << ARITY;
	static size_t const edges_count = vertices_count + (1 << (ARITY-1));
	typedef ray<ARITY,scalar_t> ray_t;

	// данные
	vec_t lo, hi;

	// конструкторы
	aabb()
	{
	}

	aabb(vec_t const &centre):
		lo(centre),
		hi(centre)
	{
	}

	aabb(vec_t const &l,vec_t const &h):
		lo(l),
		hi(h)
	{
	}

	aabb(vec_t const &centre, math::scalar radius)
	{
		vec_t delta;
		delta.set_all(radius / 2);

		lo = centre - delta;
		hi = centre + delta;
	}

	// операции
	void null() {
		for(size_t n=0; n<ARITY; n++) {
			lo.i[n] = +limits_t::max();
			hi.i[n] = -limits_t::max();
		}
	}

	void world_to_local_vector(vec_t &v,vec_t const &v0) const {
		v = v0 / (hi - lo);
	}

	// получить координаты точки в локальной системе координат
	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - lo);
	}

	// перенести прямую в локальную систему координат
	void world_to_local_ray(ray_t &r,ray_t const &r0) const {
		world_to_local_point(r.r0,r0.r0);
		world_to_local_vector(r.a,r0.a);
	}

	void normalize() {
		minmax(lo,hi);
	}

	vec_t diagonal() const {
		return hi - lo;
	}

	vec_t centre() const {
		return (lo + hi) / scalar_t(2);
	}

	void extend(vec_t const &v) {
		for(size_t n=0; n<ARITY; n++) {
			if(lo.i[n] > v.i[n]) lo.i[n] = v.i[n];
			if(hi.i[n] < v.i[n]) hi.i[n] = v.i[n];
		}
	}

	void extend(aabb const &b) {
		extend(b.lo);
		extend(b.hi);
	}

	// пересечения
	bool contains(vec_t const &right) const {
		return lo <= right && right <= hi;
	}

	bool contains(aabb const &right) const {
		return contains(right.lo) && contains(right.hi);
	}

	bool test_intersection(aabb const &right) const {
		return right.hi >= lo && hi >= right.lo;
	}

	bool trace(ray_t const &r, scalar_t *t0, scalar_t *t1, scalar_t t_min = 0, scalar_t t_max = 1) const;
	bool trace(ray_t const &r, scalar_t t_min = 0, scalar_t t_max = 1) const;

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & lo & hi;
	}
};

void bind_aabb(lua_State *L);

}

template<int N, class T>
std::ostream &operator <<(std::ostream &lhs, math::aabb<N,T> &rhs)
{
	return lhs << rhs.lo << " x " << rhs.hi;
}
