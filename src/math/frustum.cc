
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include "aabb.h"
#include "obb.h"
#include "frustum.h"

namespace math
{

template<class T>
frustum<T>::frustum()
{
}

template<class T>
frustum<T>::frustum(matrix<4,4> const &tf)
{
	load(tf);
}

template<class T>
frustum<T>::~frustum()
{
}

template<class T>
void frustum<T>::load(matrix<4,4> const &tf)
{
	// http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html

	// left clipping plane
	planes[PLANE_LEFT].A = tf._14 + tf._11;
	planes[PLANE_LEFT].B = tf._24 + tf._21;
	planes[PLANE_LEFT].C = tf._34 + tf._31;
	planes[PLANE_LEFT].D = tf._44 + tf._41;

	// right clipping plane
	planes[PLANE_RIGHT].A = tf._14 - tf._11;
	planes[PLANE_RIGHT].B = tf._24 - tf._21;
	planes[PLANE_RIGHT].C = tf._34 - tf._31;
	planes[PLANE_RIGHT].D = tf._44 - tf._41;

	// top clipping plane
	planes[PLANE_TOP].A = tf._14 - tf._12;
	planes[PLANE_TOP].B = tf._24 - tf._22;
	planes[PLANE_TOP].C = tf._34 - tf._32;
	planes[PLANE_TOP].D = tf._44 - tf._42;

	// bottom clipping plane
	planes[PLANE_BOTTOM].A = tf._14 + tf._12;
	planes[PLANE_BOTTOM].B = tf._24 + tf._22;
	planes[PLANE_BOTTOM].C = tf._34 + tf._32;
	planes[PLANE_BOTTOM].D = tf._44 + tf._42;

	// near clipping plane
	planes[PLANE_NEAR].A = tf._14 + tf._13;
	planes[PLANE_NEAR].B = tf._24 + tf._23;
	planes[PLANE_NEAR].C = tf._34 + tf._33;
	planes[PLANE_NEAR].D = tf._44 + tf._43;

	// far clipping plane
	planes[PLANE_FAR].A = tf._14 - tf._13;
	planes[PLANE_FAR].B = tf._24 - tf._23;
	planes[PLANE_FAR].C = tf._34 - tf._33;
	planes[PLANE_FAR].D = tf._44 - tf._43;
}

template<class T>
bool frustum<T>::contains(math::vec<3> const &point) const
{
	for(size_t i = 0; i < PLANES_COUNT; i++)
	{
		typename plane_t::classification_t cl = planes[i].classify(point);
		if (cl == plane_t::NEGATIVE) return false;
	}

	return true;
}

template<class T>
bool frustum<T>::test_intersection(aabb<3,T> const &bounds) const
{
	for (size_t i = 0; i < PLANES_COUNT; ++i)
	{
		if (planes[i].classify(bounds) == plane_t::NEGATIVE) return false;
	}

	return true;
}

template<class T>
bool frustum<T>::test_intersection(obb<3,T> const &bounds) const
{
	for (size_t i = 0; i < PLANES_COUNT; ++i)
	{
		if (planes[i].classify(bounds) == plane_t::NEGATIVE) return false;
	}

	return true;
}

template class frustum<>;

void bind_frustum(lua_State *L) {
	using namespace luabind;

	module(L, "math")
	[
		class_<frustum<> >("frustum")
		.enum_("const")
		[
			value("PLANES_COUNT", frustum<>::PLANES_COUNT)
		]
		.def_readonly("planes", &frustum<>::planes, return_stl_iterator)
		.def(constructor<math::matrix<4,4> const &>())
		.def("contains", &frustum<>::contains)
	];
}

}
