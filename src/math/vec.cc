
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "vec.h"

namespace math
{

template <class T> vec<3,T> const vec<3,T>::I(1, 0, 0);
template <class T> vec<3,T> const vec<3,T>::J(0, 1, 0);
template <class T> vec<3,T> const vec<3,T>::K(0, 0, 1);
template <class T> vec<3,T> const vec<3,T>::null(0, 0, 0);

template<class T> void
vec<2,T>::to_polar(scalar_t &modulus,scalar_t &angle) const
{
	modulus = length();
	angle = atan2(y, x);
}

template<class T> void
vec<3,T>::to_axis_angle(vec &axis, scalar_t &angle) const
{
	const vec original(1,0,0);

	axis = original ^ *this;
	axis.normalize();

	vec basis_y = axis ^ original;

	vec<2,T> xy;
	xy.x = x / length();
	xy.y = *this & basis_y;

	scalar_t t;
	xy.to_polar(t, angle);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_xy() const
{
	if (abs(x) + abs(y) < EPSILON) return K;
	return vec(y, -x, 0);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_yz() const
{
	if (abs(y) + abs(z) < EPSILON) return I;
	return vec(0, z, -y);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_xz() const
{
	if (abs(x) + abs(z) < EPSILON) return J;
	return vec(z, 0, -x);
}

template class vec<2>;
template class vec<3>;
template class vec<4>;

namespace
{

template<int N, class T> std::string serialize(math::vec<N, T> const &v)
{
	return std::string((char const *) v.i, sizeof(v.i));
}

template<int N, class T> void deserialize(math::vec<N, T> &v, std::string const &str)
{
	if (str.size() != sizeof(v)) throw std::runtime_error("can't deserialize");
	memcpy(v.i, str.c_str(), str.size());
}

}

template<class T>
void bind_vec2(lua_State *L, char const *name)
{
	using namespace luabind;

	module(L, "math")
	[
		class_< vec<2, T> >(name)
		.def(constructor<>())
		.def(constructor<scalar, scalar>())
		.def_readwrite("x", &vec<2, T>::x)
		.def_readwrite("y", &vec<2, T>::y)
		.def("set", &vec<2, T>::set)
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self < const_self)
		.def(const_self == const_self)
		.def(tostring(const_self))
	 ];
}

void bind_vec(lua_State *L)
{
	using namespace luabind;

	bind_vec2<int>(L, "vec2i");
	bind_vec2<scalar>(L, "vec2");

	module(L, "math")
	[
		class_<vec<3> >("vec3")
		.def(constructor<>())
		.def(constructor<scalar, scalar, scalar>())
		.def(constructor<vec<3> const &>())
		.def_readwrite("x", &vec<3>::x)
		.def_readwrite("y", &vec<3>::y)
		.def_readwrite("z", &vec<3>::z)
		.def(-const_self)
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * other<scalar>())
		.def(const_self / other<scalar>())
		.def(const_self < const_self)
		.def(const_self == const_self)
		.def("length", &vec<3>::length)
		.def("length_sq", &vec<3>::length_sq)
		.def("normalize", &vec<3>::normalize)
		.def("set", &vec<3>::set)
		.def("perpendicular_xy", &vec<3>::perpendicular_xy)
		.def("perpendicular_yz", &vec<3>::perpendicular_yz)
		.def("perpendicular_xz", &vec<3>::perpendicular_xz)
		.def(tostring(const_self)),

		class_<vec<3,bool> >("vec3b")
		.def(constructor<>())
		.def(constructor<bool, bool, bool>())
		.def(constructor<vec<3, bool> const &>())
		.def_readwrite("x", &vec<3,bool>::x)
		.def_readwrite("y", &vec<3,bool>::y)
		.def_readwrite("z", &vec<3,bool>::z)
		.def(const_self < const_self)
		.def(const_self == const_self)
		.def(tostring(const_self)),

		class_<vec<4> >("vec4")
		.def(constructor<>())
		.def(constructor<scalar, scalar, scalar, scalar>())
		.def(constructor<vec<4> const &>())
		.def_readwrite("x", &vec<4>::x)
		.def_readwrite("y", &vec<4>::y)
		.def_readwrite("z", &vec<4>::z)
		.def_readwrite("w", &vec<4>::w)
		.def(const_self < const_self)
		.def(const_self == const_self)
		.def(tostring(const_self)),

		def("dot_product", (scalar (*)(const vec<2> &,const vec<2> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<3> &,const vec<3> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<4> &,const vec<4> &)) dot_product),

		def("cross_product", (vec<3> (*)(const vec<3> &,const vec<3> &)) operator ^),
		def("cross_product", (void (*)(vec<3> &, const vec<3> &,const vec<3> &)) cross_product),

		def("serialize", &serialize<2, scalar>),
		def("serialize", &serialize<3, scalar>),
		def("serialize", &serialize<4, scalar>),

		def("deserialize", &deserialize<2, scalar>),
		def("deserialize", &deserialize<3, scalar>),
		def("deserialize", &deserialize<4, scalar>)
	];
}

}
