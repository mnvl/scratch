
#include <luabind/luabind.hpp>
#include "matrix.h"
#include "ray.h"

namespace math
{

namespace
{

template<class T> T distance(ray<2,T> const &r1, ray<2,T> const &r2)
{
	if (!r1.a.is_collinear(r2.a)) return 0;
	return (r1.r0 - r2.r0).length();
}

template<class T> bool closest_aproach(ray<3, T> const &r1, T &t1, ray<3, T> const &r2, T &t2)
{
	vec<3, T> p2_p1 = r2.r0 - r1.r0;
	vec<3, T> a1_x_a2 = r1.a ^ r2.a;

	T k = a1_x_a2.length_sq();
	if (k < EPSILON) return false;

	t1 = ((p2_p1 ^ r2.a) & a1_x_a2) / k;
	t2 = ((p2_p1 ^ r1.a) & a1_x_a2) / k;

	return true;
}

template<class T> T distance(ray<3, T> const &r1, ray<3, T> const &r2)
{
	T t1, t2;
	if (!closest_aproach(r1, t1, r2, t2)) return 0;
	return (r1.apply(t1) - r2.apply(t2)).length();
}

template<class T> bool query_intersection(ray<3, T> const &r1, ray<3, T> const &r2, T &t)
{
	T t2;
	if (!closest_aproach(r1, t, r2, t2)) return 0;
	return (r1.apply(t) - r2.apply(t2)).length_sq() < EPSILON; 
}

template<class T> bool query_intersection(ray<2, T> const &r1, ray<2, T> const &r2, T &s)
{
	return query_intersection(ray<3,T>(r1.r0, r1.a), ray<3,T>(r2.r0, r2.a), s);
}

}

template<int N, class T> T
ray<N, T>::distance(ray const &r) const
{
	return math::distance(*this, r);
}

template<int N, class T> bool
ray<N, T>::query_intersection(ray const &r, scalar_t &t) const
{
	return math::query_intersection(*this, r, t);
}

template class ray<2>;
template class ray<3>;

void bind_ray(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<ray<2> >("ray2")
		.def(constructor<>())
		.def(constructor<const vec<2> &, const vec<2> &>())
		.def("set", &ray<2>::set)
		.def("apply", &ray<2>::apply)
		.def_readwrite("r0", &ray<2>::r0)
		.def_readwrite("a", &ray<2>::a),

		class_<ray<3> >("ray3")
		.def(constructor<>())
		.def(constructor<const vec<3> &, const vec<3> &>())
		.def("set", &ray<3>::set)
		.def("apply", &ray<3>::apply)
		.def_readwrite("r0", &ray<3>::r0)
		.def_readwrite("a", &ray<3>::a)
	];
}

}
