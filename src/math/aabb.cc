
#include <luabind/luabind.hpp>
#include "ray.h"
#include "aabb.h"

namespace math
{

namespace
{

template<class T>
bool trace_impl(aabb<2, T> const &b, ray<2, T> const &r, T *t0, T *t1, T t_min, T t_max)
{
	// not implemented
	assert(0);

	return false;
}

template<class T>
bool trace_impl(aabb<3, T> const &b, ray<3, T> const &r, T *t0, T *t1, T t_min, T t_max)
{
	*t0 = t_max;
	*t1 = t_min;

	ray<3, T> r1;
	b.world_to_local_ray(r1, r);

	if(abs(r1.a.x) > EPSILON) {
		T dx = 1 / r1.a.x;

		T t = -r1.r0.x * dx;
		vec<3, T> v;

		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;

			if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}

		t = dx + t;

		if(t >= t_min && t <= t_max) {
			v += r1.a * dx;

			if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}
	}

	if(abs(r1.a.y) > EPSILON) {
		T dy = 1 / r1.a.y;

		T t = -r1.r0.y * dy;
		vec<3, T> v;

		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;

			if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}

		t = dy + t;

		if(t >= t_min && t <= t_max) {
			v += r1.a * dy;

			if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}
	}

	if(abs(r1.a.z) > EPSILON) {
		T dz = 1 / r1.a.z;

		T t = -r1.r0.z * dz;
		vec<3, T> v;

		if(t >= t_min && t <= t_max) {
			v = r1.r0 + r1.a * t;

			if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}

		t = dz + t;

		if(t >= t_min && t <= t_max) {
			v += r1.a * dz;

			if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1)
			{
				*t0 = std::min(*t0, t);
				*t1 = std::max(*t1, t);
			}
		}
	}

	return *t0 != t_max;
}
}

template<int N, class T>
bool aabb<N, T>::trace(ray_t const &r, scalar_t *t0, scalar_t *t1, scalar_t t_min, scalar_t t_max) const
{
	return trace_impl(*this, r, t0, t1, t_min, t_max);
}

template<int N, class T>
bool aabb<N, T>::trace(ray_t const &r, scalar_t t_min, scalar_t t_max) const
{
	scalar_t t0, t1;

	return trace(r, &t0, &t1, t_min, t_max);
}

template class aabb<2>;
template class aabb<3>;

void bind_aabb(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<aabb<3> >("aabb3")
		.def(constructor<aabb<3> const &>())
		.def(constructor<vec<3> const &, vec<3> const &>())
		.def_readwrite("lo", &aabb<3>::lo)
		.def_readwrite("hi", &aabb<3>::hi)
		.def("contains", (bool (aabb<3>::*)(vec<3> const &) const) &aabb<3>::contains)
	];
}

}
