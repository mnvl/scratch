#pragma once

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "ray.h"

namespace math {

template<int N, class T> class aabb;
template<int N, class T> class obb;

template<class T = scalar>
class plane {
public:
	typedef T scalar_t;
	static size_t const ARITY=3;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef ray<ARITY,scalar_t> ray_t;

	enum classification_t {
		POSITIVE = 1,
		INTERSECTS,
		NEGATIVE,
	};

	// данные
	scalar_t A, B, C, D;

	// конструкторы
	plane() {
	}

	plane(vec_t const &o, vec_t const &n) {
		set(o, n);
	}

	plane(scalar_t d, vec_t const &normal) {
		set(d, normal);
	}

	void set(vec_t const &o, vec_t const &n) {
		A = n.x;	B = n.y;	C = n.z;	D = -(o & n);
	}

	void set(scalar_t d, vec_t const &normal) {
		A = normal.x;	B = normal.y;	C = normal.z;	D = d;
	}

	scalar_t apply(vec_t const &v) const {
		return A*v.x + B*v.y + C*v.z + D;
	}

	bool test_intersection(const ray_t &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z) > EPSILON || contains(r);
	}

	bool contains(vec_t const &p) const {
		return abs(A * p.x + B * p.y + C * p.z + D) < EPSILON;
	}

	bool contains(const ray_t &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z) < EPSILON && contains(r.r0);
	}

	bool parallel(const ray_t &r) const {
		return !test_intersection(r);
	}

	scalar_t trace(ray_t const &r) const {
		scalar_t a_n = A * r.a.x + B * r.a.y + C * r.a.z;
		scalar_t d = apply(r.r0);
		return -d / a_n;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	bool trace(const ray_t &r,scalar_t &t,scalar_t t_min = 0, scalar_t t_max = 1) const {
		t = trace(r);
		return t >= t_min && t <= t_max;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	// v -- точка пересечения
	bool trace(const ray_t &r,scalar_t &t,vec_t &v,scalar_t t_min = 0, scalar_t t_max = 1) const {
		if(!trace(r,t,t_min,t_max)) return false;
		v = r.r0 + r.a * t;
		return true;
	}

	bool is_correct() const { return abs(A) > 0 || abs(B) > 0 || abs(C) > 0; }

	vec_t get_origin() const;
	vec_t get_normal() const { return vec_t(A, B, C); }

	// returns false if planes are parallel
	bool query_intersection(const plane &p, ray_t &r) const;

	classification_t classify(vec_t const &point) const;
	classification_t classify(aabb<ARITY, scalar_t> const &bounds) const;
	classification_t classify(obb<ARITY, scalar_t> const &bounds) const;
};

void bind_plane(lua_State *L);

}
