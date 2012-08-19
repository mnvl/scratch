
#include <luabind/luabind.hpp>
#include "obb.h"

namespace math
{

template <class T> typename obb<3,T>::scalar_t
obb<3,T>::get_radius() const {
	vec_t centre = get_centre();
	scalar_t radius_sq = 0;
	for (int i = 0; i < VERTICES_COUNT; ++i) {
		vec_t pos;
		get_vertex(i, pos);
		scalar_t dist_sq = (pos - centre).length_sq();
		if (dist_sq > radius_sq) radius_sq = dist_sq;
	}
	return sqrt(radius_sq);
}

template <class T> void
obb<3,T>::world_to_local_vector(vec_t &v,vec_t const &v0) const
{
	matrix3_t world_to_local;
	world_to_local.world_to_axes(tangent, normal, binormal);

	v = v0 * world_to_local;
}

template<class T> void
obb<3,T>::world_to_local_plane(plane<scalar_t> &p, plane<scalar_t> const &p0) const
{
	vec_t origin, normal;
	world_to_local_point(origin, p0.get_origin());
	world_to_local_vector(normal, p0.get_normal());
	p.set(origin, normal);
}

template<class T> bool
obb<3,T>::trace(const ray<ARITY, scalar> &r, scalar_t t_min, scalar_t t_max) const {
	// перенести луч в локальную систему координат
	ray<ARITY, scalar> r1;
	world_to_local_ray(r1, r);

	// пересечение прямой r1 с плоскостьями x=0 и x=1
	if(abs(r1.a.x) > EPSILON) {
		scalar_t dx = 1 / r1.a.x;

		scalar_t t = -r1.r0.x * dx;
		vec_t v;
		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;
			if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
		}

		t = dx - t;
		if(t >= t_min && t <= t_max) {
			v += r1.a * dx;
			if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
		}
	}

	// пересечение прямой r1 с плоскостьями y=0 и y=1
	if(abs(r1.a.y) > EPSILON) {
		scalar_t dy = 1 / r1.a.y;

		scalar_t t = -r1.r0.y * dy;
		vec_t v;
		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;
			if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
		}

		t = dy - t;
		if(t >= t_min && t <= t_max) {
			v += r1.a * dy;
			if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
		}
	}

	// пересечение прямой r1 с плоскостьями z=0 и z=1
	if(abs(r1.a.z) > EPSILON) {
		scalar_t dz = 1 / r1.a.z;

		scalar_t t = -r1.r0.z * dz;
		vec_t v;
		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;
			if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
		}

		t = dz - t;
		if(t >= t_min && t <= t_max) {
			v += r1.a * dz;
			if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
		}
	}

	return false;
}

template<class T> bool obb<3,T>::contains(const obb &r) const {
	// получить координаты r в локальной системе координат
	vec_t r_origin, r_tangent, r_normal, r_binormal;
	world_to_local_point(r_origin,r.origin);
	world_to_local_vector(r_tangent,r.tangent);
	world_to_local_vector(r_normal,r.normal);
	world_to_local_vector(r_binormal,r.binormal);

	// получить координаты вершин obb
	vec_t v0 = r_origin;
	vec_t v1 = v0 + r_tangent;
	vec_t v2 = v1 + r_normal;
	vec_t v3 = v0 + r_normal;
	vec_t v4 = v0 + r_binormal;
	vec_t v5 = v1 + r_binormal;
	vec_t v6 = v2 + r_binormal;
	vec_t v7 = v3 + r_binormal;

	// один obb содержить второй если он содержит все вершины второго
	return (v0.x>=0 && v1.x>=0 && v2.x>=0 && v3.x>=0 && v4.x>=0 && v5.x>=0 && v6.x>=0 && v7.x>=0)
		&& (v0.y>=0 && v1.y>=0 && v2.y>=0 && v3.y>=0 && v4.y>=0 && v5.y>=0 && v6.y>=0 && v7.y>=0)
		&& (v0.z>=0 && v1.z>=0 && v2.z>=0 && v3.z>=0 && v4.z>=0 && v5.z>=0 && v6.z>=0 && v7.z>=0)
		&& (v1.x<=1 && v1.x<=1 && v2.x<=1 && v3.x<=1 && v4.x<=1 && v5.x<=1 && v6.x<=1 && v7.x<=1)
		&& (v1.y<=1 && v1.y<=1 && v2.y<=1 && v3.y<=1 && v4.y<=1 && v5.y<=1 && v6.y<=1 && v7.y<=1)
		&& (v1.z<=1 && v1.z<=1 && v2.z<=1 && v3.z<=1 && v4.z<=1 && v5.z<=1 && v6.z<=1 && v7.z<=1);
}

template<class T> bool
obb<3,T>::test_nonintersection(const triangle<ARITY, scalar_t> &r) const
{
	// получить координаты вершин треугольника в локальной системе координат
	vec_t A, B, C;
	world_to_local_point(A, r.A);
	world_to_local_point(B, r.B);
	world_to_local_point(C, r.C);

	// см. функцию test_nonintersection()
	if((A.x<0 && B.x<0 && C.x<0) || (A.y<0 && B.y<0 && C.y<0) || (A.z<0 && B.z<0 && C.z<0)
	|| (A.x>1 && B.x>1 && C.x>1) || (A.y>1 && B.y>1 && C.y>1) || (C.z>1 && B.z>1 && C.z>1))
	{
		return true;
	}

	// если ни одна из сторон obb не является разделяющей плоскостью, то проверить: находится ли obb
	// целиком в одном из полупространств, задаваемых треугольником
	vec_t N = (B - A) ^ (C - B);

	scalar_t k1 = (vec_t(0,0,0) - A) & N;
	scalar_t k2 = (vec_t(0,0,1) - A) & N;
	scalar_t k3 = (vec_t(0,1,0) - A) & N;
	scalar_t k4 = (vec_t(0,1,1) - A) & N;
	scalar_t k5 = (vec_t(1,0,0) - A) & N;
	scalar_t k6 = (vec_t(1,0,1) - A) & N;
	scalar_t k7 = (vec_t(1,1,0) - A) & N;
	scalar_t k8 = (vec_t(1,1,1) - A) & N;

	return (k1>0 && k2>0 && k3>0 && k4>0 && k5>0 && k6>0 && k7>0 && k8>0)
	    || (k1<0 && k2<0 && k3<0 && k4<0 && k5<0 && k6<0 && k7<0 && k8<0);
}

template<class T> bool obb<3,T>::test_nonintersection(const obb &r) const {
	// получить координаты r в локальной системе координат
	vec_t r_origin, r_tangent, r_normal, r_binormal;
	world_to_local_point(r_origin,r.origin);
	world_to_local_vector(r_tangent,r.tangent);
	world_to_local_vector(r_normal,r.normal);
	world_to_local_vector(r_binormal,r.binormal);

	// получить координаты вершин obb
	vec_t v0 = r_origin;
	vec_t v1 = v0 + r_tangent;
	vec_t v2 = v1 + r_normal;
	vec_t v3 = v0 + r_normal;
	vec_t v4 = v0 + r_binormal;
	vec_t v5 = v1 + r_binormal;
	vec_t v6 = v2 + r_binormal;
	vec_t v7 = v3 + r_binormal;

	// два obb не пересекаются если существует плоскость, которая делит прастранство на
	// два полупространства, одно из которых содержит первый obb, а второе -- второй
	// (достаточное условие)
	return (v0.x<0 && v1.x<0 && v2.x<0 && v3.x<0 && v4.x<0 && v5.x<0 && v6.x<0 && v7.x<0)
		|| (v0.y<0 && v1.y<0 && v2.y<0 && v3.y<0 && v4.y<0 && v5.y<0 && v6.y<0 && v7.y<0)
		|| (v0.z<0 && v1.z<0 && v2.z<0 && v3.z<0 && v4.z<0 && v5.z<0 && v6.z<0 && v7.z<0)
		|| (v1.x>1 && v1.x>1 && v2.x>1 && v3.x>1 && v4.x>1 && v5.x>1 && v6.x>1 && v7.x>1)
		|| (v1.y>1 && v1.y>1 && v2.y>1 && v3.y>1 && v4.y>1 && v5.y>1 && v6.y>1 && v7.y>1)
		|| (v1.z>1 && v1.z>1 && v2.z>1 && v3.z>1 && v4.z>1 && v5.z>1 && v6.z>1 && v7.z>1);
}

template<class T> bool
obb<3,T>::test_noncollision(const triangle<ARITY, scalar_t> &tri, const vec_t &vel, scalar_t t_min, scalar_t t_max) const
{
	vec_t tri_A, tri_B, tri_C, tri_vel;
	world_to_local_point(tri_A, tri.A);
	world_to_local_point(tri_B, tri.B);
	world_to_local_point(tri_C, tri.C);
	world_to_local_vector(tri_vel, vel);

        scalar_t t_len = t_max - t_min;
	vec_t tri_delta = tri_vel * t_len;

	if(t_min != 0) {
		vec_t tri_start = tri_vel * t_min;
		tri_A -= tri_start;
		tri_B -= tri_start;
		tri_C -= tri_start;
	}

    vec_t tri_A1 = tri_A + tri_delta;
	vec_t tri_B1 = tri_B + tri_delta;
	vec_t tri_C1 = tri_C + tri_delta;

	if((tri_A.x<0 && tri_B.x<0 && tri_C.x<0 && tri_A1.x<0 && tri_A1.x<0 && tri_A1.x<0)
	|| (tri_A.x>1 && tri_B.x>1 && tri_C.x>1 && tri_A1.x>1 && tri_A1.x>1 && tri_A1.x>1)
	|| (tri_A.y<0 && tri_B.y<0 && tri_C.y<0 && tri_A1.y<0 && tri_A1.y<0 && tri_A1.y<0)
	|| (tri_A.y>1 && tri_B.y>1 && tri_C.y>1 && tri_A1.y>1 && tri_A1.y>1 && tri_A1.y>1) 
	|| (tri_A.z<0 && tri_B.z<0 && tri_C.z<0 && tri_A1.z<0 && tri_A1.z<0 && tri_A1.z<0)
	|| (tri_A.z>1 && tri_B.z>1 && tri_C.z>1 && tri_A1.z>1 && tri_A1.z>1 && tri_A1.z>1))
	{
		return true;
	}

	vec_t tri_normal = (tri_B - tri_A) ^ (tri_C - tri_B);

	scalar_t k1 = (vec_t(0,0,0) - tri_A) & tri_normal;
	scalar_t k2 = (vec_t(0,0,1) - tri_A) & tri_normal;
	scalar_t k3 = (vec_t(0,1,0) - tri_A) & tri_normal;
	scalar_t k4 = (vec_t(0,1,1) - tri_A) & tri_normal;
	scalar_t k5 = (vec_t(1,0,0) - tri_A) & tri_normal;
	scalar_t k6 = (vec_t(1,0,1) - tri_A) & tri_normal;
	scalar_t k7 = (vec_t(1,1,0) - tri_A) & tri_normal;
	scalar_t k8 = (vec_t(1,1,1) - tri_A) & tri_normal;

	return (k1<0 && k2<0 && k3<0 && k4<0 && k5<0 && k6<0 && k7<0 && k8<0)
	    || (k1>0 && k2>0 && k3>0 && k4>0 && k5>0 && k6>0 && k7>0 && k8>0);
}

template<class T> bool
obb<3,T>::test_noncollision(const obb &box, const vec_t &vel, scalar_t t_min,
	scalar_t t_max) const
{
	// получить координаты box в локальной системе координат
	vec_t box_origin, box_tangent, box_normal, box_binormal, box_vel;
	world_to_local_point(box_origin,box.origin);
	world_to_local_vector(box_tangent,box.tangent);
	world_to_local_vector(box_normal,box.normal);
	world_to_local_vector(box_binormal,box.binormal);
	world_to_local_vector(box_vel, vel);

	// вычислить длину интервала
	scalar_t t_len = t_max - t_min;
	vec_t delta = box_vel * t_len;

	// получить координаты вершин obb
	vec_t v0_0 = box_origin - box_vel * t_min;
	vec_t v1_0 = v0_0 + box_tangent;
	vec_t v2_0 = v1_0 + box_normal;
	vec_t v3_0 = v0_0 + box_normal;
	vec_t v4_0 = v0_0 + box_binormal;
	vec_t v5_0 = v1_0 + box_binormal;
	vec_t v6_0 = v2_0 + box_binormal;
	vec_t v7_0 = v3_0 + box_binormal;
	vec_t v0_1 = v0_0 + delta;
	vec_t v1_1 = v1_0 + delta;
	vec_t v2_1 = v2_0 + delta;
	vec_t v3_1 = v3_0 + delta;
	vec_t v4_1 = v4_0 + delta;
	vec_t v5_1 = v5_0 + delta;
	vec_t v6_1 = v6_0 + delta;
	vec_t v7_1 = v7_0 + delta;

	// два obb не пересекаются если существует плоскость, которая делит прастранство на
	// два полупространства, одно из которых содержит первый obb, а второе -- второй
	// (достаточное условие)
	return (v0_0.x<0 && v1_0.x<0 && v2_0.x<0 && v3_0.x<0 && v4_0.x<0 && v5_0.x<0 && v6_0.x<0 && v7_0.x<0
	    &&  v0_1.x<0 && v1_1.x<0 && v2_1.x<0 && v3_1.x<0 && v4_1.x<0 && v5_1.x<0 && v6_1.x<0 && v7_1.x<0)
	    || (v0_0.y<0 && v1_0.y<0 && v2_0.y<0 && v3_0.y<0 && v4_0.y<0 && v5_0.y<0 && v6_0.y<0 && v7_0.y<0
	    &&  v0_1.y<0 && v1_1.y<0 && v2_1.y<0 && v3_1.y<0 && v4_1.y<0 && v5_1.y<0 && v6_1.y<0 && v7_1.y<0)
	    || (v0_0.z<0 && v1_0.z<0 && v2_0.z<0 && v3_0.z<0 && v4_0.z<0 && v5_0.z<0 && v6_0.z<0 && v7_0.z<0
	    &&  v0_1.z<0 && v1_1.z<0 && v2_1.z<0 && v3_1.z<0 && v4_1.z<0 && v5_1.z<0 && v6_1.z<0 && v7_1.z<0)
	    || (v1_0.x>1 && v1_0.x>1 && v2_0.x>1 && v3_0.x>1 && v4_0.x>1 && v5_0.x>1 && v6_0.x>1 && v7_0.x>1
	    &&  v1_1.x>1 && v1_1.x>1 && v2_1.x>1 && v3_1.x>1 && v4_1.x>1 && v5_1.x>1 && v6_1.x>1 && v7_1.x>1)
	    || (v1_0.y>1 && v1_0.y>1 && v2_0.y>1 && v3_0.y>1 && v4_0.y>1 && v5_0.y>1 && v6_0.y>1 && v7_0.y>1
	    &&  v1_1.y>1 && v1_1.y>1 && v2_1.y>1 && v3_1.y>1 && v4_1.y>1 && v5_1.y>1 && v6_1.y>1 && v7_1.y>1)
	    || (v1_0.z>1 && v1_0.z>1 && v2_0.z>1 && v3_0.z>1 && v4_0.z>1 && v5_0.z>1 && v6_0.z>1 && v7_0.z>1
	    &&  v1_1.z>1 && v1_1.z>1 && v2_1.z>1 && v3_1.z>1 && v4_1.z>1 && v5_1.z>1 && v6_1.z>1 && v7_1.z>1);
}

template<class T> bool
obb<3,T>::is_basis_linearly_independent() const
{
	return abs(tangent & normal) <= EPSILON && abs(tangent & binormal) <= EPSILON && abs(normal & binormal) <= EPSILON;
}

template class obb<3>;

void bind_obb(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<obb<3> >("obb3")
		.def(constructor<>())
		.def(constructor<aabb<3> const &>())
		.def_readwrite("origin", &obb<3>::origin)
		.def_readwrite("tangent", &obb<3>::tangent)
		.def_readwrite("normal", &obb<3>::normal)
		.def_readwrite("binormal", &obb<3>::binormal)
		.def("transform", &obb<3>::transform)
		.def("get_radius", &obb<3>::get_radius)
	];
}

}
