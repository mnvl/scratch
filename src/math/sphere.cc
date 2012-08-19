
#include <luabind/luabind.hpp>
#include "ray.h"
#include "line.h"
#include "capsule.h"
#include "sphere.h"

namespace math
{

template<int N, class T> bool sphere<N, T>::test_intersection(ray<N, T> const &r) const
{
	// |x-r0|=R
	// x=x0+a*t
	// |x0+a*t-r0|=R
	// (a*t+x0-r0,a*t+x0-r0)=R^2
	// (a*t,a*t+x0-r0)+(x0-r0,a*t+x0-r0)=R^2
	// (a,a)*t^2+(a,x0-r0)*t+(a,x0-r0)*t+(x0-r0,x0-r0)=R^2
	// (a,a)*t^2+(a,x0-r0)*2*t+(x0-r0,x0-r0)-R^2=0
	// A=(a,a)
	// B=(a,x0-r0)*2
	// C=(x0-r0,x0-r0)-R

	scalar_t A = r.a.length_sq();
	scalar_t B = 2 * (r.a & (r.r0 - centre));
	scalar_t C = (r.r0 - centre).length_sq() - radius * radius;

	return B * B - 4 * A * C >= 0;
}

template<int N, class T> int
sphere<N,T>::query_intersection(ray<N, T> const &r, vec_t &p1, vec_t &p2) const
{
	// p = r.direction * t + r.origin
	// |p - centre| ^ 2 = radius ^ 2
	// |r.direction * t + r.origin - centre| ^ 2 = radius ^ 2
	// delta = r.origin - centre
	// r.direction.length_sq() * t + 2 * (r.direction & delta) * t + delta.length_sq() - radius ^ 2 = 0

	vec_t delta = r.r0 - centre;
	scalar_t A = r.a.length_sq();
	scalar_t B = scalar_t(2) * (r.a & delta);
	scalar_t C = delta.length_sq() - square(radius);

	scalar_t discriminant = square(B) - 4 * A * C;

	if (discriminant < 0) {
		return false;
	}

	if (discriminant < EPSILON) {
		T t = -B / (2 * A);
		p1 = p2 = r.a * t + r.r0;
		return 1;
	}

	scalar_t discriminant_sqrt = sqrt(discriminant);

	scalar_t t1 = (-B + discriminant_sqrt) / (2 * A);
	p1 = r.a * t1 + r.r0;

	scalar_t t2 = (-B - discriminant_sqrt) / (2 * A);
	p2 = r.a * t1 + r.r0;

	return 2;
}

template<int N, class T> bool
sphere<N, T>::test_intersection(line<N, T> const &l) const
{
	vec_t p1, p2;
	scalar_t ll = l.length_sq();

	switch(query_intersection(l.get_ray(), p1, p2)) {
		case 0: return false;
		case 1: return (p1 - l.A).length_sq() < ll;
		default: return (p1 - l.A).length_sq() < ll || (p2 - l.A).length_sq() < ll;
	}
}

template<int N, class T> int
sphere<N, T>::query_intersection(line<N, T> const &l, vec_t &p1, vec_t &p2) const
{
	switch(query_intersection(l.get_ray(), p1, p2)) {
		case 0: return 0;
		case 1: return int(l.contains(p1));
		default: return int(l.contains(p1)) + int(l.contains(p2));
	}
}

template<int N, class T> bool
sphere<N, T>::test_intersection(capsule<N, T> const &c) const
{
	return c.distance(centre) <= radius;
}

template class sphere<2>;
template class sphere<3>;

void bind_sphere(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<sphere<3> >("sphere3")
		.def_readwrite("centre", &sphere<3>::centre)
		.def_readwrite("radius", &sphere<3>::radius)
	];
}

}
