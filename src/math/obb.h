#pragma once

#include <assert.h>
#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "aabb.h"
#include "ray.h"
#include "line.h"
#include "triangle.h"
#include "matrix.h"

namespace math
{

template<class T> class plane;
template<int N,class T> class triangle;

template<int N,class T=scalar> class obb;

template<class T>
class obb<2, T> {
public:
	typedef T scalar_t;
	static size_t const ARITY = 2;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef matrix<ARITY+1,ARITY+1,scalar_t> matrix_t;
	typedef aabb<ARITY,scalar_t> aabb_t;
	static size_t const SIDES_COUNT = 4;
	static size_t const VERTICES_COUNT = 4;

	vec_t origin, tangent, normal;

	obb() {
	}

	obb(aabb_t const &rhs) {
		origin = rhs.lo;
		tangent.set(rhs.hi.x - rhs.lo.x, 0);
		normal.set(0, rhs.hi.y - rhs.lo.y);
	}

	void world_to_local_vector(vec_t &v, vec_t const &v0) const {
		v.x = (v0 & tangent) / tangent.length_sq();
		v.y = (v0 & normal)	 / normal.length_sq();
	}

	vec_t world_to_local_vector(vec_t const &v0) const {
		vec_t v;
		world_to_local_vector(v, v0);
		return v;
	}

	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - origin);
	}

	vec_t world_to_local_point(vec_t const &v0) const {
		vec_t v;
		world_to_local_point(v, v0);
		return v;
	}

	bool contains(vec_t const &v) const {
		vec_t v1;
		world_to_local_point(v1, v);
		return v1.x>=0 && v1.x<=1
		    && v1.y>=0 && v1.y<=1;
	}

	void transform(matrix_t const &tf) {
		vec_t O = origin, Ox = tangent + origin, Oy = normal + origin;
		mul(origin, O, tf);
		mul(tangent, Ox, tf);
		mul(normal, Oy, tf);
		tangent -= origin;
		normal -= origin;
	}

	vec_t get_vertex(size_t vertex_number) const {
		switch (vertex_number) {
			case 0: return origin;
			case 1: return origin + tangent;
			case 2: return origin + tangent + normal;
			case 3: return origin + normal;
			default: assert(0); return vec_t(0, 0);
		}
	}

	line<ARITY, scalar_t> get_side(size_t side_number) const {
		line<ARITY, scalar_t> side;
		side.A = get_vertex(side_number);
		side.B = get_vertex((1 + side_number) % SIDES_COUNT);
		return side;
	}

	vec_t get_normal(size_t side_number) const {
		switch (side_number) {
			case 0: return math::normalize(-normal);
			case 1: return math::normalize(tangent);
			case 2: return math::normalize(normal);
			case 3: return math::normalize(-tangent);
			default: assert(0); return vec_t(0, 0);
		}
	}

	void get_aabb(aabb_t &b) const {
		b.lo = b.hi = origin;
		b.extend(origin + tangent);
		b.extend(origin + normal);
		b.extend(origin + tangent + normal);
	}
};

template<class T>
class obb<3,T> {
public:
	typedef T scalar_t;
	static size_t const ARITY = 3;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef matrix<ARITY, ARITY, scalar_t> matrix3_t;
	typedef matrix<ARITY+1, ARITY+1, scalar_t> matrix_t;
	typedef aabb<ARITY,scalar_t> aabb_t;
	static size_t const SIDES_COUNT = 6;
	static size_t const VERTICES_COUNT = 8;
	static size_t const SEGMENTS_COUNT = 4;

	vec_t origin, tangent, normal, binormal;

	obb() {
	}

	obb(const aabb_t &a)
	{
		operator =(a);
	}

	obb(vec_t const &o, vec_t const &i, vec_t const &j, vec_t const &k):
		origin(o),
		tangent(i),
		normal(j),
		binormal(k)
	{
	}

	obb &operator =(const aabb_t &a) {
		origin = a.lo;
		tangent.set(a.hi.x - a.lo.x, 0, 0);
		normal.set(0, a.hi.y - a.lo.y, 0);
		binormal.set(0, 0, a.hi.z - a.lo.z);
		return *this;
	}

	vec_t get_centre() const {
		return origin + (tangent + normal + binormal) / scalar_t(2);
	}

	scalar_t get_radius() const;

	scalar_t get_volume() const {
		return tangent.length() * normal.length() * binormal.length();
	}

	// получить aabb
	void get_aabb(aabb_t &b) const {
		b.lo = b.hi = origin;
		b.extend(origin + tangent);
		b.extend(origin + normal);
		b.extend(origin + binormal);
		b.extend(origin + tangent + normal);
		b.extend(origin + tangent + binormal);
		b.extend(origin + normal + binormal);
		b.extend(origin + tangent + normal + binormal);
	}

	// трансформировать obb матрицей tf
	void transform(const matrix_t &tf) {
		// получить координаты точек - вершин
		vec_t O = origin,
			 Ox = tangent + origin,
			 Oy = normal + origin,
			 Oz = binormal + origin;

		// трансформировать эти координаты
		mul(origin, O, tf);
		mul(tangent, Ox, tf);
		mul(normal, Oy, tf);
		mul(binormal, Oz, tf);

		// вычислить значения tangent, normal, binormal относительно origin
		tangent -= origin;
		normal -= origin;
		binormal -= origin;
	}

	// получить координаты вершины
	void get_vertex(size_t vertex_number, vec_t &position) const {
		position = origin +	scalar_t(vertex_number & 1) * tangent;
		if (vertex_number & 2) position += normal;
		if (vertex_number & 4) position += binormal;
	}

	// получить obb сегмента
	void get_segment(size_t segment_number, obb &segment_obb) {
		segment_obb.tangent = tangent / scalar_t(2);
		segment_obb.normal = normal / scalar_t(2);
		segment_obb.binormal = binormal	/ scalar_t(2);
		segment_obb.origin = origin;

		if(segment_number & 1) segment_obb.origin += segment_obb.tangent;
		if(segment_number & 2) segment_obb.origin += segment_obb.normal;
		if(segment_number & 4) segment_obb.origin += segment_obb.binormal;
	}

	// получить координаты вектора в локальной системе координат с
	// базисом (tangent,normal,binormal)
	void world_to_local_vector(vec_t &v, vec_t const &v0) const;

	void local_to_world_vector(vec_t &v, vec_t const &v0) const {
		v = v0.x * tangent + v0.y * normal + v0.z * binormal;
	}

	// получить координаты точки в локальной системе координат
	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - origin);
	}

	// перенести прямую в локальную систему координат
	void world_to_local_ray(ray<ARITY, scalar> &r,const ray<ARITY, scalar> &r0) const {
		world_to_local_point(r.r0,r0.r0);
		world_to_local_vector(r.a,r0.a);
	}

	void world_to_local_plane(plane<scalar_t> &p, plane<scalar_t> const &p0) const;

	// проверить принадлежность точки
	bool contains(vec_t const &v) const {
		vec_t v1;
		world_to_local_point(v1, v);
		return v1.x>=0 && v1.x<=1
		    && v1.y>=0 && v1.y<=1
		    && v1.z>=0 && v1.z<=1;
	}

	// проверить принадлежность треугольника
	bool contains(const triangle<ARITY, scalar_t> &tri) const {
		return contains(tri.A) && contains(tri.B) && contains(tri.C);
	}

	// проверить пересечение с лучом
	 bool trace(const ray<ARITY, scalar> &r, scalar_t t_min = 1,
			 scalar_t t_max = 1) const;

	// проверить на содержание obb
	bool contains(const obb &r) const;

	// пересекаются ли obb и треугольник?
	bool test_intersection(const triangle<ARITY, scalar_t> &tri) const {
		return !test_nonintersection(tri);
	}

	// пересекаются ли два obb?
	bool test_intersection(const obb &box) const {
		return !test_nonintersection(box) && !box.test_nonintersection(*this);
	}

	// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
	bool test_collision(triangle<ARITY, scalar_t> const &tri, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const
	{
		return !test_noncollision(tri, vel, t_min, t_max);
	}

	// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
	bool test_collision(obb const &box, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const
	{
		return !test_noncollision(box, vel, t_min, t_max)
		    && !box.test_noncollision(*this, -vel, t_min, t_max);
	}

	bool is_basis_linearly_independent() const;

private:
	// проверить на не-пересечение с треугольником
	bool test_nonintersection(const triangle<ARITY, scalar_t> &r) const;

	// проверить на одностороннее не-пересечение с obb
	bool test_nonintersection(const obb &r) const;

	// проверить на одностороннее не-столкновение с треугольником
	// r -- данный треугольник
	// vel -- скорость box относительно настоящего obb
	// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
	bool test_noncollision(const triangle<ARITY, scalar_t> &r, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const;

	// проверить на одностороннее не-столкновение с obb
	// r -- данный obb
	// vel -- скорость box относительно настоящего obb
	// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
	bool test_noncollision(const obb &r, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const;
};

void bind_obb(lua_State *L);

}

